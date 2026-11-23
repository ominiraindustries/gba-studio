-- Builds an indexed .aseprite from a seed PNG strip (vertical frames). Never run it on an existing .aseprite
-- (seed_all.py checks that first).
-- aseprite -b --script-param in=<png> --script-param out=<aseprite> --script-param fh=<frame height>
--   --script-param tags="idle:1-2:400;attack:3-4:150;hit:5-5:200"  --script-param bg=0|1 --script-param ncol=N
local p = app.params
local src = Image{ fromFile = p["in"] }
local srcSpr = app.open(p["in"])
local pal = srcSpr.palettes[1]
local w = src.width
local fh = tonumber(p.fh or src.height)
local n = src.height // fh

local spr = Sprite(w, fh, ColorMode.INDEXED)
local ncol = tonumber(p.ncol or #pal)
local newPal = Palette(ncol)
for i = 0, ncol - 1 do
  if i < #pal then newPal:setColor(i, pal:getColor(i)) else newPal:setColor(i, Color{ r = 0, g = 0, b = 0 }) end
end
spr:setPalette(newPal)
spr.transparentColor = (p.bg == "1") and (ncol - 1) or 0  -- textures: index 0 is real black
local layer = spr.layers[1]
layer.name = (p.bg == "1") and "fondo" or "sprite"

for i = 0, n - 1 do
  local fr = (i == 0) and spr.frames[1] or spr:newEmptyFrame()
  local img = Image(w, fh, ColorMode.INDEXED)
  img:clear(0)
  img:drawImage(src, Point(0, -i * fh))
  if i == 0 then spr:deleteCel(layer, 1) end
  spr:newCel(layer, fr, img, Point(0, 0))
end

if p.bg == "1" then
  app.activeSprite = spr
  app.activeLayer = layer
  app.command.BackgroundFromLayer()
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
