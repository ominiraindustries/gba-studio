#!/usr/bin/env python3
"""Reproducible, non-interactive install of the pinned GBA toolchain (no GUI, no admin rights).

    python -I tools/scripts/install_toolchain.py            # install whatever is missing
    python -I tools/scripts/install_toolchain.py --check    # only report what is installed
    python -I tools/scripts/install_toolchain.py --only devkitpro,butano

Components (all pinned; downloads are verified by SHA-256 before anything is extracted):
  devkitpro  devkitARM r68 (gcc 16.1.0, binutils 2.46.0, newlib 4.6.0) + gbafix, grit, mmutil, bin2s
             -> toolchain/devkitPro   (packages straight from pkg.devkitpro.org, no pacman needed)
  make       MSYS2 GNU make 4.4.1 (devkitARM's rules expect an MSYS make; runs on Git for Windows' msys-2.0.dll)
             -> toolchain/msys-make/bin/make.exe
  butano     Butano 21.10.1, shallow + sparse clone (butano/, common/, examples/, template/) -> engine/butano
  mgba       mGBA 0.10.5 portable, for playing -> tools/mgba   (needs 7z/7zr; 7zr.exe is fetched if missing)

Requires Python 3.14+ (tarfile reads .tar.zst natively), git and curl-free network access.
Mesen 2 (headless tests) is NOT installed here: gbarun.py reuses an existing Mesen 2.1+ (see tools/README.md).
"""
import argparse, hashlib, io, lzma, os, pathlib, shutil, subprocess, sys, tarfile, tempfile, urllib.request

ROOT = pathlib.Path(os.environ.get("GBA_INSTALL_ROOT") or pathlib.Path(__file__).resolve().parents[2])  # override: tests
DKP = "https://pkg.devkitpro.org/packages"
DEVKITPRO_PKGS = [  # (url, sha256)
    (f"{DKP}/devkitARM-r68-1-any.pkg.tar.zst",
     "7c5dd0864f7dab505fbe9cf7699100a90f15bb8563b858740064f9842db48370"),
    (f"{DKP}/windows/x86_64/devkitarm-binutils-2.46.0-1-x86_64.pkg.tar.zst",
     "d19277f5ae8d7967fdef316faa6129fde44053a3fdf2057890148398d167e966"),
    (f"{DKP}/windows/x86_64/devkitarm-gcc-16.1.0-1-x86_64.pkg.tar.zst",
     "e88fd817412cd026fa50bf92ad40db04e03aed0253bd9f3bca77e28a86db0f6b"),
    (f"{DKP}/devkitarm-newlib-4.6.0.20260123-5-any.pkg.tar.zst",
     "d314c644c143bd5b104bd82cdc27602f6449cf907cb9c37ccfb8c0707182f89d"),
    (f"{DKP}/devkitarm-crtls-1.2.6-1-any.pkg.tar.zst",
     "65faf10f62c219a1a8a3a182b0c4c56da7992599035ea32b3d79e39e79f964bb"),
    (f"{DKP}/devkitarm-rules-1.6.0-4-any.pkg.tar.zst",
     "35023b2b5f07a2dcfb600063854c712206341fb5df3ffbaf8ff825d3cbc15fcc"),
    (f"{DKP}/windows/x86_64/general-tools-1.4.4-1-x86_64.pkg.tar.zst",
     "a0fcea1a26c4f472c2ac98c409847dbec9d8e86ed82cd8b02131454f63d239f1"),
    (f"{DKP}/windows/x86_64/gba-tools-1.2.0-2-x86_64.pkg.tar.xz",
     "de5b20caf36ee9f983b0b66cb8cd82b53e2cb4d32a6a702f61960a82450df503"),
    (f"{DKP}/windows/x86_64/grit-0.10.0-1-x86_64.pkg.tar.zst",
     "f3478fd5650033d1b806b013ff18ce18e24a1ff327e62ca427007df897aa497b"),
    (f"{DKP}/windows/x86_64/mmutil-1.10.1-1-x86_64.pkg.tar.xz",
     "1132f29b46b32d791c395fb0be547a8dc9839f636df0385bd81697f2a926aa72"),
]
MSYS_MAKE = ("https://repo.msys2.org/msys/x86_64/make-4.4.1-3-x86_64.pkg.tar.zst",
             "af0bdba17f06fe037f0194069adaa31a8fe45f1a11381501896aea1fae37bd5d")
BUTANO_URL, BUTANO_TAG = "https://github.com/GValiente/butano.git", "21.10.1"
BUTANO_COMMIT = "67afc034335e2aa873091c6da20d9568984b8d0b"
BUTANO_SPARSE = ["butano", "common", "examples", "template"]
MGBA = ("https://github.com/mgba-emu/mgba/releases/download/0.10.5/mGBA-0.10.5-win64.7z",
        "b497a57c7d9093834dadc64f33a90f7c411439c21fdb8a0143255a45ea37563a")
SEVENZR = "https://www.7-zip.org/a/7zr.exe"

TC = ROOT / "toolchain"
PATHS = dict(devkitpro=TC / "devkitPro", make=TC / "msys-make", butano=ROOT / "engine" / "butano",
             mgba=ROOT / "tools" / "mgba")


def fetch(url, sha=None):
    print(f"  download {url.rsplit('/', 1)[-1]}", flush=True)
    req = urllib.request.Request(url, headers={"User-Agent": "gba-toolchain-installer"})
    with urllib.request.urlopen(req, timeout=300) as r:
        data = r.read()
    if sha and hashlib.sha256(data).hexdigest() != sha:
        sys.exit(f"SHA-256 mismatch for {url}: refusing to install")
    return data


def open_pkg(name, data):
    if name.endswith(".zst"):
        return tarfile.open(fileobj=io.BytesIO(data), mode="r:zst")
    return tarfile.open(fileobj=io.BytesIO(lzma.decompress(data)), mode="r:")


def extract_prefix(tar, prefix, dest):
    """Extract members under `prefix` (e.g. 'opt/devkitpro/') into dest, skipping pacman metadata."""
    members = []
    for m in tar.getmembers():
        if not m.name.startswith(prefix) or m.name == prefix.rstrip("/"):
            continue
        m.name = m.name[len(prefix):]
        if m.islnk() and m.linkname.startswith(prefix):  # hard links (binutils) name the full path
            m.linkname = m.linkname[len(prefix):]
        members.append(m)
    tar.extractall(dest, members=members, filter="data")


def have(name):
    p = PATHS[name]
    return {"devkitpro": p / "devkitARM/bin/arm-none-eabi-g++.exe", "make": p / "bin/make.exe",
            "butano": p / "butano/butano.mak", "mgba": p / "mGBA.exe"}[name].exists()


def install_devkitpro():
    dest = PATHS["devkitpro"]
    dest.mkdir(parents=True, exist_ok=True)
    for url, sha in DEVKITPRO_PKGS:
        name = url.rsplit("/", 1)[-1]
        with open_pkg(name, fetch(url, sha)) as tar:
            extract_prefix(tar, "opt/devkitpro/", dest)


def install_make():
    url, sha = MSYS_MAKE
    bindir = PATHS["make"] / "bin"
    bindir.mkdir(parents=True, exist_ok=True)
    with open_pkg(url, fetch(url, sha)) as tar:
        (bindir / "make.exe").write_bytes(tar.extractfile("usr/bin/make.exe").read())


def install_butano():
    dest = PATHS["butano"]
    if dest.exists():
        sys.exit(f"{dest} exists but looks incomplete: inspect it and remove it by hand")
    dest.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["git", "-c", "advice.detachedHead=false", "clone", "-q", "--depth", "1", "--branch", BUTANO_TAG,
                    "--filter=blob:none", "--sparse", BUTANO_URL, str(dest)], check=True)
    subprocess.run(["git", "-C", str(dest), "sparse-checkout", "set", *BUTANO_SPARSE], check=True)
    head = subprocess.run(["git", "-C", str(dest), "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip()
    if head != BUTANO_COMMIT:
        sys.exit(f"Butano tag {BUTANO_TAG} now points at {head}, expected {BUTANO_COMMIT}: check upstream")


def install_mgba():
    url, sha = MGBA
    with tempfile.TemporaryDirectory() as tmp:
        tmp = pathlib.Path(tmp)
        archive = tmp / "mgba.7z"
        archive.write_bytes(fetch(url, sha))
        seven = shutil.which("7z") or shutil.which("7zr")
        if not seven:
            seven = tmp / "7zr.exe"
            seven.write_bytes(fetch(SEVENZR))
        out = tmp / "x"
        subprocess.run([str(seven), "x", "-y", f"-o{out}", str(archive)], check=True, stdout=subprocess.DEVNULL)
        shutil.move(str(next(out.iterdir())), str(PATHS["mgba"]))


def report():
    for name in PATHS:
        print(f"{name:10s} {'OK ' if have(name) else 'MISSING'} {PATHS[name]}")
    if have("devkitpro"):
        gcc = PATHS["devkitpro"] / "devkitARM/bin/arm-none-eabi-gcc.exe"
        print("  ", subprocess.run([str(gcc), "--version"], capture_output=True, text=True).stdout.splitlines()[0])
    if have("butano"):
        head = subprocess.run(["git", "-C", str(PATHS["butano"]), "rev-parse", "HEAD"],
                              capture_output=True, text=True).stdout.strip()
        print(f"   butano HEAD {head} ({'pinned ' + BUTANO_TAG if head == BUTANO_COMMIT else 'NOT the pinned commit'})")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--check", action="store_true", help="report only, install nothing")
    ap.add_argument("--only", help="comma list of components: " + ",".join(PATHS))
    a = ap.parse_args()
    if a.check:
        return report()
    wanted = a.only.split(",") if a.only else list(PATHS)
    steps = dict(devkitpro=install_devkitpro, make=install_make, butano=install_butano, mgba=install_mgba)
    for name in wanted:
        if name not in steps:
            sys.exit(f"unknown component {name}")
        if have(name):
            print(f"{name}: already installed")
            continue
        print(f"{name}: installing")
        steps[name]()
    report()


if __name__ == "__main__":
    main()
