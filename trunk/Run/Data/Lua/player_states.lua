-- Estats del player

-------------------------------------------------------------------------------------------------
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

-------------------------------------------------------------------------------------------------
State_Player_Neutre = {}

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Enter'] = function(_jugador)
  log('enter player neutre')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  
  if player_controller.current_animation ~= 'idle' then
    animation:clear_cycle(0.3)
    animation:set_cycle('idle', 0.3)
    player_controller.current_animation = 'idle'
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Exit'] = function(_jugador)
  log('exit player neutre')
  
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.current_animation = ''
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Update'] = function(_jugador, _dt)
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt)
  local moviment = _jugador:get_component(BaseComponent.movement)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  local speed = 5
  
  local direction, left
  local isMoving = false
  
  if ACTION_MANAGER:is_action_active('Aim') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Apuntar')
    return
  end
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement + direction * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk' then
      animation:clear_cycle(0.3)
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveBack') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement - direction * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk back' then
      animation:clear_cycle(0.3)
      animation:set_cycle('walk back', 0.3)
      player_controller.current_animation = 'walk back'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveLeft') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement + left * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk' then
      animation:clear_cycle(0.3)
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveRight') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement - left * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk' then
      animation:clear_cycle(0.3)
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  if not isMoving then
    if player_controller.current_animation ~= 'idle' then
      animation:clear_cycle(0.3)
      animation:set_cycle('idle', 0.3)
      player_controller.current_animation = 'idle'
    end
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Receive'] = function(_jugador, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Apuntar!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Player_Apuntar = {}

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Enter'] = function(_jugador)
  log('enter player apuntant')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  if player_controller.current_animation ~= 'aim' then
    animation:clear_cycle(0.3)
    animation:set_cycle('aim', 0.3)
    player_controller.current_animation = 'aim'
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Exit'] = function(_jugador)
  log('exit player apuntant')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  
  
  if player_controller.current_animation ~= 'aim' then
    animation:clear_cycle(player_controller.current_animation,0.3)
  end
  animation:clear_cycle('aim',0.3)
  
  player_controller.current_animation = ''
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Update'] = function(_jugador, _dt)
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt)
  local moviment = _jugador:get_component(BaseComponent.movement)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  if not ACTION_MANAGER:is_action_active('Aim') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  if ACTION_MANAGER:is_action_active('Shoot') then
    player_controller:shoot()
  end
  
  local animation = _jugador:get_component(BaseComponent.animation)
  local speed = 5
  
  local direction, left
  local isMoving = false
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement + direction * (_dt) * speed
    
    
    if player_controller.current_animation ~= 'walk' then
      if player_controller.current_animation ~= 'aim' then
        animation:clear_cycle(player_controller.current_animation,0.3)
      end
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveBack') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement - direction * (_dt) * speed
    
    
    if player_controller.current_animation ~= 'walk back' then
      if player_controller.current_animation ~= 'aim' then
        animation:clear_cycle(player_controller.current_animation,0.3)
      end
      animation:set_cycle('walk back', 0.3)
      player_controller.current_animation = 'walk back'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveLeft') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement + left * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk' then
      if player_controller.current_animation ~= 'aim' then
        animation:clear_cycle(player_controller.current_animation,0.3)
      end
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveRight') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement - left * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk' then
      if player_controller.current_animation ~= 'aim' then
        animation:clear_cycle(player_controller.current_animation,0.3)
      end
      animation:set_cycle('walk', 0.3)
      player_controller.current_animation = 'walk'
    end
    isMoving = true
  end
  
  
  if not isMoving then
    if player_controller.current_animation ~= 'aim' then
      animation:clear_cycle(player_controller.current_animation,0.3)
      --animation:set_cycle('aim', 0.3)
      player_controller.current_animation = 'aim'
    end
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Receive'] = function(_jugador, _event)

end

