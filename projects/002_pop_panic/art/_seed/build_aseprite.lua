-- Builds an indexed .aseprite from a seed PNG strip (vertical frames).
-- aseprite -b --script-param in=<png> --script-param out=<aseprite>
--   --script-param fh=<frame height> --script-param tags="idle:1-2:300;walk:3-6:100"
--   [--script-param bg=1]   (background: keep a Background layer, no transparency)
-- Never call it on an existing .aseprite (seed_all.sh checks that first).
local p = app.params
local src = Image{ fromFile = p["in"] }
local srcSpr = app.open(p["in"])
local pal = srcSpr.palettes[1]
local w = src.width
local fh = tonumber(p.fh or src.height)
local n = src.height // fh

local spr = Sprite(w, fh, ColorMode.INDEXED)
spr:setPalette(pal)
spr.transparentColor = 0
local layer = spr.layers[1]
layer.name = (p.bg == "1") and "fondo" or "sprite"

for i = 0, n - 1 do
  local fr = (i == 0) and spr.frames[1] or spr:newEmptyFrame()
  local img = Image(w, fh, ColorMode.INDEXED)
  img:clear(0)
  img:drawImage(src, Point(0, -i * fh))
  if i == 0 then
    spr:deleteCel(layer, 1)
  end
  spr:newCel(layer, fr, img, Point(0, 0))
end

if p.tags and p.tags ~= "" then
  for t in string.gmatch(p.tags, "[^;]+") do
    local name, a, b, ms = string.match(t, "([%w_]+):(%d+)-(%d+):(%d+)")
    a = tonumber(a); b = tonumber(b)
    local tag = spr:newTag(a, b)
    tag.name = name
    for f = a, b do spr.frames[f].duration = tonumber(ms) / 1000 end
  end
end

spr:saveAs(p.out)
srcSpr:close()
spr:close()
