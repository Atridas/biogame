-- Estats del player

Player_Constants = {}

Player_Constants["Idle time"] = 300.0

--mouse
Player_Constants["Vel Threshold"] = 1000
Player_Constants["Apuntant Multiplier"] = 0.5


--velocitats
Player_Constants["Walk Speed"] = 2
Player_Constants["Run Speed"] = 5

--animacions
Player_Constants["Idle"] = 'idle'
Player_Constants["Caminar enrere"] = 'walk back'
Player_Constants["Caminar"] = 'walk'
Player_Constants["Correr"] = 'run'
Player_Constants["Apuntar"] = 'aim'
Player_Constants["Disparar"] = 'shoot'
Player_Constants["Rebre impacte"] = 'impact'
Player_Constants["Morir"] = 'dead'
Player_Constants["Escut"] = 'protection'
Player_Constants["Dance"] = 'RiggleDance'
--temps animacions
Player_Constants["Temps Tocat"] = 0.3
Player_Constants["Temps Morint"] = 0.0
Player_Constants["Temps Grenade"] = 3
--sons
Player_Constants["So rebre impacte"] = 'impacte'
Player_Constants["So disparar"] = 'disparar'
Player_Constants["So granada"] = 'granada'
Player_Constants["So force"] = 'force'

State_Player_Neutre = {}
State_Player_Dance  = {}
State_Player_Apuntar = {}
State_Player_Tocat = {}
State_Player_Morint = {}
State_Player_Mort = {}
State_Player_Escut = {}
State_Player_Force = {}
State_Player_Force_Cobertura = {}

State_Player_Cobertura_Baixa = {}
State_Player_Cobertura_Baixa_Tocat = {}

State_Player_Cobertura_Alta = {}
State_Player_Cobertura_Alta_Sortir = {}

State_Player_Cobertura_Baixa_Apuntar = {}


finite = function(_f)
  if _f ~= _f or _f == math.inf or -_f == math.inf then
    return false
  else
    return true
  end
end

-------------------------------------------------------------------------------------------------
camera_player = function(_jugador, _dt, _multiplier)
  --log('a saco 2')
  local pitch
  local yaw
  
  local object3d = _jugador:get_component(BaseComponent.object_3d)

  local vec = INPUT_MANAGER:get_mouse_delta()
  
  --if _dt > 1/60 then
  --  _dt = 1/60
  --end
  
  
  local velx = vec.x / _dt
  local vely = vec.y / _dt
  
  local vel = math.sqrt(velx*velx + vely*vely)
  
  
  
  if vel > Player_Constants["Vel Threshold"] then
    vel = 1
  else
    vel = vel / Player_Constants["Vel Threshold"]
  end
  
  if not finite(vel) then
    --log("xungo! " .. vel)
    vel = 1
  end
  --vel = 1
  
  
  pitch = object3d:get_pitch()
  yaw   = object3d:get_yaw()
  
  yaw = yaw - (vec.x * _dt) * vel * _multiplier

  pitch = pitch - (vec.y * _dt) * vel * _multiplier
  
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
  local mirilla = _jugador:get_component(BaseComponent.mirilla)
  local animation = _jugador:get_component(BaseComponent.animation)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = 0
  animation:clear_all_cycles(0.3)
  animation:play_cycle(Player_Constants["Idle"], 0.3)
  mirilla:set_active(true)
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Exit'] = function(_jugador)
  --local mirilla = _jugador:get_component(BaseComponent.mirilla)
  --mirilla:set_active(false)
  
  SOUND:stop_sample('running')
  SOUND:stop_sample('walking')
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Update'] = function(_jugador, _dt)
  
  local moviment = _jugador:get_component(BaseComponent.movement)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local camera = _jugador:get_component(BaseComponent.thps_camera)
  
  local speed = Player_Constants["Walk Speed"]
  local run_speed = Player_Constants["Run Speed"]
  
  local direction, left
  local isMoving = false
  local isBack = false
  local isRunning = false
  local isAiming = false
  local mouseSpeed = 1
  
  if ACTION_MANAGER:is_action_active('Cover') then
    player_controller.time = 0
    if player_controller:cover() then
      if player_controller.cover_entity:get_component(BaseComponent.cover):get_cover_type() == ComponentCover.cover_high then
        _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Alta')
      else
        _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa')
      end
      return
    end
  end

  if ACTION_MANAGER:is_action_active('Aim') and player_controller.shoot_active then
    player_controller.time = 0
    isAiming = true
    --camera:set_zoom(1.0,12.0)
    camera:set_fov(44.0,14.0)
    mouseSpeed = Player_Constants["Apuntant Multiplier"]
  else
    isAiming = false
    --camera:set_zoom(1.7,6.0)
    camera:set_fov(55.0,14.0)
    mouseSpeed = 1
  end
  
  if ACTION_MANAGER:is_action_active('Shield') and player_controller:is_ready_force() then
    --if _jugador:get_component(BaseComponent.shield):is_ready() then
      _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Force')
      
      return
    --end
  end
  
  if ACTION_MANAGER:is_action_active('Use') then
    player_controller.time = 0
    --log('using')
    player_controller:use()
  end
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt, mouseSpeed)
  
  if ACTION_MANAGER:is_action_active('MoveLeft') then
    player_controller.time = 0
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement + left * (_dt) * speed

    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveRight') then
    player_controller.time = 0
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement - left * (_dt) * speed

    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    player_controller.time = 0
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    
    local anim
    
    if ACTION_MANAGER:is_action_active('Run') and not isAiming then
      moviment.movement = moviment.movement + direction * (_dt) * run_speed
      isRunning = true
    else
      moviment.movement = moviment.movement + direction * (_dt) * speed
    end
    
    isMoving = true
  elseif ACTION_MANAGER:is_action_active('MoveBack') then
    player_controller.time = 0
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement - direction * (_dt) * speed
    
    isMoving = true
    isBack = true
  end
  
  local aim_angle = (pitch*1.75 + 1) * 0.5
  
  if isAiming then
    if ACTION_MANAGER:is_action_active('Shoot') then
      if player_controller:shoot() then
        SOUND:play_sample(Player_Constants["So disparar"])
        
        if isMoving then
          
        else
          --animation:play(Player_Constants["Disparar"], 0.5, 1.0, false)
          animation:play('shootUp', 0.5, aim_angle, false)
          animation:play('shootDown', 0.5, 1-aim_angle, false)
        end
      end
    end
	
	  if ACTION_MANAGER:is_action_active('Grenade') then
      if player_controller:shoot_grenade(Player_Constants["Temps Grenade"]) then
        SOUND:play_sample(Player_Constants["So granada"])
        
        if isMoving then
          
        else
          --animation:play(Player_Constants["Disparar"], 0.5, 1.0, false)
          animation:play('shootUp', 0.5, aim_angle, false)
          animation:play('shootDown', 0.5, 1-aim_angle, false)
        end
      end
    end
  end
  
  if isMoving then
    animation:clear_cycle(Player_Constants["Idle"], 0.3)
    --animation:clear_cycle('PointUpIdle',0.3)
    --animation:clear_cycle('pointDownIdle',0.3)
    
    
    --animation:clear_all_cycles(0.3)
    
    if isRunning then
      animation:stop_cycle(Player_Constants["Caminar enrere"],0.3)
      animation:stop_cycle(Player_Constants["Caminar"],0.3)
      animation:play_cycle(Player_Constants["Correr"],0.3)
      animation:clear_cycle('pointWalkUp',0.3)
      animation:clear_cycle('pointWalkDown',0.3)
      animation:clear_cycle('PointUpIdle',0.3)
      animation:clear_cycle('pointDownIdle',0.3)
      
      SOUND:play_sample('running')
      SOUND:stop_sample('walking')
    else
      SOUND:stop_sample('running')
      SOUND:play_sample('walking')
      
      if isAiming then
        animation:play_cycle('pointWalkUp',0.15,aim_angle)
        animation:play_cycle('pointWalkDown',0.15,1-aim_angle)
        animation:stop_cycle(Player_Constants["Caminar enrere"],0.15)
        animation:stop_cycle(Player_Constants["Correr"],0.15)
        animation:stop_cycle(Player_Constants["Caminar"],0.15)
      else
        animation:stop_cycle('pointWalkUp',0.3)
        animation:stop_cycle('pointWalkDown',0.3)
        
        if isBack then
          animation:play_cycle(Player_Constants["Caminar enrere"],0.3)
          animation:stop_cycle(Player_Constants["Caminar"],0.3)
          animation:stop_cycle(Player_Constants["Correr"],0.3)
        else
          animation:stop_cycle(Player_Constants["Caminar enrere"],0.3)
          animation:play_cycle(Player_Constants["Caminar"],0.3)
          animation:stop_cycle(Player_Constants["Correr"],0.3)
        end
      
      end
      
    end
  else
  
    SOUND:stop_sample('running')
    SOUND:stop_sample('walking')
    
    animation:clear_cycle(Player_Constants["Caminar"],0.3)
    animation:clear_cycle(Player_Constants["Caminar enrere"],0.3)
    animation:clear_cycle(Player_Constants["Correr"],0.3)
    animation:clear_cycle('pointWalkUp',0.3)
    animation:clear_cycle('pointWalkDown',0.3)
    
    if isAiming then
      animation:play_cycle('PointUpIdle', 0.05, aim_angle)
      animation:play_cycle('pointDownIdle', 0.05, 1 - aim_angle)
      animation:stop_cycle(Player_Constants["Idle"], 0.15)
      
    else
      animation:play_cycle(Player_Constants["Idle"], 0.3)
      animation:stop_cycle('PointUpIdle',0.3)
      animation:stop_cycle('pointDownIdle',0.3)
    end
  end
  
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > Player_Constants["Idle time"] then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Dance')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Neutre['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Dance!!!! ------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------
State_Player_Dance['Enter'] = function(_jugador)
  set_song(ComponentBGMController.rr)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_all_cycles(0.0)
  animation:play_cycle(Player_Constants["Dance"], 0.3)
end

-------------------------------------------------------------------------------------------------
State_Player_Dance['Exit'] = function(_jugador)
  set_song(ComponentBGMController.init_level)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:clear_all_cycles(0.0)
end

-------------------------------------------------------------------------------------------------
State_Player_Dance['Update'] = function(_jugador, _dt)

  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  if ACTION_MANAGER:is_action_active('Cover') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end

  if ACTION_MANAGER:is_action_active('Aim') and player_controller.shoot_active then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  if ACTION_MANAGER:is_action_active('Shield') and player_controller:is_ready_force() then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  if ACTION_MANAGER:is_action_active('Use') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt, 1.0)
  
  if ACTION_MANAGER:is_action_active('MoveLeft') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  if ACTION_MANAGER:is_action_active('MoveRight') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  elseif ACTION_MANAGER:is_action_active('MoveBack') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Dance['Receive'] = function(_jugador, _event)

  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

--[[

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
  
  animation:clear_all_cycles(0.1)
  --animation:play_cycle(Player_Constants["Apuntar"], 0.1)
	mirilla:set_active(true)
	
  --end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Exit'] = function(_jugador)
  --log('exit player apuntant')
  
  --local player_controller = _jugador:get_component(BaseComponent.player_controller)
  --local animation = _jugador:get_component(BaseComponent.animation)
  local mirilla = _jugador:get_component(BaseComponent.mirilla)
   
  --animation:clear_cycle('aim',0.3)
  mirilla:set_active(false)

end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Update'] = function(_jugador, _dt)
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  local moviment = _jugador:get_component(BaseComponent.movement)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local speed = Player_Constants["Walk Speed"]
  
  local direction, left
  local isMoving = false
  local isBack = false
  
  
  if not ACTION_MANAGER:is_action_active('Aim') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    return
  end
  
  --if ACTION_MANAGER:is_action_active('Shield') then
  --  if _jugador:get_component(BaseComponent.shield):is_ready() then
  --    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Escut')
  --    
  --    return
  --  end
  --end
  
  if ACTION_MANAGER:is_action_active('Shoot') then
    animation:play(Player_Constants["Disparar"], 0.3, 1.0, false)
    player_controller:shoot()
    SOUND:play_sample(Player_Constants["So disparar"])
  end
  
  if ACTION_MANAGER:is_action_active('Grenade') then
    animation:play(Player_Constants["Disparar"], 0.3, 1.0, false)
    player_controller:shoot_grenade(Player_Constants["Temps Grenade"])
    SOUND:play_sample(Player_Constants["So granada"])
  end
  
  if ACTION_MANAGER:is_action_active('MoveFwd') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement + direction * (_dt) * speed
    
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveBack') then
    direction = Vect3f(math.cos(yaw), 0, math.sin(yaw) )
    moviment.movement = moviment.movement - direction * (_dt) * speed
    
    
    isMoving = true
    isBack = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveLeft') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement + left * (_dt) * speed
    
    isMoving = true
  end
  
  if ACTION_MANAGER:is_action_active('MoveRight') then
    left = Vect3f(math.cos(yaw + math.pi / 2), 0, math.sin(yaw + math.pi / 2) )
    moviment.movement = moviment.movement - left * (_dt) * speed
    
    isMoving = true
  end
  
  
  --TODO: blends amb cames movent-se / cames estàtiques
  if isMoving then 
    animation:clear_cycle(Player_Constants["Apuntar"], 0.5)
    animation:play_cycle('pointWalk',0.5)
    --animation:stop_cycle('walk',0.3)
    --animation:stop_cycle('run',0.3)
  else
    animation:play_cycle(Player_Constants["Apuntar"], 0.1)
    animation:clear_cycle('pointWalk',0.1)
    --animation:clear_cycle('run',0.3)
    --animation:clear_cycle('walk back',0.3)
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Apuntar['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Tocat!!!! ------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Player_Tocat['Enter'] = function(_jugador)
  --log('enter player apuntant')
  
  local vida = _jugador:get_component(BaseComponent.vida)
  vida:set_active(false)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:set_cycle('idle', 0.3)
  animation:play(Player_Constants["Rebre impacte"], 0.3, 1.0, false)
  player_controller.time = 0
  
  SOUND:play_sample(Player_Constants["So rebre impacte"])
end

-------------------------------------------------------------------------------------------------
State_Player_Tocat['Exit'] = function(_jugador)

  local vida = _jugador:get_component(BaseComponent.vida)
  vida:set_active(true)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Tocat['Update'] = function(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = player_controller.time + _dt
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt, 1)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  if player_controller.time >= Player_Constants["Temps Tocat"] then
    --if not ACTION_MANAGER:is_action_active('Aim') then
      _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    --else
    --  _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Apuntar')
    --end
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():revert_state()
    return
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
-- Escut !!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------


State_Player_Escut['Enter'] = function(_jugador)
  --log('enter player apuntant')
  local shield = _jugador:get_component(BaseComponent.shield)
  
  if shield:activate() == false then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():revert_state()
    
    return
  end
  
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:play_cycle(Player_Constants["Escut Idle"], 0.3)
  animation:clear_all_cycles(0.1)
  animation:play_cycle(Player_Constants["Escut"], 0.1)
  
  --SOUND:play_sample(Player_Constants["So escut activat"])
end

-------------------------------------------------------------------------------------------------
State_Player_Escut['Exit'] = function(_jugador)
  local shield = _jugador:get_component(BaseComponent.shield)
  shield:deactivate()
  
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:stop_cycle(Player_Constants["Escut"], 0.1)
end

-------------------------------------------------------------------------------------------------
State_Player_Escut['Update'] = function(_jugador, _dt)
  local shield = _jugador:get_component(BaseComponent.shield)
  
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:play_cycle(Player_Constants["Escut Idle"], 0.3)
  
  if shield:is_active() == false or not ACTION_MANAGER:is_action_active('Shield') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():revert_state()
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Escut['Receive'] = function(_jugador, _event)
end

--]]
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Force !!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------


State_Player_Force['Enter'] = function(_jugador)

  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:play_cycle(Player_Constants["Escut Idle"], 0.3)
  animation:clear_all_cycles(0.1)
  animation:play_cycle(Player_Constants["Escut"], 0.1)
  player_controller.time = 0
  player_controller:force()
  SOUND:play_sample(Player_Constants["So force"])
end

-------------------------------------------------------------------------------------------------
State_Player_Force['Exit'] = function(_jugador)
  
  --local animation = _jugador:get_component(BaseComponent.animation)
  --animation:stop_cycle(Player_Constants["Escut"], 0.2)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Force['Update'] = function(_jugador, _dt)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > 0.3 then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Force['Receive'] = function(_jugador, _event)

  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Force cobertura!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------


State_Player_Force_Cobertura['Enter'] = function(_jugador)

  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  --animation:play_cycle(Player_Constants["Escut Idle"], 0.3)
  animation:clear_all_cycles(0.1)
  animation:play_cycle(Player_Constants["Escut"], 0.1)
  player_controller.time = 0
  player_controller:force()
  SOUND:play_sample(Player_Constants["So force"])
end

-------------------------------------------------------------------------------------------------
State_Player_Force_Cobertura['Exit'] = function(_jugador)
  
  --local animation = _jugador:get_component(BaseComponent.animation)
  --animation:stop_cycle(Player_Constants["Escut"], 0.2)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Force_Cobertura['Update'] = function(_jugador, _dt)
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > 0.3 then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa_Apuntar')
  end
  
end

-------------------------------------------------------------------------------------------------
State_Player_Force_Cobertura['Receive'] = function(_jugador, _event)

  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Morint['Enter'] = function(_jugador)
  --log('morint')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:play(Player_Constants["Morir"], 0.3, 1.0, true)
  player_controller.time = 0
  
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Exit'] = function(_jugador)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Update'] = function(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local vida = _jugador:get_component(BaseComponent.vida)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > Player_Constants["Temps Morint"] then
    --log('respawn')
    --vida:set(100.0)
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
    --SOUND:play_sample('pipip')
    --_jugador:get_component(BaseComponent.object_3d):set_position(Vect3f(player_controller.pos_inicial))
    --animation:stop('dead')
    --vida.immortal = false
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Mort')
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Morint['Receive'] = function(_jugador, _event)

end



-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Mort!!!! -------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Mort['Enter'] = function(_jugador)
  --log('enter player apuntant')
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = 0
  --player_controller:die()
  
  --local ragdoll = _jugador:get_component(BaseComponent.ragdoll)
  --ragdoll:activate_ragdoll()
end

-------------------------------------------------------------------------------------------------
State_Player_Mort['Exit'] = function(_jugador)
  _jugador:get_component(BaseComponent.vida):set_active(true)
  local animation = _jugador:get_component(BaseComponent.animation)
  animation:stop(Player_Constants["Morir"])
end

-------------------------------------------------------------------------------------------------
State_Player_Mort['Update'] = function(_jugador, _dt)
  camera_player(_jugador, _dt, 1)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time > 3 then
    CORE:get_level_changer():reset_level()
    --player_controller:respawn()
    --_jugador:get_component(BaseComponent.vida):set(100)
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
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
  
  moviment.movement = moviment.movement + player_controller.cover_position + player_controller.cover_normal * 0.0 - object3d:get_position()
  
  --object3d:set_yaw(-((player_controller.cover_normal):get_angle_y()) - math.pi*0.5)
  
  renderable_object:set_yaw(-((player_controller.cover_normal):get_angle_y()) + math.pi)
  
  --Amb animacio
  --animation:clear_all_cycles(0.3)
  --animation:play_cycle('CoverAvallDretaIdle', 0.3)
  --animation:play('CoverAvallDreta', 0.3, 1.0, false)
  
  --Nomes blend
  animation:clear_all_cycles(0.3)
  animation:play_cycle('CoverAvallDretaIdle', 0.3)
  
  player_controller.time = 0
  
  renderable_object.block_yaw = true
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Exit'] = function(_jugador)
  
  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  renderable_object.block_yaw = false
  
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  
  if ACTION_MANAGER:is_action_active('Cover') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end

  if ACTION_MANAGER:is_action_active('Aim') and _jugador:get_component(BaseComponent.player_controller).shoot_active then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa_Apuntar')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
  --  _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

--[[
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- State_Player_Cobertura_Baixa_Tocat -----------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Player_Cobertura_Baixa_Tocat['Enter'] = function(_jugador)

  local vida = _jugador:get_component(BaseComponent.vida)
  vida:set_active(false)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  renderable_object.block_yaw = true
  
  --animation:set_cycle('idle', 0.3)
  --animation:play('impact', 0.3, 1.0, false)
  
  player_controller.time = 0
  
  SOUND:play_sample('impacte')
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Tocat['Exit'] = function(_jugador)

  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  renderable_object.block_yaw = false
  
  local vida = _jugador:get_component(BaseComponent.vida)
  vida:set_active(true)
  
  --local animation = _jugador:get_component(BaseComponent.animation)
  --animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Tocat['Update'] = function(_jugador, _dt)
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  player_controller.time = player_controller.time + _dt
  
  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  
  if player_controller.time >= Player_Constants["Temps Tocat"] then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa')
  end
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Tocat['Receive'] = function(_jugador, _event)

end
--]]


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura Alta!!!! --------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------



-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Enter'] = function(_jugador)
  
  log('Michael Jackson YEAH!')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  local moviment = _jugador:get_component(BaseComponent.movement)
  local object3d = _jugador:get_component(BaseComponent.object_3d)
  local renderable_object = _jugador:get_component(BaseComponent.renderable_object)
  
  moviment.movement = moviment.movement + player_controller.cover_position + player_controller.cover_normal * 0.0 - object3d:get_position()
  
  object3d:set_yaw(-((player_controller.cover_normal):get_angle_y()) - math.pi*0.5)
  
  renderable_object:set_yaw(-((player_controller.cover_normal):get_angle_y()) + math.pi)
  
  --Amb animacio
  --animation:clear_all_cycles(0.3)
  --animation:play_cycle('CoverDretaIdle', 1.0)
  --animation:play('CoverDreta', 0.3, 1.0, false)
  
  
  --Nomes blend
  animation:clear_all_cycles(0.3)
  animation:play_cycle('CoverDretaIdle', 0.3)
  
  player_controller.time = 0
  renderable_object.block_yaw = true
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Exit'] = function(_jugador)

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  
  if ACTION_MANAGER:is_action_active('Cover') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Alta_Sortir')
  end

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura Alta Sortir ------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta_Sortir['Enter'] = function(_jugador)
  
  log('Michael Jackson YEAH!')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  
  --Amb animacio
  --animation:clear_all_cycles(0.3)
  --animation:play('CoverSortidaDreta',0.3,1.0,false)
  
  --Nomes blend
  animation:clear_all_cycles(0.3)
  animation:play_cycle(Player_Constants["Idle"],0.3)
  
  _jugador:get_component(BaseComponent.renderable_object).block_yaw = true
  player_controller.time = 0
  
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta_Sortir['Exit'] = function(_jugador)
  _jugador:get_component(BaseComponent.renderable_object).block_yaw = false
  
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta_Sortir['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  local player_controller = _jugador:get_component(BaseComponent.player_controller)

  player_controller.time = player_controller.time + _dt
  
  if player_controller.time >= 0.3 then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  end

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Alta_Sortir['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura_Baixa_Apuntar --------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Apuntar['Enter'] = function(_jugador)
  
  --log('Michael Jackson YEAH!')
  
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  --local mirilla = _jugador:get_component(BaseComponent.mirilla)
  local camera = _jugador:get_component(BaseComponent.thps_camera)
  
  --Nomes blend
  animation:clear_all_cycles(0.2)
  animation:play_cycle(Player_Constants["Apuntar"], 0.2)
  
  --Amb animacio
  --animation:clear_all_cycles(0.3)
  --animation:play_cycle('aim', 0.3)
  --animation:play('CoverSortidaAvallDreta', 0.3, 1.0, false)
  
	--mirilla:set_active(true)
  camera:set_fov(44.0,14.0)
  
  _jugador:get_component(BaseComponent.renderable_object).block_yaw = false
  player_controller.time = 0
  
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Apuntar['Exit'] = function(_jugador)

  local camera = _jugador:get_component(BaseComponent.thps_camera)

  --log('Michael Jackson YEAH!')
  --_jugador:get_component(BaseComponent.renderable_object).block_yaw = false
  --local mirilla = _jugador:get_component(BaseComponent.mirilla)
  
  --mirilla:set_active(false)
  camera:set_fov(55.0,14.0)
end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Apuntar['Update'] = function(_jugador, _dt)

  local pitch, yaw, object3d = camera_player(_jugador, _dt, Player_Constants["Apuntant Multiplier"])
  local player_controller = _jugador:get_component(BaseComponent.player_controller)
  local animation = _jugador:get_component(BaseComponent.animation)
  
  player_controller.time = player_controller.time + _dt
  
  if player_controller.time <= 0.15 then
    return
  end
  
  if not ACTION_MANAGER:is_action_active('Aim') then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Cobertura_Baixa')
  end   
    
  if ACTION_MANAGER:is_action_active('Shoot') then
    if player_controller:shoot() then
      animation:play(Player_Constants["Disparar"], 0.3, 1.0, false)
      SOUND:play_sample(Player_Constants["So disparar"])
    end
  end
  
  if ACTION_MANAGER:is_action_active('Grenade') then
    if player_controller:shoot_grenade(Player_Constants["Temps Grenade"]) then
      animation:play(Player_Constants["Disparar"], 0.3, 1.0, false)
      SOUND:play_sample(Player_Constants["So granada"])
    end
  end
  
  
  if ACTION_MANAGER:is_action_active('Shield') and player_controller.force_active then
    --if _jugador:get_component(BaseComponent.shield):is_ready() then
      _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Force_Cobertura')
      
      return
    --end
  end
  
  --local aim_angle = (pitch*1.75 + 1) * 0.5
  --animation:play_cycle('PointUpIdle', 0.05, aim_angle)
  --animation:play_cycle('pointDownIdle', 0.05, 1 - aim_angle)
  
  --local player_controller = _jugador:get_component(BaseComponent.player_controller)

  
  --if player_controller.time >= 0.45 then
  --  _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  --end

end

-------------------------------------------------------------------------------------------------
State_Player_Cobertura_Baixa_Apuntar['Receive'] = function(_jugador, _event)

  --if _event.msg == Event.rebre_impacte then
    --_jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Tocat')
  --else
  if _event.msg == Event.morir then
    _jugador:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Morint')
  end
  
end