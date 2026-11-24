# Download PixelLab review candidates and build labelled contact sheets.
import sys, os, urllib.request
from PIL import Image, ImageDraw
BASE = "https://backblaze.pixellab.ai/file/pixellab-characters/objects/6638c9f7-4cf6-4191-aef7-21d5135f7e17/{oid}/rotations/frame_{i}.png"
RAW = os.path.join(os.path.dirname(__file__), "..", "..", "raw", "style_samples")
def fetch(name, oid, n, out_sheet):
    d = os.path.join(RAW, name); os.makedirs(d, exist_ok=True)
    ims = []
    for i in range(n):
        p = os.path.join(d, f"{i:02d}.png")
        if not os.path.exists(p):
            req = urllib.request.Request(BASE.format(oid=oid, i=i), headers={"User-Agent": "Mozilla/5.0"})
            open(p, "wb").write(urllib.request.urlopen(req).read())
        ims.append(Image.open(p).convert("RGBA"))
    w, h = ims[0].size; cols = 8; rows = (n + cols - 1) // cols; s = 3; pad = 10
    sheet = Image.new("RGBA", (cols * (w * s + 4), rows * (h * s + pad + 4)), (90, 90, 100, 255))
    dr = ImageDraw.Draw(sheet)
    for i, im in enumerate(ims):
        x = (i % cols) * (w * s + 4); y = (i // cols) * (h * s + pad + 4)
        sheet.paste(im.resize((w * s, h * s), Image.NEAREST), (x, y + pad), im.resize((w * s, h * s), Image.NEAREST))
        dr.text((x + 1, y), str(i), fill=(255, 255, 0, 255))
    sheet.save(out_sheet)
if __name__ == "__main__":
    fetch(sys.argv[1], sys.argv[2], int(sys.argv[3]), sys.argv[4])
