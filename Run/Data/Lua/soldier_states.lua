

-----------------------------------------------

State_Soldier_Idle = {}

State_Soldier_Buscant_Cobertura = {}

State_Soldier_Disparant = {}

State_Soldier_Cobrint = {}

State_Soldier_Cobrint_Disparant = {}

State_Soldier_Morint = {}

State_Soldier_Mort = {}

State_Soldier_Hit = {}

State_Soldier_Hit_Cobertura = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Idle["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  
  --animation:clear_all_cycles(0.0)
  --animation:play_cycle('idle', 0.3)
  animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Soldier_Idle["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('idle',0.3)
end

-------------------------------------------------------------------------------------------------
State_Soldier_Idle['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq < Enemy_Constants["Distance Walk"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Buscant_Cobertura')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Soldier_Idle['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Buscant Cobertura!!!! ------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Buscant_Cobertura["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0)
  --animation:play_cycle('run', 0.3)
  animation:set_animation_state('run')
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  --En cas de no trobar cami, es comporta com un enemic standard.
  if ia_brain:plan_path_to_cobertura(Enemy_Constants["Max Node Distance"]) == false then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
  end
  ia_brain.time = 0
end

-------------------------------------------------------------------------------------------------
State_Soldier_Buscant_Cobertura["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('run',0.3)
end

-------------------------------------------------------------------------------------------------
State_Soldier_Buscant_Cobertura['Update'] = function(_enemic, _dt)

  local object3d = _enemic:get_component(BaseComponent.object_3d)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  local moviment = _enemic:get_component(BaseComponent.movement)
  
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local ia_pos     = object3d:get_position()
  
  --moviment per nodes fins arribar a cobertura
  if ia_brain:arrived_at_node(Enemy_Constants["Node Distance"]) then
    if ia_brain:arrived_at_destination() then
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Cobrint')
      return
    else
      ia_brain:set_next_node()
    end
  end
  
  local node_pos = ia_brain:get_next_node_position()
  
  local dist_sq_player = (ia_pos - node_pos):length_sq()
  
  --TEMPORAL
  --if dist_sq_player > Enemy_Constants["Distance Lose Walk"] then
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Idle')
  --  return
  --end
  
  ia_brain.time = ia_brain.time + _dt
  if ia_brain.time > Enemy_Constants["Time Btw Shoot"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Disparant')
    return
  end

  --moviment
  local direction = (node_pos - ia_pos)
  
  dist_sq = direction:length_sq()
  
  direction:normalize(1)

  local angle = direction:get_angle_y() - math.pi/2
  
  moviment.movement = moviment.movement + direction * _dt * Enemy_Constants["Walk Speed"]

  object3d:set_yaw(angle);
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Buscant_Cobertura['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Cobertura!!!! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Cobrint["Enter"] = function(_enemic)

  --animacio
  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0)
  --animation:play('CoverDownLeft', 0.3, 1.0, false)
  --animation:play_cycle('CoverDownLeftIdle', 0.3)
  animation:set_animation_state('cover down')
  
  --temps de dispar
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = Enemy_Constants["Time Btw Shoot"] * math.random()
  if ia_brain.time < 1.5 then
    ia_brain.time = 1.5
  end
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('CoverDownLeftIdle',0.3)
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)  
  ia_brain.time = ia_brain.time - _dt
  
  if ia_brain.time <= 0 then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Cobrint_Disparant')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit_Cobertura')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Disparant!!!! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Disparant["Enter"] = function(_enemic)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  ia_brain.time = 0
  ia_brain.shooted = false
  
  
  --mirem al personatge per apuntar-lo
  local object3d = _enemic:get_component(BaseComponent.object_3d)
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local direction = (player_pos - object3d:get_position())
  local angle = direction:get_angle_y() - math.pi/2
  
  --face char
  object3d:set_yaw(angle);
  
  --disparem
  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0);
  --animation:play_cycle('shoot', 0.3)
  animation:set_animation_state('shoot')
end

-------------------------------------------------------------------------------------------------
State_Soldier_Disparant["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('shoot',0.3)
end

-------------------------------------------------------------------------------------------------
State_Soldier_Disparant['Update'] = function(_enemic, _dt)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  local object3d = _enemic:get_component(BaseComponent.object_3d) 
  
  ia_brain.time = ia_brain.time + _dt
  
  local ia_pos     = object3d:get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local direction = (player_pos - ia_pos)
  
  if ia_brain.time > Enemy_Constants["Time Shoot"] and not ia_brain.shooted then
    ia_brain:shoot(0.05)
    
    if direction:length_sq() < 10*10  then
      SOUND:play_sample("disparar_enemic")
    end
    
    ia_brain.shooted = true
  end
  
  if ia_brain.time > Enemy_Constants["Time Shooting"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Buscant_Cobertura')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Disparant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Disparant dsede Cobertura!!!! ----------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Cobrint_Disparant["Enter"] = function(_enemic)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  ia_brain.time = 0
  ia_brain.shooted = false
  
  
  --mirem al personatge per apuntar-lo
  local object3d = _enemic:get_component(BaseComponent.object_3d)
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local direction = (player_pos - object3d:get_position())
  local angle = direction:get_angle_y() - math.pi/2
  
  --face char
  object3d:set_yaw(angle);
  
  --disparem
  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:play_cycle('shoot', 0.3)
  animation:set_animation_state('shoot')
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint_Disparant["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('shoot',0.3)
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint_Disparant['Update'] = function(_enemic, _dt)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  local object3d = _enemic:get_component(BaseComponent.object_3d) 
  
  ia_brain.time = ia_brain.time + _dt
  
  local ia_pos     = object3d:get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local direction = (player_pos - ia_pos)
  
  if ia_brain.time > Enemy_Constants["Time Shoot Cover"] and not ia_brain.shooted then
    ia_brain:shoot(0.05)
    
    if direction:length_sq() < 10*10  then
      SOUND:play_sample("disparar_enemic")
    end
    
    ia_brain.shoots = ia_brain.shoots + 1
    ia_brain.shooted = true
  end
  
  if ia_brain.shoots >= 3 and ia_brain.time > Enemy_Constants["Time Shooting Cover"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Cobrint')
    ia_brain.shoots = 0
    return
  end
  
  if ia_brain.shoots < 3 and ia_brain.time > Enemy_Constants["Time Shooting Cover"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Cobrint_Disparant')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Cobrint_Disparant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit_Cobertura')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Hit!!!! --------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Hit["Enter"] = function(_enemic)

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
State_Soldier_Hit["Exit"] = function(_enemic)

  local vida = _enemic:get_component(BaseComponent.vida)
  vida:set_active(true)

  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('idle', 0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Hit['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = ia_brain.time + _dt
  
  if ia_brain.time > Enemy_Constants["Time Hit"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Buscant_Cobertura')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Soldier_Hit['Receive'] = function(_enemic, _event)

  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
    return
  end
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Hit Cobertura!!!! ----------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Hit_Cobertura["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_all_cycles(0.0);
  --animation:play_cycle('idle', 0.3)
  --animation:play('impact', 0.3, 1.0,false)
  animation:set_animation_state('impact')
  
  local vida = _enemic:get_component(BaseComponent.vida)
  
  vida:set_active(false)
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
end

-------------------------------------------------------------------------------------------------
State_Soldier_Hit_Cobertura["Exit"] = function(_enemic)

  local vida = _enemic:get_component(BaseComponent.vida)
  vida:set_active(true)

  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:clear_cycle('idle', 0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Soldier_Hit_Cobertura['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = ia_brain.time + _dt
  
  if ia_brain.time > Enemy_Constants["Time Hit"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Cobrint')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Soldier_Hit_Cobertura['Receive'] = function(_enemic, _event)

  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
    return
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

--State_Soldier_Morint["Enter"] = function(_enemic)
--
--  local animation = _enemic:get_component(BaseComponent.animation)
--  animation:clear_all_cycles(0.0)
--  animation:play('dead', 0.3, 1.0, true)
--  
--  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
--  ia_brain.time = 0
--end
--
---------------------------------------------------------------------------------------------------
--State_Soldier_Morint["Exit"] = function(_enemic)
--
--end
--
---------------------------------------------------------------------------------------------------
--State_Soldier_Morint['Update'] = function(_enemic, _dt)
--  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
--  ia_brain.time = ia_brain.time + _dt
--  
--  if ia_brain.time > Enemy_Constants["Time Dying"] then
--    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
--  end
--end
--
---------------------------------------------------------------------------------------------------
--State_Soldier_Morint['Receive'] = function(_enemic, _event)
--  if _event.msg == Event.rebre_force then
--    --g_force_event = _event
--    _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
--    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
--  end
--end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Mort!!!! -------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Soldier_Mort["Enter"] = function(_enemic)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  --ia_brain:die()
end

-------------------------------------------------------------------------------------------------
State_Soldier_Mort["Exit"] = function(_enemic)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:stop('dead')
end

-------------------------------------------------------------------------------------------------
State_Soldier_Mort['Update'] = function(_enemic, _dt)
  --_enemic:get_component(BaseComponent.vida).set(100)
  --_enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Idle')
end

-------------------------------------------------------------------------------------------------
State_Soldier_Mort['Receive'] = function(_enemic, _event)
  --if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  --end
end
