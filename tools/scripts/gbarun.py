#!/usr/bin/env python3
"""Run a GBA ROM headless in Mesen 2 (--testrunner + Lua) and collect screenshots / memory checks.

Usage (Git Bash):
  python -I tools/scripts/gbarun.py projects/000_hello/000_hello.gba [options]

Options:
  --frames N            total frames to run (default 120)
  --shot F[,F...]       frames to screenshot (default: last frame); every F must be 1..--frames
  --input SPEC          keypad script, ';'-separated "F[-F2]:key+key", e.g. "30-90:right;100:a+b"
                        keys: a b l r start select up down left right
  --check SPEC[,SPEC]   pass/fail at the last frame: LOC OP VALUE, OP in == != <= >= < >
  --check-at "F:SPEC[,SPEC];F2:SPEC"   the same checks at intermediate frames of the same run
                        LOC = [mem:]addr_or_symbol[+/-off][.8|.16|.32]   (default width .8, little endian,
                        unsigned). mem: ewram iwram vram pal oam rom sram (offsets inside the region) or
                        bus (full GBA address, the default). Symbols come from the .elf next to the ROM
                        (arm-none-eabi-nm -C), e.g. "test_player_x.32>=140"  "ewram:0x100.16==3"
  --dump SPEC[,SPEC]    memory dumps at the last frame: mem[:start:len] -> mem.bin (mem as above, not bus)
  --nonblank            fail if any screenshot is one solid colour
  --golden DIR          compare each shot with DIR/shot_NNNN.png; any differing pixel or a missing reference
                        fails and writes diff_shot_NNNN.png. --update-golden (re)writes the references
  --sheet PNG           contact sheet of all shots
  --out DIR             output dir (default <rom dir>/run); old .png/.bin/.json/.lua there are deleted
  --timeout S           seconds before Mesen is stopped (default GBARUN_TIMEOUT or 60)

Exit codes: 0 pass, 1 a check/golden/nonblank failed, 2 Mesen produced no result, 3 bad arguments.
Mesen: $MESEN, else tools/mesen/Mesen.exe, else ../SNES/tools/mesen/Mesen.exe (Mesen 2.1+, which emulates GBA).
A GBA BIOS is required: if Mesen's Firmware/gba_bios.bin is missing, tools/bios/cult_of_gba_bios.bin (open source)
is copied there. Colour correction and frame blending are forced off, so shots are the raw RGB555 picture.
Runs are serialised machine-wide with the same lock file as the SNES repo's snesrun.py (both share
Documents/Mesen2). On timeout only the Mesen process started by this script is killed.
"""
import argparse, contextlib, json, os, pathlib, re, shutil, subprocess, sys, tempfile, textwrap, time

ROOT = pathlib.Path(__file__).resolve().parents[2]
MESEN_LOCK = pathlib.Path(tempfile.gettempdir()) / "snuts_mesen_testrunner.lock"  # shared with snesrun.py
KEYS = {"a", "b", "l", "r", "start", "select", "up", "down", "left", "right"}
MEM = {"bus": "gbaMemory", "ewram": "gbaExtWorkRam", "iwram": "gbaIntWorkRam", "vram": "gbaVideoRam",
       "pal": "gbaPaletteRam", "oam": "gbaSpriteRam", "rom": "gbaPrgRom", "sram": "gbaSaveRam"}
OPS = ("==", "!=", "<=", ">=", "<", ">")
W, H = 240, 160
SYMS = {}


def die(msg):
    print(f"gbarun: error: {msg}", file=sys.stderr)
    sys.exit(3)


def main_root():
    """The main checkout (a worktree under .claude/worktrees/ shares its git dir)."""
    try:
        common = subprocess.run(["git", "-C", str(ROOT), "rev-parse", "--path-format=absolute", "--git-common-dir"],
                                capture_output=True, text=True, timeout=10).stdout.strip()
        return pathlib.Path(common).parent if common else ROOT
    except (OSError, subprocess.SubprocessError):
        return ROOT


def find_mesen():
    main = main_root()
    for c in (os.environ.get("MESEN"), ROOT / "tools/mesen/Mesen.exe", main / "tools/mesen/Mesen.exe",
              ROOT.parent / "SNES/tools/mesen/Mesen.exe", main.parent / "SNES/tools/mesen/Mesen.exe"):
        if c and pathlib.Path(c).is_file():
            return pathlib.Path(c)
    die("Mesen.exe not found: set MESEN=<path to Mesen 2.1+ Mesen.exe> (see tools/README.md)")


def ensure_bios(mesen):
    """Mesen needs a GBA BIOS (Firmware/gba_bios.bin in its home). Without one the CPU never leaves reset and every
    screenshot is a grey screen. If none is present, install the open-source Cult-of-GBA BIOS (MIT, tools/bios/)."""
    home = mesen.parent if (mesen.parent / "settings.json").exists() else pathlib.Path.home() / "Documents/Mesen2"
    fw = home / "Firmware" / "gba_bios.bin"
    if not fw.exists():
        fw.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT / "tools/bios/cult_of_gba_bios.bin", fw)
        print(f"NOTE: installed the open Cult-of-GBA BIOS as {fw}", file=sys.stderr)


def find_nm():
    dka = os.environ.get("DEVKITARM")
    for c in ([pathlib.Path(dka) / "bin/arm-none-eabi-nm.exe"] if dka else []) + [
            ROOT / "toolchain/devkitPro/devkitARM/bin/arm-none-eabi-nm.exe"]:
        if c.is_file():
            return c
    return shutil.which("arm-none-eabi-nm")


def load_syms(rom):
    elf = rom.with_suffix(".elf")
    nm = find_nm()
    if not elf.exists() or not nm:
        return
    out = subprocess.run([str(nm), "-C", str(elf)], capture_output=True, text=True).stdout
    for line in out.splitlines():
        parts = line.split(maxsplit=2)
        if len(parts) == 3 and parts[1] in "bBdDgGsSrR" and len(parts[1]) == 1:
            SYMS[parts[2]] = int(parts[0], 16)


@contextlib.contextmanager
def mesen_slot(path=MESEN_LOCK):
    """Machine-wide testrunner slot (an OS byte lock, released even if this process dies)."""
    path.parent.mkdir(parents=True, exist_ok=True)
    handle = path.open("a+b")
    handle.seek(0, 2)
    if not handle.tell():
        handle.write(b"0")
        handle.flush()
    handle.seek(0)
    start = time.monotonic()
    import msvcrt
    while True:
        try:
            msvcrt.locking(handle.fileno(), msvcrt.LK_NBLCK, 1)
            break
        except OSError:
            time.sleep(.05)
    try:
        yield round((time.monotonic() - start) * 1000)
    finally:
        handle.seek(0)
        msvcrt.locking(handle.fileno(), msvcrt.LK_UNLCK, 1)
        handle.close()


def run_mesen(cmd, out, timeout):
    """Run Mesen once (retry once only if it left no result.json). Kills only its own process."""
    attempts = []
    with mesen_slot() as waited:
        if waited > 100:
            print(f"NOTE: waited {waited} ms for another Mesen testrunner", file=sys.stderr)
        for number in (1, 2):
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            try:
                so, se = p.communicate(timeout=timeout + 15)
            except subprocess.TimeoutExpired:
                p.kill()  # this PID only
                so, se = p.communicate()
                se += "\n[gbarun] Mesen killed after the timeout"
            attempts.append(dict(attempt=number, exit=p.returncode, stdout=so[-2000:], stderr=se[-2000:]))
            if (out / "result.json").exists():
                break
    return attempts


def lua_str(s):
    return json.dumps(str(s).replace("\\", "/"))


def parse_input(spec):
    out = []
    for part in filter(None, (x.strip() for x in (spec or "").split(";"))):
        rng, sep, keys = part.partition(":")
        if not sep:
            die(f"bad --input part {part!r}: expected F[-F2]:key+key")
        a, _, b = rng.partition("-")
        names = [k.strip().lower() for k in keys.split("+")]
        if set(names) - KEYS:
            die(f"unknown keys {set(names) - KEYS} (use {' '.join(sorted(KEYS))})")
        out.append((int(a), int(b or a), names))
    return out


def parse_loc(spec):
    """'[mem:]addr_or_symbol[+/-off][.8|.16|.32]' -> (mem, address, width_bytes)"""
    spec = spec.strip()
    m = re.fullmatch(r"(?:(\w+):(?!:))?(.+?)(?:\.(8|16|32))?", spec)
    mem, addr, bits = m[1], m[2], int(m[3] or 8)
    width = bits // 8
    sm = re.fullmatch(r"([A-Za-z_][\w:]*)(?:\s*([+-])\s*(0[xX][0-9A-Fa-f]+|\d+))?", addr)
    if sm and not re.fullmatch(r"\d+|0[xX][0-9A-Fa-f]+", sm[1]):
        if sm[1] not in SYMS:
            die(f"unknown symbol {sm[1]!r} (is there a .elf next to the ROM? is the variable global, not static?)")
        if mem not in (None, "bus"):
            die(f"{spec!r}: symbols are bus addresses, drop the {mem}: prefix")
        off = int(sm[3], 0) if sm[3] else 0
        return "bus", SYMS[sm[1]] + (-off if sm[2] == "-" else off), width
    mem = mem or "bus"
    if mem not in MEM:
        die(f"unknown memory {mem!r} in {spec!r} (use {', '.join(MEM)})")
    try:
        return mem, int(addr, 0), width
    except ValueError:
        die(f"bad address {addr!r} in {spec!r}")


def parse_check(c):
    op = next((o for o in OPS if o in c), None)
    if op is None:
        die(f"bad check {c!r}: no operator (use {' '.join(OPS)})")
    lhs, val = c.split(op, 1)
    mem, addr, width = parse_loc(lhs)
    try:
        v = int(val.strip(), 0)
    except ValueError:
        die(f"bad check {c!r}: value {val.strip()!r} is not a number")
    return mem, addr, width, v, c.strip(), "~=" if op == "!=" else op


def parse_check_at(spec):
    out = []
    for part in filter(None, (x.strip() for x in (spec or "").split(";"))):
        f, sep, rest = part.partition(":")
        if not sep or not f.strip().isdigit():
            die(f"bad --check-at part {part!r}: expected F:check[,check]")
        out += [(int(f), parse_check(c)) for c in filter(None, (x.strip() for x in rest.split(",")))]
    return out


def build_lua(a, out, shots):
    checks = [parse_check(c) for c in filter(None, (a.check or "").split(","))]
    check_at = parse_check_at(a.check_at)
    bad = [f"--check-at {f}" for f, _ in check_at if not 1 <= f <= a.frames]
    if bad:
        die("frames outside 1..--frames: " + ", ".join(bad))
    dumps = []
    for d in filter(None, (a.dump or "").split(",")):
        p = d.split(":")
        if p[0] not in MEM or p[0] == "bus":
            die(f"bad --dump {d!r}: memory must be one of {', '.join(k for k in MEM if k != 'bus')}")
        dumps.append((p[0], int(p[1], 0) if len(p) > 1 else 0, int(p[2], 0) if len(p) > 2 else -1))
    inputs = parse_input(a.input)

    def chk_lua(c, frame=None):
        mem, addr, width, val, src, op = c
        fr = "" if frame is None else f',\"frame\":{frame}'
        return (f"do local v = rd({addr}, emu.memType.{MEM[mem]}, {width}); local pass = (v {op} {val}); "
                f"ok = ok and pass; results[#results+1] = string.format('{{\"check\":%q,\"value\":%d,\"pass\":%s{fr}}}', "
                f"{lua_str(src)}, v, tostring(pass)) end")

    at = {}
    for f, c in check_at:
        at.setdefault(f, []).append(chk_lua(c, f))
    L = [f"local OUT = {lua_str(out)}", f"local LAST = {a.frames}",
         "local SHOTS = {" + ",".join(f"[{s}]=true" for s in shots) + "}",
         "local INPUTS = {" + ",".join("{%d,%d,{%s}}" % (f0, f1, ",".join(f"{k}=true" for k in ks))
                                       for f0, f1, ks in inputs) + "}",
         "local frame, ok, results = 0, true, {}"]
    L.append(textwrap.dedent("""
    local function write(path, data, mode)
      local f = assert(io.open(OUT .. "/" .. path, mode or "wb")); f:write(data); f:close()
    end
    local function rd(addr, mt, width)
      local v = 0
      for i = width - 1, 0, -1 do v = (v << 8) | emu.read(addr + i, mt) end
      return v
    end
    local function dump(name, mt, start, len)
      if len < 0 then len = emu.getMemorySize(mt) - start end
      local t = {}
      for i = 0, len - 1 do t[#t + 1] = string.char(emu.read(start + i, mt)) end
      write(name .. ".bin", table.concat(t))
    end
    -- emu.getScreenBuffer() at the START of the next frame = the finished picture of the frame just ended
    local function shot(f)
      local buf = emu.getScreenBuffer()
      local parts = {}
      for i = 1, #buf do
        local v = buf[i]
        parts[i] = string.char((v >> 16) & 255, (v >> 8) & 255, v & 255)
      end
      write(string.format("shot_%04d.rgb", f), table.concat(parts))
    end
    local function pad_now()
      local pad = {}
      for _, r in ipairs(INPUTS) do
        if frame + 1 >= r[1] and frame + 1 <= r[2] then for k, v in pairs(r[3]) do pad[k] = v end end
      end
      return pad
    end
    local pending_shot, stop_code = nil, nil
    emu.addEventCallback(function()
      if pending_shot then shot(pending_shot); pending_shot = nil end
      if stop_code then emu.stop(stop_code) return end
      emu.setInput(pad_now(), 0)
    end, emu.eventType.startFrame)
    emu.addEventCallback(function() emu.setInput(pad_now(), 0) end, emu.eventType.inputPolled)
    emu.addEventCallback(function()
      if stop_code then return end
      frame = frame + 1
      if SHOTS[frame] then pending_shot = frame end
    """))
    for f, lines in sorted(at.items()):
        L.append(f"  if frame == {f} then " + " ".join(lines) + " end")
    L.append("  if frame < LAST then return end")
    for mem, start, ln in dumps:
        L.append(f"  dump({lua_str(mem)}, emu.memType.{MEM[mem]}, {start}, {ln})")
    L += ["  " + chk_lua(c) for c in checks]
    L.append(textwrap.dedent("""
      write("result.json", '{"frames":' .. frame .. ',"checks":[' .. table.concat(results, ",") .. '],"ok":' .. tostring(ok) .. '}', "w")
      stop_code = ok and 0 or 1
    end, emu.eventType.endFrame)
    """))
    return "\n".join(L)


def rgb_to_png(out):
    from PIL import Image
    for raw in sorted(out.glob("shot_*.rgb")):
        data = raw.read_bytes()
        if len(data) != W * H * 3:
            print(f"WARNING: {raw.name}: {len(data)} bytes, expected {W * H * 3}", file=sys.stderr)
        n = len(data) // 3
        Image.frombytes("RGB", (W, n // W), data[: (n // W) * W * 3]).save(raw.with_suffix(".png"))
        raw.unlink()


def golden_compare(out, golden, update, data):
    from PIL import Image, ImageChops
    if update:
        golden.mkdir(parents=True, exist_ok=True)
    results, shots = [], sorted(out.glob("shot_*.png"))
    for s in shots:
        ref = golden / s.name
        if update:
            ref.write_bytes(s.read_bytes())
            results.append(f"{s.name}: reference updated - review it")
            continue
        if not ref.exists():
            data["ok"] = False
            results.append(f"{s.name}: MISSING reference {ref} (--update-golden creates it)")
            continue
        a_img, b_img = Image.open(ref).convert("RGB"), Image.open(s).convert("RGB")
        if a_img.size != b_img.size:
            data["ok"] = False
            results.append(f"{s.name}: DIFFERENT size")
            continue
        diff = ImageChops.difference(a_img, b_img).convert("L").point(lambda v: 255 if v else 0)
        n = sum(1 for v in diff.tobytes() if v)
        if n:
            data["ok"] = False
            vis = Image.blend(b_img, Image.new("RGB", b_img.size), 0.6)
            vis.paste((255, 0, 0), mask=diff)
            vis.save(out / f"diff_{s.name}")
            results.append(f"{s.name}: DIFFERENT ({n} px) -> diff_{s.name}")
        else:
            results.append(f"{s.name}: identical")
    produced = {s.name for s in shots}
    for f in (sorted(golden.glob("shot_*.png")) if golden.is_dir() else []):
        if f.name not in produced:
            if update:
                f.unlink()
            results.append(f"{f.name}: stale reference{' DELETED' if update else ''}")
    data["golden"] = results
    print("golden:", *results, sep="\n  ")


def contact_sheet(out, path, cols=4):
    from PIL import Image, ImageDraw
    shots = sorted(out.glob("shot_*.png"))
    if not shots:
        return
    rows = (len(shots) + cols - 1) // cols
    sheet = Image.new("RGB", (cols * W, rows * (H + 12)), (40, 40, 40))
    d = ImageDraw.Draw(sheet)
    for i, s in enumerate(shots):
        x, y = (i % cols) * W, (i // cols) * (H + 12)
        sheet.paste(Image.open(s).convert("RGB"), (x, y + 12))
        d.text((x + 2, y), s.stem, fill=(255, 255, 255))
    sheet.save(path)


def main():
    ap = argparse.ArgumentParser(description="Headless GBA test runner (Mesen 2)",
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument("rom")
    ap.add_argument("--frames", type=int, default=120)
    ap.add_argument("--shot")
    ap.add_argument("--input")
    ap.add_argument("--check")
    ap.add_argument("--check-at")
    ap.add_argument("--dump")
    ap.add_argument("--nonblank", action="store_true")
    ap.add_argument("--golden")
    ap.add_argument("--update-golden", action="store_true")
    ap.add_argument("--sheet")
    ap.add_argument("--out")
    ap.add_argument("--timeout", type=int, default=int(os.environ.get("GBARUN_TIMEOUT") or 60))
    a = ap.parse_args()
    rom = pathlib.Path(a.rom).resolve()
    if not rom.is_file():
        die(f"ROM not found: {rom}")
    if a.frames < 1:
        die("--frames must be >= 1")
    shots = sorted({int(x) for x in a.shot.split(",")}) if a.shot else [a.frames]
    if any(not 1 <= s <= a.frames for s in shots):
        die("--shot frames must be within 1..--frames")
    if a.update_golden and not a.golden:
        die("--update-golden needs --golden DIR")
    load_syms(rom)
    out = pathlib.Path(a.out).resolve() if a.out else rom.parent / "run"
    script = build_lua(a, out, shots)  # validates every spec before touching anything
    out.mkdir(parents=True, exist_ok=True)
    for f in out.glob("*"):
        if f.suffix in (".png", ".bin", ".json", ".lua", ".rgb"):
            f.unlink()
    lua = out / "_run.lua"
    lua.write_text(script, encoding="utf-8")
    mesen = find_mesen()
    ensure_bios(mesen)
    cmd = [str(mesen), "--testrunner", str(rom), str(lua), f"--timeout={a.timeout}",
           "--debug.scriptWindow.allowIoOsAccess=true", "--doNotSaveSettings",
           # the settings file is shared with the user's Mesen window: force the raw picture
           "--video.videoFilter=None",
           "--gba.disableFrameSkipping=true", "--gba.skipBootScreen=true",
           "--gba.gbaAdjustColors=false", "--gba.blendFrames=false",  # raw RGB555 -> RGB888, no LCD look
           "--gba.ramPowerOnState=AllZeros"]
    attempts = run_mesen(cmd, out, a.timeout)
    res = out / "result.json"
    if not res.exists():
        print("FAIL: no result.json from Mesen")
        for t in attempts:
            print(f"attempt {t['attempt']}: exit {t['exit']}\nstdout: {t['stdout']}\nstderr: {t['stderr']}")
        sys.exit(2)
    rgb_to_png(out)
    data = json.loads(res.read_text())
    data["exit"] = attempts[-1]["exit"]
    data["retried"] = len(attempts) > 1
    for c in data["checks"]:
        if not c["pass"]:
            print(f"FAIL: check{' at frame ' + str(c['frame']) if 'frame' in c else ''}: {c['check']} "
                  f"(value {c['value']})")
    if a.nonblank:
        from PIL import Image
        blank = [f.name for f in sorted(out.glob("shot_*.png"))
                 if len(Image.open(f).convert("RGB").getcolors(1 << 16) or [0, 0]) <= 1]
        data["blank_shots"] = blank
        if blank:
            data["ok"] = False
            print("FAIL: solid-colour screenshots:", ", ".join(blank))
    if a.golden:
        golden_compare(out, pathlib.Path(a.golden).resolve(), a.update_golden, data)
    if a.sheet:
        contact_sheet(out, pathlib.Path(a.sheet).resolve())
    data["files"] = sorted(f.name for f in out.iterdir() if f.suffix in (".png", ".bin"))
    print(json.dumps(data, indent=1))
    print("out:", out)
    sys.exit(0 if data["ok"] else 1)


if __name__ == "__main__":
    main()
