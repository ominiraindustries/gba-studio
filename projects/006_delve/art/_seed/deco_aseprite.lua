-- Builds an indexed deco_*.aseprite with two layers ("sombra" = shadow on the wall, "objeto" = the deco) from two
-- seed PNG strips (vertical frames, same palette). Never run it on an existing .aseprite (deco_seed.py checks).
-- aseprite -b --script-param shadow=<png> --script-param object=<png> --script-param out=<aseprite>
--   --script-param fh=64 --script-param tags="flame:1-4" --script-param durs="110,70,140,90"
local p = app.params
local srcSpr = app.open(p.object)
local pal = srcSpr.palettes[1]
local obj = Image{ fromFile = p.object }
local sha = (p.shadow and p.shadow ~= "") and Image{ fromFile = p.shadow } or nil
local w = obj.width
local fh = tonumber(p.fh)
local n = obj.height // fh

local spr = Sprite(w, fh, ColorMode.INDEXED)
local newPal = Palette(16)
for i = 0, 15 do
  if i < #pal then newPal:setColor(i, pal:getColor(i)) else newPal:setColor(i, Color{ r = 0, g = 0, b = 0 }) end
end
spr:setPalette(newPal)
spr.transparentColor = 0

local layers = {}
if sha then
  layers[#layers + 1] = { spr.layers[1], sha }
  spr.layers[1].name = "sombra"
  local top = spr:newLayer()
  top.name = "objeto"
  layers[#layers + 1] = { top, obj }
else
  spr.layers[1].name = "objeto"
  layers[1] = { spr.layers[1], obj }
end

for i = 2, n do spr:newEmptyFrame() end
for _, pair in ipairs(layers) do
  local layer, src = pair[1], pair[2]
  for i = 0, n - 1 do
    local img = Image(w, fh, ColorMode.INDEXED)
    img:clear(0)
    img:drawImage(src, Point(0, -i * fh))
    if layer:cel(i + 1) then spr:deleteCel(layer, i + 1) end
    spr:newCel(layer, spr.frames[i + 1], img, Point(0, 0))
  end
end

local durs = {}
if p.durs and p.durs ~= "" then
  for d in string.gmatch(p.durs, "[^,]+") do durs[#durs + 1] = tonumber(d) end
end
for i = 1, n do
  if durs[i] then spr.frames[i].duration = durs[i] / 1000 end
end

if p.tags and p.tags ~= "" then
  for t in string.gmatch(p.tags, "[^;]+") do
    local name, a, b = string.match(t, "([%w_]+):(%d+)-(%d+)")
    local tag = spr:newTag(tonumber(a), tonumber(b))
    tag.name = name
  end
end

spr:saveAs(p.out)
srcSpr:close()
spr:close()
