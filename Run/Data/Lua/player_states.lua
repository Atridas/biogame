-- Estats del player

camera_player = function(_jugador, _dt)
  --log('a saco 2')
  local pitch
  local yaw
  
  local object3d = _jugador:get_component(BaseComponent.object_3d)

  local vec = INPUT_MANAGER:get_mouse_delta()
  
  pitch = object3d:get_pitch()
  yaw   = object3d:get_yaw()
  
  yaw = yaw - vec.x * _dt

  pitch = pitch - vec.y * _dt
  
  if pitch < -1 then
    pitch = -1;
  end
  if pitch > 1 then
    pitch = 1;
  end
  
  object3d:set_pitch(pitch)
  object3d:set_yaw(yaw)
  
  return pitch, yaw, object3d
end

State_Player_Neutre = {}

State_Player_Neutre['Enter'] = function(_jugador)
  log('enter player neutre')
end

State_Player_Neutre['Exit'] = function(_jugador)

end

State_Player_Neutre['Update'] = function(_jugador, _dt)
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt)
end

State_Player_Neutre['Receive'] = function(_jugador, _event)

end
