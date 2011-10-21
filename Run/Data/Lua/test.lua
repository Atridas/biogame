function test()
  god_mode(true)
  exit()
end

function hangar()
  EM:get_entity("hangar_puerta_hangar"):get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Hangar_Porta_Opening')
end