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
State_Player_Cobertura_Baixa = {}
State_Player_Cobertura_Alta = {}

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
    if player_controller:cover() then
      if player_controller.cover_entity:get_component(BaseComponent.cover):get_cover_type() == ComponentCover.cover_high then
        _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Alta')
      else
        _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa')
      end
      return
    end
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
  local mirilla = _jugador:get_component(BaseComponent.mirilla)
  
  if player_controller.current_animation ~= 'aim' then
    animation:clear_cycle(0.3)
    animation:set_cycle('aim', 0.3)
    player_controller.current_animation = 'aim'
	mirilla:set_active(true)
	
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Exit'] = function(_jugador)
  --log('exit player apuntant')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local mirilla = _jugador:get_component(BaseComponent.mirilla)
   
  if player_controller.current_animation ~= 'aim' then
    animation:clear_cycle(player_controller.current_animation,0.3)
  end
  animation:clear_cycle('aim',0.3)
  mirilla:set_active(false)
  
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
    SOUND:play_sample('disparar')
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
  
  SOUND:play_sample('impacte')
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
    _jugador:get_component(BaseComponent.vida).vida = 100
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    SOUND:play_sample('pipip')
    _jugador:get_component(BaseComponent.object_3d):set_position(Vect3f(player_controller.pos_inicial))
    
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


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura Baixa!!!! --------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Enter'] = function(_jugador)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local moviment = _jugador:get_component(BaseComponent.movement)
  local object3d = _jugador:get_component(BaseComponent.object_3d)
  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  
  moviment.movement = moviment.movement + player_controller.cover_position + player_controller.cover_normal * 1.0 - object3d:get_position()
  
  object3d:set_yaw(-((player_controller.cover_normal):get_angle_y()) - math.pi*0.5)
  
  --renderable_object:set_yaw(object3d:get_yaw()) VULL FER AIXO
  
  animation:set_cycle('CoverAvallDretaIdle', 1.0)
  animation:set_animation('CoverAvallDreta', 0.0)
  player_controller.time = 0
  
  renderable_object.block_yaw = true
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Exit'] = function(_jugador)

  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
  _jugador:get_component(BaseComponent.renderable_object).block_yaw = false
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt)
  
  if ACTION_MANAGER:is_action_active('Cover') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Receive'] = function(_jugador, _event)

  if _event.msg == Event.rebre_impacte then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  elseif _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura Alta!!!! --------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Enter'] = function(_jugador)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local moviment = _jugador:get_component(BaseComponent.movement)
  local object3d = _jugador:get_component(BaseComponent.object_3d)
  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  
  moviment.movement = moviment.movement + player_controller.cover_position + player_controller.cover_normal * 1.0 - object3d:get_position()
  
  object3d:set_yaw(-((player_controller.cover_normal):get_angle_y()) - math.pi*0.5)
  
  --renderable_object:set_yaw(object3d:get_yaw()) VULL FER AIXO
  
  animation:set_cycle('CoverDretaIdle', 1.0)
  animation:set_animation('CoverDreta', 0.0)
  player_controller.time = 0
  
  renderable_object.block_yaw = true
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Exit'] = function(_jugador)

  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
  _jugador:get_component(BaseComponent.renderable_object).block_yaw = false
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt)
  
  if ACTION_MANAGER:is_action_active('Cover') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Receive'] = function(_jugador, _event)

  if _event.msg == Event.rebre_impacte then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  elseif _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end


