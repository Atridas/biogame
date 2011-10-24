Elevator_Constants = {}
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Constants  !!!! ------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

Elevator_Constants["Speed"]  = 0.5

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- States  !!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Elevator_Idle   = {}
State_Elevator_Moving = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Idle  !!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Elevator_Idle['Enter'] = function(_entitat)

end

-------------------------------------------------------------------------------------------------
State_Elevator_Idle['Exit'] = function(_entitat)
  
end

-------------------------------------------------------------------------------------------------
State_Elevator_Idle['Update'] = function(_entitat, _dt)

end

-------------------------------------------------------------------------------------------------
State_Elevator_Idle['Receive'] = function(_entitat, _event)
  if _event.msg == Event.obrir then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Elevator_Moving')
    return
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Moving  !!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Elevator_Moving['Enter'] = function(_entitat)
  --TODO: so montacÓrregues
end

-------------------------------------------------------------------------------------------------
State_Elevator_Moving['Exit'] = function(_entitat)
  --TODO: aturar so montacÓrregues
end

-------------------------------------------------------------------------------------------------
State_Elevator_Moving['Update'] = function(_entitat, _dt)
  local physx = _entitat:get_component(BaseComponent.physx_actor)
  local position = physx:get_position()
  
  physx:set_position(Vect3f(position.x, position.y + Elevator_Constants["Speed"] * _dt, position.z))
  
  position = physx:get_position()
  
  --Moviment del player!
  local player = EM:get_entity("Player")
  
  physx = player:get_component(BaseComponent.physx_controller)
  local player_position = physx:get_position()
  
  physx:set_position(Vect3f(player_position.x, position.y, player_position.z))
  
end

-------------------------------------------------------------------------------------------------
State_Elevator_Moving['Receive'] = function(_entitat, _event)
  if _event.msg == Event.tancar then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Elevator_Idle')
    return
  end
end