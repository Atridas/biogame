

State_Enemy_Idle = {}

State_Enemy_Caminant = {}

State_Enemy_Disparant = {}

State_Enemy_Morint = {}

State_Enemy_Mort = {}

State_Enemy_Hit = {}

--State_Enemy_Hit_Force = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Idle["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  
  --animation:clear_all_cycles(0.0)
  --animation:play_cycle('idle', 0.3)
  animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('idle',0.3)
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq < Enemy_Constants["Distance Walk"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Caminant')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    --_enemic:get_component(BaseComponent.ia_brain):recive_shoot(_event)
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
    --if _enemic:get_component(BaseComponent.ia_brain):recive_shoot(_event) then
    --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
    --else
    --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
    --end
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Caminant!!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Caminant["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0)
  --animation:play_cycle('walk', 0.3)
  animation:set_animation_state('walk')
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant["Exit"] = function(_enemic)

  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('walk',0.3)
  --animation:clear_cycle('idle',0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant['Update'] = function(_enemic, _dt)

  local object3d = _enemic:get_component(BaseComponent.object_3d)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  local moviment = _enemic:get_component(BaseComponent.movement)
  
  local ia_pos     = object3d:get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq > Enemy_Constants["Distance Lose Walk"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
    return
  end
  
  ia_brain.time = ia_brain.time + _dt
  if ia_brain.time > Enemy_Constants["Time Btw Shoot"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Disparant')
    return
  end

  local direction = (player_pos - ia_pos)
  local angle = direction:get_angle_y() - math.pi/2;

  direction:normalize(1)
  --log('pos: ' .. direction.x .. ' ' .. direction.y .. ' ' .. direction.z)

  
  if dist_sq > Enemy_Constants["Distance Stop Walk"] then
    moviment.movement = moviment.movement + direction * _dt * Enemy_Constants["Walk Speed"]
    
    local animation = _enemic:get_component(BaseComponent.animation)
    --animation:clear_cycle('idle',0.3)
    --animation:play_cycle('walk', 0.3)
    animation:set_animation_state('walk')
  else
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Disparant')
    --local animation = _enemic:get_component(BaseComponent.animation)
    --animation:clear_cycle('walk',0.3)
    --animation:play_cycle('idle', 0.3)
    return
  end
  object3d:set_yaw(angle);
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    --_enemic:get_component(BaseComponent.ia_brain):recive_shoot(_event)
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
    --if _enemic:get_component(BaseComponent.ia_brain):recive_shoot(_event) then
    --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
    --else
    --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
    --end
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Hit!!!! --------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Hit["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0);
  --animation:play_cycle('idle', 0.3)
  --animation:play('impact', 0.3, 1.0,false)
  animation:set_animation_state('impact')
  
  local vida = _enemic:get_component(BaseComponent.vida)
  
  vida:set_active(false)
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
  
  --log('hit!')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Hit["Exit"] = function(_enemic)

  local vida = _enemic:get_component(BaseComponent.vida)
  vida:set_active(true)

  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Hit['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = ia_brain.time + _dt
  
  if ia_brain.time > Enemy_Constants["Time Hit"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
  end
end

-------------------------------------------------------------------------------------------------
State_Enemy_Hit['Receive'] = function(_enemic, _event)

  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Disparant!!!! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Disparant["Enter"] = function(_enemic)
  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0);
  --animation:play_cycle('shoot', 0.3)
  animation:set_animation_state('shoot')
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
  ia_brain.shooted = false
end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('shoot',0.3)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant['Update'] = function(_enemic, _dt)
  local object3d = _enemic:get_component(BaseComponent.object_3d) 
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  ia_brain.time = ia_brain.time + _dt
  
  local ia_pos     = object3d:get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local direction = (player_pos - ia_pos)
  local angle = direction:get_angle_y() - math.pi/2

  object3d:set_yaw(angle-0.2)
  
  if ia_brain.time > Enemy_Constants["Time Shoot"] and not ia_brain.shooted then
    ia_brain:shoot(0.25)
    
    if direction:length_sq() < 10*10  then
      SOUND:play_sample("disparar_enemic")
    end
    
    ia_brain.shoots = ia_brain.shoots + 1
    ia_brain.shooted = true
  end
  
  if ia_brain.shoots >= 4 and ia_brain.time > Enemy_Constants["Time Shooting"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
    ia_brain.shoots = 0
    return
  end
  
  if ia_brain.shoots < 4 and ia_brain.time > Enemy_Constants["Time Shooting"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Disparant')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  end
  --if _event.msg == Event.rebre_force then
  --  --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Mort!!!! -------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Mort["Enter"] = function(_enemic)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  --ia_brain:die()
end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:stop('dead')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Update'] = function(_enemic, _dt)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Receive'] = function(_enemic, _event)

end
