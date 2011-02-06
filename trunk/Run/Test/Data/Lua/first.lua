log("LUA:: Init test script ")

for i = 1, 10 do
  local l_vPos = Vect3f(math.random(-15,15), 0.0, math.random(-15,15))
  local l_pStaticModel = get_core():get_renderable_objects_manager():add_static("braç","bracet_"..i)
  l_pStaticModel:set_position(l_vPos)
end

--[[
local pos = vect3f 0 0 0

get_core():get_light_manager():get_resource("Spot01"):set_position(pos)

]]