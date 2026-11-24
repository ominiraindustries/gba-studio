"""Download the PixelLab sources used to seed the 007_survivors art into art/raw/ (PNG, untouched).

Usage: python -I fetch_raw.py [name ...]   (no names = everything)
Raw files are only a seed: the editable source of truth is art/*.aseprite.
"""
import io
import json
import os
import sys
import time
import urllib.request
import zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
RAW = os.path.join(os.path.dirname(HERE), "raw")
API = "https://api.pixellab.ai/mcp"
OBJ = "https://backblaze.pixellab.ai/file/pixellab-characters/objects/6638c9f7-4cf6-4191-aef7-21d5135f7e17"

CHARACTERS = {
    "wizard": "89c9725a-b6d6-4f15-b9d3-73beef656b36",
    "skeleton_big": "2bb58cad-360c-4731-a6c4-b5b54a47f469",
    "skeleton": "b7358716-3d2d-4346-bd40-226cb6557cc0",
    "skeleton_v1": "76b118d5-829e-4cfa-a5a8-891dade59de6",  # first try, too dark at 1x
    "orc": "10556b92-1c6a-49a3-81fe-03e3ceec81af",
    "orc_warchief": "cecf4a98-4663-4b51-8850-6b5251b0ef74",
    "goblin": "d5a27dd2-73aa-43ae-8569-b93fb057f7d7",
    "bat": "e7a204a6-0e2a-4194-ba8c-c90155224f63",
    "slime": "2573c735-5537-4422-8514-2b3065d53617",
    "slime_king": "14119346-7937-4f10-bd3f-cd07aa5363b0",
}
# Review packs: candidate frames downloadable by index.
PACKS = {
    "props64": ("1a5ae4fe-23b1-4052-b0e3-168bb39e4bb8", 16),
    "props32": ("d0540c55-1e67-4533-ab60-995e2e5e5c61", 64),
    "items16": ("cc2030f3-6303-48c1-b4a6-66e08fb7c30a", 64),
}
OBJECTS = {"shrine": "f8f167df-d6e7-4b22-bb4f-e27a8ee06b05"}
TILESETS = {"meadow": "2819c4cb-a420-4af0-a10d-0a4dfc542db8"}


def get(url, tries=6):
    for i in range(tries):
        try:
            req = urllib.request.Request(url, headers={"User-Agent": "gba-art-fetch/1.0"})
            with urllib.request.urlopen(req, timeout=60) as r:
                return r.read()
        except Exception as e:  # 423 while generating, 503 busy
            if i == tries - 1:
                raise
            print("  retry", url, e)
            time.sleep(10)


def save(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)


def main(names):
    want = lambda n: not names or n in names
    for name, cid in CHARACTERS.items():
        if want(name):
            z = zipfile.ZipFile(io.BytesIO(get(f"{API}/characters/{cid}/download")))
            z.extractall(os.path.join(RAW, name))
            print(name, len(z.namelist()), "files")
    for name, (oid, n) in PACKS.items():
        if want(name):
            for i in range(n):
                save(os.path.join(RAW, name, f"{i:02d}.png"), get(f"{OBJ}/{oid}/rotations/frame_{i}.png"))
            print(name, n, "candidates")
    for name, oid in OBJECTS.items():
        if want(name):
            data = get(f"{API}/objects/{oid}/download")
            if data[:2] == b"PK":
                zipfile.ZipFile(io.BytesIO(data)).extractall(os.path.join(RAW, name))
            else:
                save(os.path.join(RAW, name, "south.png"), data)
            print(name, "ok")
    for name, tid in TILESETS.items():
        if want(name):
            save(os.path.join(RAW, name, "sheet.png"), get(f"{API}/tilesets/{tid}/image?inline=true"))
            save(os.path.join(RAW, name, "metadata.json"), get(f"{API}/tilesets/{tid}/metadata"))
            print(name, "ok")


if __name__ == "__main__":
    main(sys.argv[1:])
