-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Configuració! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

Key_Machine_Constants = {}

-- Llums
Key_Machine_Constants["LIGHT Init"] = "lvl1_Omni116"

Key_Machine_Constants["LIGHT Top on"]  = "lvl1_porta_claus_light_top_on"
Key_Machine_Constants["LIGHT Top off"] = "lvl1_porta_claus_light_top_off"
Key_Machine_Constants["LIGHT Top Center"] = "lvl1_Omni107"

Key_Machine_Constants["LIGHT Bot on"]  = "lvl1_porta_claus_light_bot_on"
Key_Machine_Constants["LIGHT Bot off"] = "lvl1_porta_claus_light_bot_off"
Key_Machine_Constants["LIGHT Bot Center"] = "lvl1_Omni108"

Key_Machine_Constants["LIGHT Right on"]  = "lvl1_porta_claus_light_right_on"
Key_Machine_Constants["LIGHT Right off"] = "lvl1_porta_claus_light_right_off"
Key_Machine_Constants["LIGHT Right Center"] = "lvl1_Omni106"

Key_Machine_Constants["LIGHT Left on"]  = "lvl1_porta_claus_light_left_on"
Key_Machine_Constants["LIGHT Left off"] = "lvl1_porta_claus_light_left_off"
Key_Machine_Constants["LIGHT Left Center"] = "lvl1_Omni103"

-- Claus
Key_Machine_Constants["KEY Top"]   = "lvl1_key_green"
Key_Machine_Constants["KEY Bot"]   = "lvl1_key_blue"
Key_Machine_Constants["KEY Right"] = "lvl1_key_purple"
Key_Machine_Constants["KEY Left"]  = "lvl1_key_yellow"

-- Portes
Key_Machine_Constants["DOOR Init"]  = "lvl1_door_inici"
Key_Machine_Constants["DOOR Top"]   = "lvl1_door03"
Key_Machine_Constants["DOOR Bot"]   = "lvl1_door04"
Key_Machine_Constants["DOOR Right"] = "lvl1_door01"
Key_Machine_Constants["DOOR Left"]  = "lvl1_door_final"

-- Temps
Key_Machine_Constants["Last Door Waiting"] = 3.0
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
-- Helper Functions -----------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
function change_to_red(_LightName)
  local l_light = LM:get_resource(_LightName)
  if l_light then
    local l_color = Color(1.0, 0.0, 0.0, 1.0)
    l_light:set_color(l_color)
  else
    log("change_to_red:: No es troba la llum: " .. _LightName)
  end
end

function change_to_green(_LightName)
  local l_light = LM:get_resource(_LightName)
  if l_light then
    local l_color = Color(0.0, 1.0, 0.0, 1.0)
    l_light:set_color(l_color)
  else
    log("change_to_red:: No es troba la llum: " .. _LightName)
  end
end

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
  
  --canviar llum
  change_to_green(Key_Machine_Constants["LIGHT Top Center"])
  
  --bloquejar porta inici
  local l_door = EM:get_entity(Key_Machine_Constants["DOOR Init"])
  if l_door then
    l_door = l_door:get_component(BaseComponent.door)
    if l_door then
      l_door:block(true)
      change_to_red(Key_Machine_Constants["LIGHT Init"])
    else
      log("KeyStateMachine::TopOn No es troba la porta de l'entitat: " .. Key_Machine_Constants["DOOR Init"])
    end
  else
    log("KeyStateMachine::TopOn No es troba l'entitat: " .. Key_Machine_Constants["DOOR Init"])
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
      if unlock(l_player, Key_Machine_Constants["KEY Bot"], Key_Machine_Constants["DOOR Bot"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_BotOn')
      end
    else
      log("KeyStateMachine::TopOn No es troba l'entitat player a través del missatge")
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
  
  --canviar llum
  change_to_green(Key_Machine_Constants["LIGHT Bot Center"])
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
      if unlock(l_player, Key_Machine_Constants["KEY Right"], Key_Machine_Constants["DOOR Right"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_RightOn')
      end
    else
      log("KeyStateMachine::BotOn No es troba l'entitat player a través del missatge")
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
  
  --canviar llum
  change_to_green(Key_Machine_Constants["LIGHT Right Center"])
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
      if unlock(l_player, Key_Machine_Constants["KEY Left"], Key_Machine_Constants["DOOR Left"]) then
        _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Key_LeftOn')
      end
    else
      log("KeyStateMachine::RightOn No es troba l'entitat player a través del missatge")
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
  
  --canviar llum
  change_to_green(Key_Machine_Constants["LIGHT Left Center"])
  
  activate_entity("lvl1_miner07")
  
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
