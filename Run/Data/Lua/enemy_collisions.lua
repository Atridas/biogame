-- start collision
function enemy_on_start_colision(_Entity, _OtherEntity)
  local ragdoll = _Entity:get_component(BaseComponent.ragdoll)
  
  if ragdoll then
    SOUND:play_sample_3D('body_fall',ragdoll:get_position())
  end
  
end