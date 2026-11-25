"""Convert the PixelLab audio originals in art/audio_src/ into what Butano/Maxmod plays (audio/).

Run: python -I art/audio_src/convert_audio.py   (from projects/007_survivors)

- SFX (sfx/*.wav, 44.1 kHz stereo 16-bit) -> audio/sfx_<name>.wav: mono, 16 kHz, 8-bit, trailing silence trimmed,
  short fade-out, peak-normalised.
- Music (music/*.mp3 + loop points from PixelLab make_music_loop) -> audio/mus_<name>.it: an Impulse Tracker module
  with two 8-bit samples: the intro (played once) and the loop body (loop_start..loop_end, forward sample loop that
  repeats seamlessly). Each is triggered once by a note; Maxmod plays the module with bn::music.
  Maxmod's mixer keeps the read position as 20.12 fixed point, so a sample must stay under 2^20 frames: the rate
  is 15768 Hz (the real MM_MIX_16KHZ mixing rate) unless the loop body needs a lower one.

MP3 decoding and resampling use the ffmpeg binary bundled with the imageio-ffmpeg Python package (already installed
in miniforge; nothing new was installed).
"""

import struct
import subprocess
import sys
import wave
from pathlib import Path

import numpy as np
import imageio_ffmpeg

HERE = Path(__file__).resolve().parent
AUDIO = HERE.parent.parent / "audio"
FFMPEG = imageio_ffmpeg.get_ffmpeg_exe()
MAX_FRAMES = (1 << 20) - 64

# name: (source mp3, loop start s, loop end s, max rate)
MUSIC = {
    "title": ("music/title_loop.mp3", None, None, 16000),
    "meadow": ("music/stage1_loop.mp3", None, None, 16000),
    "boss": ("music/boss_loop.mp3", None, None, 16000),
    "graveyard": ("music/graveyard_loop.mp3", None, None, 16000),
    "keep": ("music/keep_loop.mp3", None, None, 16000),
    "shop": ("music/shop_loop.mp3", None, None, 16000),
    "final": ("music/final_loop.mp3", None, None, 16000),
}
LOOPS_FILE = HERE / "music" / "loops.txt"   # lines: name loop_start loop_end (seconds, from get_music)

SFX = ["spark", "hit", "poof", "gem", "levelup", "card", "blink", "nova", "hurt", "wave", "boss",
       "howl", "horn", "stomp", "explosion", "zap", "chest", "coin_burst",
       # blocks 3-5
       "root_crack", "slam", "thorn", "curse_sting", "buy", "refund", "menu_move", "error", "evolve",
       "wail", "arrow", "rise", "skull", "teleport", "bone", "breath", "meteor", "burst", "slash"]
# Short one-shot jingles (no loop): same processing as SFX, saved as audio/<name>.wav (no sfx_ prefix).
JINGLES = ["jingle_victory", "jingle_defeat", "jingle_unlock", "jingle_boss_down"]
SFX_RATE = 16000


def decode(path, rate):
    out = subprocess.run([FFMPEG, "-v", "error", "-i", str(path), "-ac", "1", "-ar", str(rate), "-f", "s16le", "-"],
                         check=True, capture_output=True).stdout
    return np.frombuffer(out, dtype=np.int16).astype(np.float32) / 32768.0


def to_s8(x, peak=0.97):
    m = float(np.max(np.abs(x))) or 1.0
    x = x * (peak / m)
    return np.clip(np.round(x * 127.0), -128, 127).astype(np.int8)


def convert_sfx(name, out_name=None):
    x = decode(HERE / "sfx" / f"{name}.wav", SFX_RATE)
    thr = 10 ** (-42 / 20) * float(np.max(np.abs(x)))
    idx = np.nonzero(np.abs(x) > thr)[0]
    start, end = (int(idx[0]), int(idx[-1]) + 1) if len(idx) else (0, len(x))
    x = x[start:end]
    fade = min(len(x), SFX_RATE // 100)   # 10 ms
    x[-fade:] *= np.linspace(1.0, 0.0, fade)
    # Normalise on the 99.9th percentile (not the single highest sample) and soft-clip, so one stray click does
    # not leave the whole effect quiet.
    ref = max(float(np.percentile(np.abs(x), 99.9)), 1e-6)
    x = np.tanh(x * (1.1 / ref)) / np.tanh(1.1)
    s8 = to_s8(x, 0.92)
    dst = AUDIO / f"{out_name or 'sfx_' + name}.wav"
    with wave.open(str(dst), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(1)
        w.setframerate(SFX_RATE)
        w.writeframes((s8.astype(np.int16) + 128).astype(np.uint8).tobytes())
    return dst, len(s8) / SFX_RATE


MIX_RATE = 15768                 # Maxmod MM_MIX_16KHZ real rate (= BN_AUDIO_MIXING_RATE_16_KHZ)
TEMPO = 125
TICK_S = ((39420 // TEMPO) & ~1) / MIX_RATE   # Maxmod samples_per_tick for MM_MIX_16KHZ, made even
OVERLAP_S = 0.02                 # intro tail that overlaps the body note, faded out (no gap if timing drifts)


def _sample_header(title, n, rate, loop, data_off):
    flags = 0x01 | (0x10 if loop else 0)
    s = b"IMPS" + b"MUSIC.RAW".ljust(12, b"\0") + b"\0" + bytes([64, flags, 64])
    s += title.encode("ascii")[:25].ljust(26, b"\0") + bytes([0x01, 32])   # signed samples, pan centre
    s += struct.pack("<IIIIIII", n, 0, n if loop else 0, rate, 0, 0, data_off)
    s += bytes([0, 0, 0, 0])
    assert len(s) == 80
    return s


def write_it(dst, title, intro, intro_rate, body, body_rate, body_row):
    """Two samples: intro (once, channel 1, row 0) and body (forward loop over the whole sample, channel 2,
    row body_row). After the body note the song idles on a pattern that jumps to itself (B command), so no note is
    ever retriggered: the mixer's sample loop repeats the body seamlessly."""
    rows_per_pat = 200
    n_rows = body_row + 1
    n_pats = (n_rows + rows_per_pat - 1) // rows_per_pat
    events = {0: bytes([0x81, 0x03, 60, 1]), body_row: bytes([0x82, 0x03, 60, 2])}
    patterns = []
    for p in range(n_pats):
        data = b""
        for r in range(rows_per_pat):
            g = p * rows_per_pat + r
            data += events.get(g, b"") + b"\0"
        patterns.append(struct.pack("<HH4x", len(data), rows_per_pat) + data)
    idle = b"\0" * 63 + bytes([0x81, 0x08, 2, n_pats, 0])   # B<n_pats>: jump to this same order forever
    patterns.append(struct.pack("<HH4x", len(idle), 64) + idle)
    orders = bytes(list(range(n_pats + 1)) + [255])

    header_size = 192
    off = header_size + len(orders) + 4 * (2 + len(patterns))
    smp_offs = [off, off + 80]
    off += 160
    pat_offs = []
    for p in patterns:
        pat_offs.append(off)
        off += len(p)
    intro_off = off
    body_off = off + len(intro)

    name = title.encode("ascii")[:25].ljust(26, b"\0")
    h = b"IMPM" + name + bytes([4, 16])
    h += struct.pack("<HHHHHHHH", len(orders), 0, 2, len(patterns), 0x0214, 0x0200, 0x0009, 0)
    h += bytes([128, 48, 1, TEMPO, 128, 0])   # global vol, mix vol, speed 1, tempo, separation, pwd
    h += struct.pack("<HII", 0, 0, 0)
    h += bytes([32, 32] + [160] * 62)          # channel pan (channels 1-2 enabled)
    h += bytes([64] * 64)
    assert len(h) == header_size
    h += orders + b"".join(struct.pack("<I", o) for o in smp_offs + pat_offs)
    h += _sample_header(title + " intro", len(intro), intro_rate, False, intro_off)
    h += _sample_header(title + " loop", len(body), body_rate, True, body_off)
    dst.write_bytes(h + b"".join(patterns) + intro.tobytes() + body.tobytes())


def convert_music(name, loops):
    src, _, _, max_rate = MUSIC[name]
    ls, le = loops[name]
    body_row = int(-(-ls // TICK_S))           # first tick at or after the loop start
    pad = body_row * TICK_S - ls                # silence before the intro so the body lands on its tick
    intro_rate = min(max_rate, MIX_RATE)
    body_rate = min(max_rate, MIX_RATE, int(MAX_FRAMES / (le - ls)) // 10 * 10)
    xi = decode(HERE / src, intro_rate)
    xb = decode(HERE / src, body_rate)
    peak = max(float(np.max(np.abs(xi[: int(le * intro_rate)]))), 1e-6)
    tail = int(OVERLAP_S * intro_rate)
    intro = np.concatenate([np.zeros(int(round(pad * intro_rate)), np.float32),
                            xi[: int(round(ls * intro_rate)) + tail].copy()])
    intro[-tail:] *= np.linspace(1.0, 0.0, tail)
    body = xb[int(round(ls * body_rate)): int(round(le * body_rate))]
    q = lambda x: np.clip(np.round(x / peak * 0.95 * 127.0), -128, 127).astype(np.int8)
    dst = AUDIO / f"music_{name}.it"
    write_it(dst, f"HEXBOUND {name}", q(intro), intro_rate, q(body), body_rate, body_row)
    return dst, intro_rate, body_rate, len(intro) / intro_rate, len(body) / body_rate


def main():
    AUDIO.mkdir(exist_ok=True)
    for n in SFX:
        if not (HERE / "sfx" / f"{n}.wav").exists():
            continue
        dst, secs = convert_sfx(n)
        print(f"{dst.name}: {secs:.2f} s, {dst.stat().st_size} B")
    for n in JINGLES:
        if (HERE / "sfx" / f"{n}.wav").exists():
            dst, secs = convert_sfx(n, n)
            print(f"{dst.name}: {secs:.2f} s, {dst.stat().st_size} B")
    loops = {}
    if LOOPS_FILE.exists():
        for line in LOOPS_FILE.read_text().split("\n"):
            parts = line.split()
            if len(parts) == 3:
                loops[parts[0]] = (float(parts[1]), float(parts[2]))
    for n in MUSIC:
        if n in loops and (HERE / MUSIC[n][0]).exists():
            dst, ir, br, isec, bsec = convert_music(n, loops)
            print(f"{dst.name}: intro {isec:.2f} s @ {ir} Hz, loop {bsec:.2f} s @ {br} Hz, "
                  f"{dst.stat().st_size} B")


if __name__ == "__main__":
    sys.exit(main())
