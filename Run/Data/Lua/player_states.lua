-- Estats del player

Player_Constants = {}

Player_Constants["Walk Speed"] = 4
Player_Constants["Run Speed"] = 10
Player_Constants["Temps Tocat"] = 0.3
Player_Constants["Temps Morint"] = 2.6



State_Player_Neutre = {}
State_Player_Apuntar = {}
State_Player_Tocat = {}
State_Player_Morint = {}
State_Player_Mort = {}

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
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------
State_Player_Neutre['Enter'] = function(_jugador)
  --log('enter player neutre')
  
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
  --log('exit player neutre')
  
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
  
  local speed = Player_Constants["Walk Speed"]
  local run_speed = Player_Constants["Run Speed"]
  
  local direction, left
  local isMoving = false
  
  if ACTION_MANAGER:is_action_active('Cover') then
	log('sergi')
    player_controller:cover()
    return
  end

  if ACTION_MANAGER:is_action_active('Aim') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Apuntar')
    return
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
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    
    local anim
    
    if ACTION_MANAGER:is_action_active('Run') then
      moviment.movement = moviment.movement + direction * (_dt) * run_speed
      anim = 'run'
    else
      moviment.movement = moviment.movement + direction * (_dt) * speed
      anim = 'walk'
    end
    
    if player_controller.current_animation ~= anim then
      animation:clear_cycle(0.3)
      animation:set_cycle(anim, 0.3)
      player_controller.current_animation = anim
    end
    
    isMoving = true
  elseif ACTION_MANAGER:is_action_active('MoveBack') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement - direction * (_dt) * speed
    
    if player_controller.current_animation ~= 'walk back' then
      animation:clear_cycle(0.3)
      animation:set_cycle('walk back', 0.3)
      player_controller.current_animation = 'walk back'
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

  if _event.msg == Event.rebre_impacte then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  elseif _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Apuntar!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Enter'] = function(_jugador)
  --log('enter player apuntant')
  
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
  --log('exit player apuntant')
  
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
  local speed = Player_Constants["Walk Speed"]
  
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

  if _event.msg == Event.rebre_impacte then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  elseif _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Tocat!!!! ------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Tocat['Enter'] = function(_jugador)
  --log('enter player apuntant')
  
  local vida = _jugador:get_component(BaseComponent.vida)
  vida.immortal = true
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:set_cycle('idle', 0.3)
  animation:set_animation('impact', 0.3)
  player_controller.time = 0
  
end

-------------------------------------------------------------------------------------------------
State_Player_Tocat['Exit'] = function(_jugador)

  local vida = _jugador:get_component(BaseComponent.vida)
  vida.immortal = false
  
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Tocat['Update'] = function(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = player_controller.time + _dt
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  if player_controller.time > Player_Constants["Temps Tocat"] then
    if not ACTION_MANAGER:is_action_active('Aim') then
      _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    else
      _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Apuntar')
    end
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Tocat['Receive'] = function(_jugador, _event)

  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Payer_Morint')
  end
end



-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Morint['Enter'] = function(_jugador)
  --log('enter player apuntant')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:set_animation('dead', 0.3)
  player_controller.time = 0
  
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Exit'] = function(_jugador)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Update'] = function(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > Player_Constants["Temps Morint"] then
    log('Por què?')
    _jugador:get_component(BaseComponent.vida).vida = 100
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    --SOUND:play_sample('pipip')
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Mort')
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Receive'] = function(_jugador, _event)

end




-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Mort['Enter'] = function(_jugador)
  --log('enter player apuntant')
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = 0
  player_controller:die()
  
  --local ragdoll = _jugador:get_component(BaseComponent.ragdoll)
  --ragdoll:activate_ragdoll()
end

-------------------------------------------------------------------------------------------------
State_Player_Mort['Exit'] = function(_jugador)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Mort['Update'] = function(_jugador, _dt)
  camera_player(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > 5 then
    player_controller:respawn()
    _jugador:get_component(BaseComponent.vida).vida = 100
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end
  --local animation = _jugador:get_component(BaseComponent.animation)
  --animation:set_cycle('idle', 0.3)
end

-------------------------------------------------------------------------------------------------
State_Player_Mort['Receive'] = function(_jugador, _event)

end

