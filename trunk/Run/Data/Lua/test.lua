function test()
  --local entity = EM:get_entity("lvl1_elice")
  if entity then
    --local l_o3d = entity:get_component(BaseComponent.object_3d)
    --if l_o3d then
    EM:init_pickup("lvl1_key_green", "lvl1_key_green", Vect3f(0.0,1.5,0.0), "get_key_green")
    --end
  else
    log('entity no trobada')
  end
  
  EM:init_pickup("lvl1_key_green", "lvl1_key_green", Vect3f(0.0,1.5,0.0), "get_key_green")
  
end