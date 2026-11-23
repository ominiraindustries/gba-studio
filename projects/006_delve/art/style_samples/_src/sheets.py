import os, sys
from PIL import Image
R = r"C:\Users\David\Desktop\GBA\projects\006_delve\art\style_samples"
for v in sorted(d for d in os.listdir(R) if os.path.isdir(os.path.join(R, d)) and not d.startswith("_")):
    p = os.path.join(R, v)
    tex = [Image.open(os.path.join(p, "tex_%s.png" % t)).convert("RGB") for t in ("wall", "floor", "ceiling", "door")]
    spr = [Image.open(os.path.join(p, s + ".png")).convert("RGBA") for s in ("deco_torch_front", "mon_skeleton")]
    s = Image.new("RGB", (4 * 66 + 34 + 66 + 2, 66), (10, 10, 12))
    for i, t in enumerate(tex):
        s.paste(t, (1 + i * 66, 1))
    s.paste(spr[0], (1 + 4 * 66, 1), spr[0])
    s.paste(spr[1], (1 + 4 * 66 + 34, 1), spr[1])
    s.resize((s.width * 3, s.height * 3), Image.NEAREST).save(os.path.join(p, "sheet_3x.png"))
