#!/usr/bin/env python3
"""Measure how a GBA ROM does things, headless in Mesen 2 (study of commercial games, also works on our ROMs).

Two sub-commands (Git Bash, output always to the scratchpad, never to the repo):

  record ROM --from F --to F [--input SPEC] [--ram LIST] [--shot-every N] [--boxes] --out DIR
      Per frame in [from, to]: OAM decoded (x, y, w, h, tile, palette bank, priority, affine/param, flips, mode,
      8bpp, mosaic), display IO (DISPCNT, DISPSTAT, BGxCNT, BG scroll, BG2/3 affine, WIN0/1 rects, WININ/WINOUT,
      BLDCNT/BLDALPHA/BLDY, MOSAIC), palette RAM (512 colours, written only when it changes) and an optional RAM list.
      Writes frames.jsonl (everything), oam.csv (visible sprites), io.csv, ram.csv, summary.json and, with
      --shot-every, a contact sheet (sheet.png; --boxes draws each visible sprite's box over the picture).

  search ROM --snap F1,F2[,F3...] --rule R1[,R2...] [--input SPEC] [--width 8|16|32] [--signed] --out DIR
      Classic memory search: snapshots of EWRAM + IWRAM at each --snap frame, then keeps the addresses whose value
      follows every rule between consecutive snapshots. Rules: '+' increased, '-' decreased, '=' unchanged,
      '!' changed, '+N' / '-N' changed by exactly N, '==V' equals V at the later snapshot.
      Example (player x while holding right, then still, then left):
        --snap 30,70,100,140 --input "31-70:right;101-140:left" --rule "+40,=,-40"
      Candidates are printed with the nearest .elf symbol when the ROM has one (our own ROMs).

  --input  keypad script as in gbarun.py: "F[-F2]:key+key;..." (a b l r start select up down left right)
  --poke   "F:LOC=V;..." (record) writes V at the end of frame F, e.g. to confirm that a found address is the one
           that moves the sprite: "200:ewram:0x1F40.16=50"
  --ram    "name=LOC[s],..." with LOC as in gbarun.py ([mem:]addr_or_symbol[.8|.16|.32]); a trailing 's' = signed,
           e.g. "px=ewram:0x1F40.16s,py=0x02001F44.16s,hp=iwram:0x120.8"

Timing: values are sampled at Mesen's endFrame event (the start of vblank, scanline 160). What a game changes
mid-frame (HBlank DMA / HBlank IRQ raster effects) shows only its end-of-frame value; scroll, window and BLDY
come from Mesen's PPU state because those registers are write-only on the bus.
OAM sampled at endFrame is the picture just drawn; a game variable is usually one frame ahead of its sprite
(Butano commits OAM in the next vblank). Shot F and OAM row F match (the --boxes overlay relies on it).
Commercial ROMs are the user's own dumps and live OUTSIDE the repo; a warning is printed otherwise.
Exit codes: 0 ok, 1 nothing found (search), 2 Mesen produced no result, 3 bad arguments.
"""
import argparse, csv, json, pathlib, sys, textwrap

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))  # python -I does not add the script dir
import gbarun  # noqa: E402  (find_mesen, ensure_bios, run_mesen, parse_input, parse_loc, load_syms, ...)

W, H = gbarun.W, gbarun.H
SIZES = {0: [(8, 8), (16, 16), (32, 32), (64, 64)],   # shape square
         1: [(16, 8), (32, 8), (32, 16), (64, 32)],   # wide
         2: [(8, 16), (8, 32), (16, 32), (32, 64)]}   # tall
# readable display IO (bus): name -> address
IO_BUS = {"DISPCNT": 0x04000000, "DISPSTAT": 0x04000004, "BG0CNT": 0x04000008, "BG1CNT": 0x0400000A,
          "BG2CNT": 0x0400000C, "BG3CNT": 0x0400000E, "WININ": 0x04000048, "WINOUT": 0x0400004A,
          "BLDCNT": 0x04000050, "BLDALPHA": 0x04000052}
# write-only registers: taken from Mesen's PPU state
IO_STATE = {**{f"BG{i}HOFS": f"ppu.bgLayers[{i}].scrollX" for i in range(4)},
            **{f"BG{i}VOFS": f"ppu.bgLayers[{i}].scrollY" for i in range(4)},
            **{f"BG{i + 2}P{'ABCD'[j]}": f"ppu.transform[{i}].matrix[{j}]" for i in range(2) for j in range(4)},
            **{f"BG{i + 2}X": f"ppu.transform[{i}].originX" for i in range(2)},
            **{f"BG{i + 2}Y": f"ppu.transform[{i}].originY" for i in range(2)},
            **{f"WIN{i}_{k}": f"ppu.window[{i}].{v}" for i in range(2)
               for k, v in (("L", "leftX"), ("R", "rightX"), ("T", "topY"), ("B", "bottomY"))},
            "BLDY": "ppu.brightness", "BG_MOSAIC_H": "ppu.bgMosaicSizeX", "BG_MOSAIC_V": "ppu.bgMosaicSizeY",
            "OBJ_MOSAIC_H": "ppu.objMosaicSizeX", "OBJ_MOSAIC_V": "ppu.objMosaicSizeY"}


def die(msg):
    print(f"gbastudy: error: {msg}", file=sys.stderr)
    sys.exit(3)


def lua_common(out, last, inputs, shots):
    """Shared Lua prologue: frame counter, scripted input and screenshots, identical timing to gbarun.py."""
    head = [f"local OUT = {gbarun.lua_str(out)}", f"local LAST = {last}",
            "local SHOTS = {" + ",".join(f"[{s}]=true" for s in shots) + "}",
            "local INPUTS = {" + ",".join("{%d,%d,{%s}}" % (f0, f1, ",".join(f"{k}=true" for k in ks))
                                          for f0, f1, ks in inputs) + "}",
            "local frame = 0"]
    return "\n".join(head) + textwrap.dedent("""
    local function write(path, data, mode)
      local f = assert(io.open(OUT .. "/" .. path, mode or "wb")); f:write(data); f:close()
    end
    local function hex16(mt, start, count)
      local t = {}
      for i = 0, count - 1 do t[#t + 1] = string.format("%04x", emu.read16(start + i * 2, mt)) end
      return table.concat(t)
    end
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
    """)


def parse_ram(spec):
    out = []
    for item in filter(None, (x.strip() for x in (spec or "").split(","))):
        name, sep, loc = item.partition("=")
        if not sep:
            die(f"bad --ram item {item!r}: expected name=LOC")
        signed = loc.endswith("s")
        mem, addr, width = gbarun.parse_loc(loc[:-1] if signed else loc)
        out.append(dict(name=name.strip(), mem=mem, addr=addr, width=width, signed=signed))
    return out


def run(lua_text, out, timeout):
    lua = out / "_study.lua"
    lua.write_text(lua_text, encoding="utf-8")
    mesen = gbarun.find_mesen()
    gbarun.ensure_bios(mesen)
    cmd = [str(mesen), "--testrunner", str(ROM), str(lua),
           f"--timeout={timeout}", "--debug.scriptWindow.allowIoOsAccess=true", "--doNotSaveSettings",
           "--video.videoFilter=None", "--gba.disableFrameSkipping=true", "--gba.skipBootScreen=true",
           "--gba.gbaAdjustColors=false", "--gba.blendFrames=false", "--gba.ramPowerOnState=AllZeros"]
    attempts = gbarun.run_mesen(cmd, out, timeout)
    if not (out / "result.json").exists():
        print("FAIL: no result.json from Mesen")
        for t in attempts:
            print(f"attempt {t['attempt']}: exit {t['exit']}\nstdout: {t['stdout']}\nstderr: {t['stderr']}")
        sys.exit(2)


def prepare_out(path):
    out = pathlib.Path(path).resolve()
    out.mkdir(parents=True, exist_ok=True)
    for f in out.glob("*"):
        if f.is_file() and f.suffix in (".png", ".bin", ".json", ".jsonl", ".csv", ".lua", ".rgb"):
            f.unlink()
    return out


# ---------------------------------------------------------------- record

def decode_oam(hexstr):
    words = [int(hexstr[i:i + 4], 16) for i in range(0, len(hexstr), 4)]
    sprites, params = [], []
    for g in range(32):  # affine parameter groups live in attr3 of 4 consecutive entries
        pa, pb, pc, pd = (words[(g * 4 + k) * 4 + 3] for k in range(4))
        params.append([v - 0x10000 if v & 0x8000 else v for v in (pa, pb, pc, pd)])
    for i in range(128):
        a0, a1, a2 = words[i * 4: i * 4 + 3]
        affine = bool(a0 & 0x100)
        if not affine and a0 & 0x200:
            continue  # disabled entry
        shape, size = a0 >> 14, a1 >> 14
        if shape == 3:
            continue  # prohibited shape: treat as unused
        w, h = SIZES[shape][size]
        double = affine and bool(a0 & 0x200)
        y, x = a0 & 0xFF, a1 & 0x1FF
        if y >= 160:
            y -= 256
        if x >= 256:
            x -= 512
        bw, bh = (w * 2, h * 2) if double else (w, h)
        if x >= W or x + bw <= 0 or y >= H or y + bh <= 0:
            continue  # off screen
        sprites.append(dict(i=i, x=x, y=y, w=w, h=h, tile=a2 & 0x3FF, pal=(a2 >> 12) & 15, prio=(a2 >> 10) & 3,
                            affine=affine, param=(a1 >> 9) & 31 if affine else None, double=double,
                            hflip=bool(a1 & 0x1000) and not affine, vflip=bool(a1 & 0x2000) and not affine,
                            mode=("normal", "blend", "window", "bad")[(a0 >> 10) & 3], bpp8=bool(a0 & 0x2000),
                            mosaic=bool(a0 & 0x1000)))
    used = sorted({s["param"] for s in sprites if s["affine"]})
    return sprites, {p: params[p] for p in used}


def parse_poke(spec):
    out = []
    for part in filter(None, (x.strip() for x in (spec or "").split(";"))):
        f, sep, rest = part.partition(":")
        loc, sep2, val = rest.rpartition("=")
        if not sep or not sep2 or not f.strip().isdigit():
            die(f"bad --poke part {part!r}: expected F:LOC=V")
        mem, addr, width = gbarun.parse_loc(loc)
        out.append((int(f), mem, addr, width, int(val, 0) & ((1 << width * 8) - 1)))
    return out


def lua_record(a, out, shots, ram):
    L = [lua_common(out, a.to, gbarun.parse_input(a.input), shots)]
    L.append("local POKES = {" + ",".join("{%d,%d,emu.memType.%s,%d,%d}" % (f, addr, gbarun.MEM[mem], w, v)
                                          for f, mem, addr, w, v in parse_poke(a.poke)) + "}")
    L.append("local FROM = %d" % a.frm)
    L.append("local IOB = {" + ",".join(f"{{{json.dumps(k)},{v}}}" for k, v in IO_BUS.items()) + "}")
    L.append("local IOS = {" + ",".join(f"{{{json.dumps(k)},{json.dumps(v)}}}" for k, v in IO_STATE.items()) + "}")
    L.append("local RAM = {" + ",".join("{%d,emu.memType.%s,%d}" % (r["addr"], gbarun.MEM[r["mem"]], r["width"])
                                        for r in ram) + "}")
    L.append(textwrap.dedent("""
    local log = assert(io.open(OUT .. "/frames.jsonl", "w"))
    local last_pal = nil
    emu.addEventCallback(function()
      if stop_code then return end
      frame = frame + 1
      if SHOTS[frame] then pending_shot = frame end
      for _, p in ipairs(POKES) do
        if p[1] == frame then for i = 0, p[4] - 1 do emu.write(p[2] + i, (p[5] >> (8 * i)) & 255, p[3]) end end
      end
      if frame >= FROM then
        local st = emu.getState()
        local io_t = {}
        for _, r in ipairs(IOB) do io_t[#io_t + 1] = string.format('%q:%d', r[1], emu.read16(r[2], emu.memType.gbaMemory)) end
        for _, r in ipairs(IOS) do
          local v = st[r[2]]
          if type(v) == "boolean" then v = v and 1 or 0 end
          io_t[#io_t + 1] = string.format('%q:%d', r[1], math.tointeger(v) or 0)
        end
        local ram_t = {}
        for _, r in ipairs(RAM) do
          local v = 0
          for i = r[3] - 1, 0, -1 do v = (v << 8) | emu.read(r[1] + i, r[2]) end
          ram_t[#ram_t + 1] = tostring(v)
        end
        local pal = hex16(emu.memType.gbaPaletteRam, 0, 512)
        local pal_s = ""
        if pal ~= last_pal then pal_s = ',"pal":"' .. pal .. '"'; last_pal = pal end
        log:write('{"f":' .. frame .. ',"oam":"' .. hex16(emu.memType.gbaSpriteRam, 0, 512) .. '","io":{' ..
                  table.concat(io_t, ",") .. '},"ram":[' .. table.concat(ram_t, ",") .. ']' .. pal_s .. '}\\n')
      end
      if frame < LAST then return end
      log:close()
      write("result.json", '{"frames":' .. frame .. ',"ok":true}', "w")
      stop_code = 0
    end, emu.eventType.endFrame)
    """))
    return "\n".join(L)


def draw_boxes(out, per_frame):
    from PIL import Image, ImageDraw
    for png in sorted(out.glob("shot_*.png")):
        f = int(png.stem.split("_")[1])
        if f not in per_frame:
            continue
        img = Image.open(png).convert("RGB")
        d = ImageDraw.Draw(img)
        for s in per_frame[f]:
            bw, bh = (s["w"] * 2, s["h"] * 2) if s["double"] else (s["w"], s["h"])
            d.rectangle([s["x"], s["y"], s["x"] + bw - 1, s["y"] + bh - 1],
                        outline=(255, 0, 255) if s["affine"] else (0, 255, 0))
        img.save(png.with_name("box_" + png.name))


def cmd_record(a):
    if a.frm < 1 or a.to < a.frm:
        die("need 1 <= --from <= --to")
    ram = parse_ram(a.ram)
    shots = list(range(a.frm, a.to + 1, a.shot_every)) if a.shot_every else []
    out = prepare_out(a.out)
    run(lua_record(a, out, shots, ram), out, a.timeout)
    gbarun.rgb_to_png(out)
    rows, io_rows, ram_rows, per_frame, pal_changes = [], [], [], {}, []
    max_vis, tiles, pals, modes = 0, set(), set(), set()
    io_first, io_changed = None, set()
    for line in (out / "frames.jsonl").read_text().splitlines():
        rec = json.loads(line)
        f = rec["f"]
        sprites, params = decode_oam(rec["oam"])
        per_frame[f] = sprites
        max_vis = max(max_vis, len(sprites))
        for s in sprites:
            tiles.add(s["tile"]); pals.add(s["pal"])
            p = params.get(s["param"]) if s["affine"] else None
            rows.append(dict(frame=f, **{k: (int(v) if isinstance(v, bool) else v) for k, v in s.items()},
                             pa=p[0] if p else "", pb=p[1] if p else "", pc=p[2] if p else "", pd=p[3] if p else ""))
        io = rec["io"]
        modes.add(io["DISPCNT"] & 7)
        io_first = io_first or io
        io_changed |= {k for k, v in io.items() if io_first.get(k) != v}
        io_rows.append(dict(frame=f, **io))
        vals = []
        for r, v in zip(ram, rec["ram"]):
            if r["signed"] and v >= 1 << (r["width"] * 8 - 1):
                v -= 1 << (r["width"] * 8)
            vals.append(v)
        ram_rows.append(dict(frame=f, **{r["name"]: v for r, v in zip(ram, vals)}))
        if "pal" in rec:
            pal_changes.append(f)
    if rows:
        with open(out / "oam.csv", "w", newline="") as fh:
            wr = csv.DictWriter(fh, fieldnames=list(rows[0]))
            wr.writeheader(); wr.writerows(rows)
    with open(out / "io.csv", "w", newline="") as fh:
        wr = csv.DictWriter(fh, fieldnames=list(io_rows[0]))
        wr.writeheader(); wr.writerows(io_rows)
    if ram:
        with open(out / "ram.csv", "w", newline="") as fh:
            wr = csv.DictWriter(fh, fieldnames=list(ram_rows[0]))
            wr.writeheader(); wr.writerows(ram_rows)
    if a.boxes:
        draw_boxes(out, per_frame)
    if shots:
        prefix = "box_" if a.boxes else "shot_"
        sheet_dir = out
        if a.boxes:  # contact_sheet globs shot_*.png: build it from the boxed copies in a sub dir
            sheet_dir = out / "_boxed"
            sheet_dir.mkdir(exist_ok=True)
            for p in sheet_dir.glob("*.png"):
                p.unlink()
            for p in out.glob(prefix + "*.png"):
                (sheet_dir / p.name[4:]).write_bytes(p.read_bytes())
        gbarun.contact_sheet(sheet_dir, out / "sheet.png", cols=a.cols)
    disp = io_first["DISPCNT"]
    summary = dict(frames=[a.frm, a.to], video_modes=sorted(modes),
                   dispcnt_first=f"0x{disp:04X}",
                   layers_on_first=[n for b, n in ((8, "BG0"), (9, "BG1"), (10, "BG2"), (11, "BG3"), (12, "OBJ"),
                                                   (13, "WIN0"), (14, "WIN1"), (15, "OBJWIN")) if disp >> b & 1],
                   obj_1d_mapping=bool(disp & 0x40), max_visible_sprites=max_vis,
                   distinct_sprite_tiles=len(tiles), sprite_palette_banks=sorted(pals),
                   io_registers_that_changed=sorted(io_changed),
                   palette_change_frames=pal_changes[:50] + (["..."] if len(pal_changes) > 50 else []))
    if ram_rows and ram:
        summary["ram_first_last"] = {r["name"]: [ram_rows[0][r["name"]], ram_rows[-1][r["name"]]] for r in ram}
    (out / "summary.json").write_text(json.dumps(summary, indent=1))
    print(json.dumps(summary, indent=1))
    print("out:", out)


# ---------------------------------------------------------------- search

SEARCH_MEM = [("ewram", 0x02000000, 0x40000), ("iwram", 0x03000000, 0x8000)]


def lua_search(a, out, snaps):
    L = [lua_common(out, snaps[-1], gbarun.parse_input(a.input), [])]
    L.append("local SNAPS = {" + ",".join(f"[{s}]=true" for s in snaps) + "}")
    L.append("local MEMS = {" + ",".join("{%s,emu.memType.%s,%d}" % (json.dumps(n), gbarun.MEM[n], size)
                                         for n, _, size in SEARCH_MEM) + "}")
    L.append(textwrap.dedent("""
    emu.addEventCallback(function()
      if stop_code then return end
      frame = frame + 1
      if SNAPS[frame] then
        for _, m in ipairs(MEMS) do
          local t = {}
          for i = 0, m[3] - 1, 2 do
            local v = emu.read16(i, m[2])
            t[#t + 1] = string.char(v & 255, v >> 8)
          end
          write(string.format("snap_%04d_%s.bin", frame, m[1]), table.concat(t))
        end
      end
      if frame < LAST then return end
      write("result.json", '{"frames":' .. frame .. ',"ok":true}', "w")
      stop_code = 0
    end, emu.eventType.endFrame)
    """))
    return "\n".join(L)


def parse_rule(r):
    r = r.strip()
    if r in ("+", "-", "=", "!"):
        return r, None
    if r.startswith("=="):
        return "==", int(r[2:], 0)
    if r[:1] in "+-" and r[1:].lstrip().isdigit():
        return "d", int(r, 0)
    die(f"bad rule {r!r}: use + - = ! +N -N ==V")


def rule_ok(rule, before, after):
    kind, n = rule
    return {"+": after > before, "-": after < before, "=": after == before, "!": after != before,
            "d": after - before == n, "==": after == n}[kind]


def nearest_symbol(addr):
    best = None
    for name, sa in gbarun.SYMS.items():
        if sa <= addr and (best is None or sa > best[1]):
            best = (name, sa)
    if best and addr - best[1] < 0x1000:
        return best[0] + (f"+0x{addr - best[1]:X}" if addr != best[1] else "")
    return ""


def cmd_search(a):
    snaps = sorted({int(x) for x in a.snap.split(",")})
    rules = [parse_rule(r) for r in a.rule.split(",")]
    if len(snaps) < 2 or len(rules) != len(snaps) - 1:
        die("need at least 2 --snap frames and exactly one --rule per interval (len(snap) - 1)")
    width = a.width // 8
    out = prepare_out(a.out)
    run(lua_search(a, out, snaps), out, a.timeout)
    found = []
    for name, base, size in SEARCH_MEM:
        bufs = [(out / f"snap_{s:04d}_{name}.bin").read_bytes() for s in snaps]
        for off in range(0, size - width + 1, width if a.aligned else 1):
            vals = [int.from_bytes(b[off:off + width], "little", signed=a.signed) for b in bufs]
            if all(rule_ok(r, vals[k], vals[k + 1]) for k, r in enumerate(rules)):
                found.append(dict(addr=f"0x{base + off:08X}", loc=f"{name}:0x{off:X}.{a.width}{'s' if a.signed else ''}",
                                  values=vals, symbol=nearest_symbol(base + off)))
    res = dict(snaps=snaps, rules=a.rule, width=a.width, signed=a.signed, count=len(found), candidates=found[:a.max])
    (out / "candidates.json").write_text(json.dumps(res, indent=1))
    print(f"{len(found)} candidate(s); showing {min(len(found), a.max)}")
    for c in found[:a.max]:
        print(f"  {c['addr']}  {c['loc']:<22} {c['values']}  {c['symbol']}")
    print("out:", out)
    if not a.keep_snaps:
        for f in out.glob("snap_*.bin"):
            f.unlink()
    sys.exit(0 if found else 1)


ROM = None


def main():
    global ROM
    ap = argparse.ArgumentParser(description="GBA study recorder / memory search (Mesen 2, headless)",
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("record", "search"):
        p = sub.add_parser(name, formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
        p.add_argument("rom")
        p.add_argument("--input")
        p.add_argument("--out", required=True, help="output dir (use the scratchpad, never the repo)")
        p.add_argument("--timeout", type=int, default=120)
        if name == "record":
            p.add_argument("--from", dest="frm", type=int, default=1)
            p.add_argument("--to", type=int, required=True)
            p.add_argument("--ram")
            p.add_argument("--poke")
            p.add_argument("--shot-every", type=int, default=0)
            p.add_argument("--boxes", action="store_true", help="draw visible sprite boxes on the shots")
            p.add_argument("--cols", type=int, default=6)
        else:
            p.add_argument("--snap", required=True)
            p.add_argument("--rule", required=True)
            p.add_argument("--width", type=int, choices=(8, 16, 32), default=16)
            p.add_argument("--signed", action="store_true")
            p.add_argument("--aligned", action="store_true", help="only addresses aligned to the width")
            p.add_argument("--max", type=int, default=40)
            p.add_argument("--keep-snaps", action="store_true")
    a = ap.parse_args()
    ROM = pathlib.Path(a.rom).resolve()
    if not ROM.is_file():
        die(f"ROM not found: {ROM}")
    repo = gbarun.ROOT.resolve()
    if repo in ROM.parents and (repo / "projects") not in ROM.parents:
        print("WARNING: commercial ROMs must stay outside the repository (docs/guias/estudiar-juegos.md)",
              file=sys.stderr)
    gbarun.load_syms(ROM)
    (cmd_record if a.cmd == "record" else cmd_search)(a)


if __name__ == "__main__":
    main()
