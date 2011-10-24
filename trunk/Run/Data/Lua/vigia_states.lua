State_Vigia_Idle = {}

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
