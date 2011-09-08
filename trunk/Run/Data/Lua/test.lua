function test()
  local a = EM:get_entity('tetera_Miner')
  if a then
    log(a:get_name())
    local position = a:get_component(BaseComponent.object3d):get_position()
    log('Vect3f(' .. position:length())-- .. ', ' .. position.y .. ', ' position.z .. ')')
  else
    log('a es nil')
  end
end