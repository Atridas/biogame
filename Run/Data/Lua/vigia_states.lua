Vigia_Constants = {}

Vigia_Constants["Time Patrol New Node"] = 4
Vigia_Constants["Time Atac To Shoot"]    = 0.8
Vigia_Constants["Time Search Node"]      = 0.3
Vigia_Constants["Time Shoots"]           = 0.3
Vigia_Constants["Time Rebre Impacte"]    = 0.5
Vigia_Constants["Num Shoots"]            = 3
Vigia_Constants["Distancia Atac"] = 8 * 8


State_Vigia_Patrol        = {}
State_Vigia_Atac          = {}
State_Vigia_Shoot         = {}
State_Vigia_Search_Node   = {}
State_Vigia_Rebre_Impacte = {}
State_Vigia_Mort          = {}

State_Vigia_Idle = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Patroll!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Patrol["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia:fly(true)
  
  ia_brain_vigia.time = 0
  
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Vigia_Patrol["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Vigia_Patrol['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > Vigia_Constants["Time Patrol New Node"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Search_Node')
    return
  end
  
  
  local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  local player_pos = ia_brain_vigia.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  if dist_sq < Vigia_Constants["Distancia Atac"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Atac')
    return
  end
  
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Patrol['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Rebre_Impacte')
    return
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Atac!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Atac["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = 0
  
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Vigia_Atac["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Vigia_Atac['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > Vigia_Constants["Time Atac To Shoot"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Shoot')
    return
  end
  
  
  local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  local player_pos = ia_brain_vigia.player:get_component(BaseComponent.object_3d):get_position()
  
  local dist_sq = (ia_pos - player_pos):length_sq()
  
  ia_brain_vigia.patrol_direction = player_pos - ia_pos
  
  if dist_sq > Vigia_Constants["Distancia Atac"] then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Patrol')
    return
  end
  
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Atac['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Rebre_Impacte')
    return
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Shoot!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Shoot["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = 0
  ia_brain_vigia.shoots = 0
  
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Vigia_Shoot["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Vigia_Shoot['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > Vigia_Constants["Time Shoots"] then

    ia_brain_vigia:shoot(0)
    
    ia_brain_vigia.time = 0
    
    ia_brain_vigia.shoots = ia_brain_vigia.shoots + 1
    
    if ia_brain_vigia.shoots >= Vigia_Constants["Num Shoots"] then
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Search_Node')
      return
    end
  
  end  
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Shoot['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Rebre_Impacte')
    return
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Search_Node')
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Search Node!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Search_Node["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = 0
  
  log("dafuq")
  
  ia_brain_vigia:choose_new_patrol_position()
  --local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
  --ia_brain_vigia.patrol_direction = ia_brain_vigia.patrol_position - ia_pos
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Search_Node["Exit"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia:set_target_position(ia_brain_vigia.patrol_position)
end

-------------------------------------------------------------------------------------------------
State_Vigia_Search_Node['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > Vigia_Constants["Time Search Node"] then
    
    local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
    local player_pos = ia_brain_vigia.player:get_component(BaseComponent.object_3d):get_position()
    
    local dist_sq = (ia_pos - player_pos):length_sq()
    
    if dist_sq > Vigia_Constants["Distancia Atac"] then
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Patrol')
      return
    else
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Atac')
      return
    end
  end
  
  
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Search_Node['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Rebre_Impacte')
    return
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Rebre Impacte!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Rebre_Impacte["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  local emiter = _enemic:get_component(BaseComponent.emiter)
  
  ia_brain_vigia.time = 0
  
  ia_brain_vigia:fly(false)
  
  emiter:set_active(true)
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Vigia_Rebre_Impacte["Exit"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  local emiter = _enemic:get_component(BaseComponent.emiter)
  
  ia_brain_vigia:fly(true)
  
  emiter:set_active(false)
end

-------------------------------------------------------------------------------------------------
State_Vigia_Rebre_Impacte['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > Vigia_Constants["Time Rebre Impacte"] then
    local vida = _enemic:get_component(BaseComponent.vida)
    
    if vida:get_hp() > 0 then
    
      local ia_pos     = _enemic:get_component(BaseComponent.object_3d):get_position()
      local player_pos = ia_brain_vigia.player:get_component(BaseComponent.object_3d):get_position()
      
      local dist_sq = (ia_pos - player_pos):length_sq()
      
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Search_Node')
      return
      
      --if dist_sq > Vigia_Constants["Distancia Atac"] then
      --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Patrol')
      --  return
      --else
      --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Atac')
      --  return
      --end
    else
      _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Mort')
    end
  end
  
  
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Rebre_Impacte['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    --_enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Rebre_Impacte')
    --return
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Rebre Impacte!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Mort["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  local emiter = _enemic:get_component(BaseComponent.emiter)
  
  ia_brain_vigia:fly(false)
  
  ia_brain_vigia.time = 0
  
  emiter:set_active(true)
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Mort["Exit"] = function(_enemic)
  
  local emiter = _enemic:get_component(BaseComponent.emiter)
  emiter:set_active(false)
end

-------------------------------------------------------------------------------------------------
State_Vigia_Mort['Update'] = function(_enemic, _dt)
  
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia.time = ia_brain_vigia.time + _dt
  
  if ia_brain_vigia.time > 10 then
    local vida = _enemic:get_component(BaseComponent.vida)
    
    vida:set(100)
    
    ia_brain_vigia:fly(true)
    
    _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Vigia_Search_Node')
    
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Vigia_Mort['Receive'] = function(_enemic, _event)
  
end























-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Neutre!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Vigia_Idle["Enter"] = function(_enemic)

  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia:fly(true)
  
  --local animation = _enemic:get_component(BaseComponent.animation)
  --animation:set_animation_state('idle')
end

-------------------------------------------------------------------------------------------------
State_Vigia_Idle["Exit"] = function(_enemic)

end

-------------------------------------------------------------------------------------------------
State_Vigia_Idle['Update'] = function(_enemic, _dt)
  local ia_brain_vigia = _enemic:get_component(BaseComponent.ia_brain_vigia)
  
  ia_brain_vigia:fly(true)
end

-------------------------------------------------------------------------------------------------
State_Vigia_Idle['Receive'] = function(_enemic, _event)
  if _event.msg == Event.rebre_impacte then
    --_enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Hit')
  end
  if _event.msg == Event.morir then
    --_enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
  if _event.msg == Event.rebre_force then
    --g_force_event = _event
  --  _enemic:get_component(BaseComponent.ia_brain):recive_force(_event)
  --  _enemic:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Soldier_Mort')
  end
end
