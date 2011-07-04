-- Estats de l'enemic (en mode de proves)

Enemy_Constants = {}

Enemy_Constants["Walk Speed"] = 3

Enemy_Constants["Distance Walk"] = 20 * 20 -- al quadrat!
Enemy_Constants["Distance Stop Walk"] = 5 * 5 -- "
Enemy_Constants["Distance Lose Walk"] = 30 * 30

Enemy_Constants["Time Hit"] = 0.5

Enemy_Constants["Time Dying"] = 0.5

Enemy_Constants["Time Shoot"]    = 0.7

Enemy_Constants["Time Shooting"] = 1.7

Enemy_Constants["Time Btw Shoot"] = 3

-----------------------------------------------

State_Enemy_Idle = {}

State_Enemy_Caminant = {}

State_Enemy_Disparant = {}

State_Enemy_Morint = {}

State_Enemy_Mort = {}

State_Enemy_Hit = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Idle["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_all_cycles()
  animation:play_cycle('idle', 0.3)
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle["Exit"] = function(_enemic)
  
  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_cycle('idle',0.3)
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
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Caminant!!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Caminant["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_all_cycles()
  animation:play_cycle('walk', 0.3)
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant["Exit"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_cycle('walk',0.3)
  
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
  
  dist_sq = direction:length_sq()
  
  direction:normalize(1)

  --Calculem l'angle
  local aux = direction:get_proj_zx()
  aux:normalize(1)
  local cosinus = aux * Vect2f(0,1)
  local y       = (direction ^ Vect3f(1,0,0)).y
  
  local angle = math.acos(cosinus);
  if y < 0 then
    angle = -angle
  end


  --log('pos: ' .. direction.x .. ' ' .. direction.y .. ' ' .. direction.z)

  
  --if dist_sq > Enemy_Constants["Distance Stop Walk"] then
    moviment.movement = moviment.movement + direction * _dt * Enemy_Constants["Walk Speed"]
    
    --local animation = _enemic:get_component(BaseComponent.animation)
    --animation:clear_cycle(0.3)
    --animation:set_cycle('walk', 0.3)
  --else
    
    --local animation = _enemic:get_component(BaseComponent.animation)
    --animation:clear_cycle(0.3)
    --animation:set_cycle('idle', 0.3)
  --end
  object3d:set_yaw(angle);
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Hit!!!! --------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Hit["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_all_cycles();
  animation:play_cycle('idle', 0.3)
  animation:play('impact', 0.3, 1.0,false)
  
  local vida = _enemic:get_component(BaseComponent.vida)
  
  vida.immortal = true
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
  
  --log('hit!')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Hit["Exit"] = function(_enemic)

  local vida = _enemic:get_component(BaseComponent.vida)
  vida.immortal = false

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
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Morint["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:play('dead', 0.3,1.0,true)
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint['Update'] = function(_enemic, _dt)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = ia_brain.time + _dt
  
  if ia_brain.time > Enemy_Constants["Time Dying"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Mort')
  end
end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint['Receive'] = function(_enemic, _event)
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Disparant!!!! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Disparant["Enter"] = function(_enemic)
  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_all_cycles();
  animation:play_cycle('shoot', 0.3)
  
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = 0
  ia_brain.shooted = false
end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant["Exit"] = function(_enemic)
  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_cycle('shoot',0.3)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant['Update'] = function(_enemic, _dt)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain.time = ia_brain.time + _dt
  
  if ia_brain.time > Enemy_Constants["Time Shoot"] and not ia_brain.shooted then
    ia_brain:shoot()
    ia_brain.shooted = true
  end
  
  if ia_brain.time > Enemy_Constants["Time Shooting"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
  end
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Disparant['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Hit')
  end
  if _event.msg == Event.morir then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Morint')
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Mort!!!! -------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Mort["Enter"] = function(_enemic)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  ia_brain:die()
end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort["Exit"] = function(_enemic)
  local animation = _enemic:get_component(BaseComponent.animation)
  animation:stop('dead')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Update'] = function(_enemic, _dt)
  --_enemic:get_component(BaseComponent.vida).vida = 100
  --_enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Receive'] = function(_enemic, _event)

end
