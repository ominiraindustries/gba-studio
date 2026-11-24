# PROVISIONAL sound effects for 007_survivors block 1 (audio/tmp_*.wav), synthesised placeholders until
# real sounds exist. Usage: python -I projects/007_survivors/tests/gen_tmp_sfx.py
import math
import os
import random
import struct
import wave

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "audio")
RATE = 16000


def write(name, samples):
    with wave.open(os.path.join(OUT, name + ".wav"), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(1)
        w.setframerate(RATE)
        w.writeframes(bytes(max(0, min(255, int(128 + s * 100))) for s in samples))


def tone(f0, f1, dur, wave_kind="square", vol=0.8, noise=0.0):
    n = int(RATE * dur)
    out = []
    ph = 0.0
    rnd = random.Random(1)
    for i in range(n):
        t = i / n
        f = f0 + (f1 - f0) * t
        ph += f / RATE
        if wave_kind == "square":
            s = 1.0 if (ph % 1.0) < 0.5 else -1.0
        elif wave_kind == "tri":
            s = 4 * abs((ph % 1.0) - 0.5) - 1
        else:
            s = math.sin(ph * 2 * math.pi)
        s = s * (1 - noise) + rnd.uniform(-1, 1) * noise
        out.append(s * vol * (1 - t) ** 1.5)
    return out


write("tmp_hit", tone(900, 300, 0.06, "square", 0.6, 0.4))
write("tmp_gem", tone(1800, 2400, 0.07, "tri", 0.7))
write("tmp_level", tone(500, 500, 0.08, "square", 0.6) + tone(750, 750, 0.08, "square", 0.6)
      + tone(1000, 1300, 0.2, "square", 0.6))
write("tmp_hurt", tone(400, 120, 0.15, "square", 0.8, 0.5))
write("tmp_boss", tone(120, 60, 0.5, "square", 0.9, 0.6))
write("tmp_poof", tone(300, 100, 0.08, "tri", 0.5, 0.7))
print("ok")
