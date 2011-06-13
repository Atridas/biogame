-- Estats de l'enemic (en mode de proves)

State_Enemy_Idle = {}

State_Enemy_Caminant = {}

State_Enemy_Morint = {}

State_Enemy_Mort = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Idle["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:set_cycle('idle', 0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle["Exit"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle['Update'] = function(_enemic, _dt)

  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  
  local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq < 100 then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Caminant')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Enemy_Idle['Receive'] = function(_enemic, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Caminant!!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Caminant["Enter"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:set_cycle('walk', 0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant["Exit"] = function(_enemic)

  local animation = _enemic:get_component(BaseComponent.animation)
  animation:clear_cycle(0.3)
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant['Update'] = function(_enemic, _dt)

  local object3d = _enemic:get_component(BaseComponent.object_3d)
  local ia_brain = _enemic:get_component(BaseComponent.ia_brain)
  local moviment = _enemic:get_component(BaseComponent.movement)
  
  local ia_pos     = object3d:get_position()
  local player_pos = ia_brain.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq > 110 then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Enemy_Idle')
    return
  end

  local direction = (player_pos - ia_pos)
  
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

  moviment.movement = moviment.movement + direction * _dt * 3
  object3d:set_yaw(angle);
  
end

-------------------------------------------------------------------------------------------------
State_Enemy_Caminant['Receive'] = function(_enemic, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Morint!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Morint["Enter"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint['Update'] = function(_enemic, _dt)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Morint['Receive'] = function(_enemic, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Mort!!!! -------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Enemy_Mort["Enter"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Update'] = function(_enemic, _dt)

end

-------------------------------------------------------------------------------------------------
State_Enemy_Mort['Receive'] = function(_enemic, _event)

end
