-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Configuració! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

Key_Machine_Constants = {}

-- Llums
Key_Machine_Constants["LIGHT Top on"]  = "proves_key_top_on"
Key_Machine_Constants["LIGHT Top off"] = "proves_key_top_off"

Key_Machine_Constants["LIGHT Right on"]  = "proves_key_right_on"
Key_Machine_Constants["LIGHT Right off"] = "proves_key_right_off"

Key_Machine_Constants["LIGHT Bot on"]  = "proves_key_bot_on"
Key_Machine_Constants["LIGHT Bot off"] = "proves_key_bot_off"

Key_Machine_Constants["LIGHT Left on"]  = "proves_key_left_on"
Key_Machine_Constants["LIGHT Left off"] = "proves_key_left_off"

-- Claus
Key_Machine_Constants["KEY Top"]   = "proves_key_top"
Key_Machine_Constants["KEY Right"] = "proves_key_right"
Key_Machine_Constants["KEY Bot"]   = "proves_key_bot"
Key_Machine_Constants["KEY Left"]  = "proves_key_left"

-- Portes
Key_Machine_Constants["DOOR Top"]   = "Porta01"
Key_Machine_Constants["DOOR Right"] = "Porta02"
Key_Machine_Constants["DOOR Bot"]   = "Porta01"
Key_Machine_Constants["DOOR Left"]  = "Porta02"

-- Temps
Key_Machine_Constants["Last Door Waiting"] = 5.0
Key_Machine_Constants["Last Door Waiting Count"] = 0.0

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Definició d'estats ---------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Key_AllOff   = {}

State_Key_TopOn  = {}

State_Key_RightOn  = {}

State_Key_BotOn = {}

State_Key_LeftOn = {}

State_Key_AllOn    = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- ALL OFF!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_AllOff["Enter"] = function(_self)
  local l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top off"])
  if l_light then
    l_light:set_active(true)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot off"])
  if l_light then
    l_light:set_active(true)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left off"])
  if l_light then
    l_light:set_active(true)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right off"])
  if l_light then
    l_light:set_active(true)
  end
end

-------------------------------------------------------------------------------------------------
State_Key_AllOff["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_AllOff['Update'] = function(_self, _dt)

end

-------------------------------------------------------------------------------------------------
State_Key_AllOff['Receive'] = function(_self, _event)
  if _event.msg == Event.interaccio then
    local l_player = EM:get_entity(_event.sender)
    if l_player then
      if unlock(l_player, Key_Machine_Constants["KEY Top"], Key_Machine_Constants["DOOR Top"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_TopOn')
      end
    else
      log("KeyStateMachine::AllOff No es troba l'entitat player a través del missatge")
    end
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- TOP ON!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_TopOn["Enter"] = function(_self)
  local l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top on"])
  if l_light then
    l_light:set_active(true)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top off"])
  if l_light then
    l_light:set_active(false)
  end
end

-------------------------------------------------------------------------------------------------
State_Key_TopOn["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_TopOn['Update'] = function(_self, _dt)

end

-------------------------------------------------------------------------------------------------
State_Key_TopOn['Receive'] = function(_self, _event)
  if _event.msg == Event.interaccio then
    local l_player = EM:get_entity(_event.sender)
    if l_player then
      if unlock(l_player, Key_Machine_Constants["KEY Right"], Key_Machine_Constants["DOOR Right"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_RightOn')
      end
    else
      log("KeyStateMachine::AllOff No es troba l'entitat player a través del missatge")
    end
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- RIGHT ON!!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_RightOn["Enter"] = function(_self)
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right on"])
  if l_light then
    l_light:set_active(true)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right off"])
  if l_light then
    l_light:set_active(false)
  end
end

-------------------------------------------------------------------------------------------------
State_Key_RightOn["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_RightOn['Update'] = function(_self, _dt)

end

-------------------------------------------------------------------------------------------------
State_Key_RightOn['Receive'] = function(_self, _event)
  if _event.msg == Event.interaccio then
    local l_player = EM:get_entity(_event.sender)
    if l_player then
      if unlock(l_player, Key_Machine_Constants["KEY Bot"], Key_Machine_Constants["DOOR Bot"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_BotOn')
      end
    else
      log("KeyStateMachine::AllOff No es troba l'entitat player a través del missatge")
    end
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- BOT ON!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_BotOn["Enter"] = function(_self)
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot on"])
  if l_light then
    l_light:set_active(true)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot off"])
  if l_light then
    l_light:set_active(false)
  end
end

-------------------------------------------------------------------------------------------------
State_Key_BotOn["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_BotOn['Update'] = function(_self, _dt)

end

-------------------------------------------------------------------------------------------------
State_Key_BotOn['Receive'] = function(_self, _event)
  if _event.msg == Event.interaccio then
    local l_player = EM:get_entity(_event.sender)
    if l_player then
      if unlock(l_player, Key_Machine_Constants["KEY Left"], Key_Machine_Constants["DOOR Left"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_LeftOn')
      end
    else
      log("KeyStateMachine::AllOff No es troba l'entitat player a través del missatge")
    end
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- LEFT ON!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_LeftOn["Enter"] = function(_self)
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left on"])
  if l_light then
    l_light:set_active(true)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left off"])
  if l_light then
    l_light:set_active(false)
  end
  
  Key_Machine_Constants["Last Door Waiting Count"] = 0.0
end

-------------------------------------------------------------------------------------------------
State_Key_LeftOn["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_LeftOn['Update'] = function(_self, _dt)
  Key_Machine_Constants["Last Door Waiting Count"] = Key_Machine_Constants["Last Door Waiting Count"] + _dt
  if Key_Machine_Constants["Last Door Waiting Count"] >= Key_Machine_Constants["Last Door Waiting"] then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_AllOn')
  end
end

-------------------------------------------------------------------------------------------------
State_Key_LeftOn['Receive'] = function(_self, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- ALL ON!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Key_AllOn["Enter"] = function(_self)
  local l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Top off"])
  if l_light then
    l_light:set_active(false)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Bot off"])
  if l_light then
    l_light:set_active(false)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Left off"])
  if l_light then
    l_light:set_active(false)
  end
  
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right on"])
  if l_light then
    l_light:set_active(false)
  end
  l_light = LM:get_resource(Key_Machine_Constants["LIGHT Right off"])
  if l_light then
    l_light:set_active(false)
  end
  
  --TODO: Crear particules del centre
end

-------------------------------------------------------------------------------------------------
State_Key_AllOn["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Key_AllOn['Update'] = function(_self, _dt)

end

-------------------------------------------------------------------------------------------------
State_Key_AllOn['Receive'] = function(_self, _event)

end
