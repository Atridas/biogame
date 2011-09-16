function test()
  local entity = EM:get_entity(58)
  if entity then
    log(entity:get_name())
    --[[
    local component = entity:get_component(BaseComponent.renderable_object)
    if component then
      component:set_yaw(20)
    else
      log('component no trobat')
    end
    ]]
  else
    log('entity no trobada')
  end
end