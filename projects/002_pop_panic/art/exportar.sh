#!/bin/sh
# Export art/*.aseprite -> graphics/<name>.bmp (vertical strip) + .json for Butano.
# Usage (Git Bash, from anywhere): sh projects/002_pop_panic/art/exportar.sh [name ...]
#   --seed : first create missing .aseprite files from art/_seed (never overwrites).
set -e
ART="$(cd "$(dirname "$0")" && pwd)"
GFX="${GFX:-$ART/../graphics}"
ASE="${ASEPRITE:-/c/Program Files (x86)/Steam/steamapps/common/Aseprite/Aseprite.exe}"

# name|frame height|tags (name:from-to:ms, 1-based)|json
ITEMS='player|16|idle:1-2:400;walk:3-6:110;shoot:7-7:150;hurt:8-9:300|{"type":"sprite","height":16,"bpp_mode":"bpp_4"}
ball_24|32||{"type":"sprite","bpp_mode":"bpp_4"}
ball_32|32||{"type":"sprite","bpp_mode":"bpp_4"}
ball_16|16||{"type":"sprite","bpp_mode":"bpp_4"}
ball_8|8||{"type":"sprite","bpp_mode":"bpp_4"}
harpoon|16|tip:1-1:100;chain:2-2:100|{"type":"sprite","height":16,"bpp_mode":"bpp_4"}
pop_fx|32|pop:1-4:50|{"type":"sprite","height":32,"bpp_mode":"bpp_4"}
item_double|16||{"type":"sprite","bpp_mode":"bpp_4"}
item_shield|16||{"type":"sprite","bpp_mode":"bpp_4"}
pigeon|16|fly:1-3:100|{"type":"sprite","height":16,"bpp_mode":"bpp_4"}
bg_madrid|256||{"type":"regular_bg","bpp_mode":"bpp_4_manual"}
bg_paris|256||{"type":"regular_bg","bpp_mode":"bpp_4_manual"}
bg_london|256||{"type":"regular_bg","bpp_mode":"bpp_4_manual"}'

SEED=0
if [ "$1" = "--seed" ]; then SEED=1; shift; fi
ONLY="$*"

echo "$ITEMS" | while IFS='|' read -r name fh tags json; do
  if [ -n "$ONLY" ] && ! echo " $ONLY " | grep -q " $name "; then continue; fi
  src="$ART/$name.aseprite"
  if [ ! -f "$src" ] && [ "$SEED" = 1 ]; then
    png="$ART/_seed/out/$name.png"
    bg=0; case "$name" in bg_*) bg=1;; esac
    "$ASE" -b --script-param in="$png" --script-param out="$src" --script-param fh="$fh" \
      --script-param tags="$tags" --script-param bg="$bg" --script "$ART/_seed/build_aseprite.lua"
    echo "seeded $name.aseprite"
  fi
  [ -f "$src" ] || { echo "missing $src"; continue; }
  "$ASE" -b "$src" --sheet-type vertical --sheet "$GFX/$name.bmp" >/dev/null
  case "$name" in
    bg_*)  # only the used 16-colour banks: colors_count = (highest index / 16 + 1) * 16
      cc=$(python -I -c "import sys; from PIL import Image; print((max(Image.open(sys.argv[1]).getdata()) // 16 + 1) * 16)" "$GFX/$name.bmp" 2>/dev/null)
      json=$(printf '%s' "$json" | sed "s/}\$/,\"colors_count\":$cc}/");;
  esac
  printf '%s\n' "$json" > "$GFX/$name.json"
  echo "exported $name"
done
