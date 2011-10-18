-- Estats de la porta

Porta_Constants = {}

--animacions
Porta_Constants["Open Animation"] = "obrir"
Porta_Constants["Open"] = "idleopen"
Porta_Constants["Close Animation"] = "tancar"
Porta_Constants["Closed"] = "idleclose"

--temps animacions
--Porta_Constants["Open Speed"] = 2.0
--Porta_Constants["Close Speed"] = 0.5


State_Porta_Open    = {}
State_Porta_Opening = {}

State_Porta_Closed  = {}
State_Porta_Closing = {}

-------------------------------------------------------------------------------------------------
function open_door(_trigger, _actor)
  local l_trigger = _trigger:get_name()
  
  --LEVEL PROVES
  if l_trigger == "TriggerPorta01" then
    send_open_door("Porta01", _actor)
    send_open_door("Porta02", _actor)
    
  --LEVEL -2
  elseif l_trigger == "Trigger_Porta_Video" then
    if _actor:get_name() == "Player" then
      send_open_door("Porta_Videovigilancia", _actor)
      activate_entity("Miner_video00")
    end
    
  --LEVEL -1 : Sala Central
  elseif l_trigger == "lvl1_trigger_porta_inici" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door_inici", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta04" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door04", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta01" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door01", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta_final" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door_final", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta_claus" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door_claus", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta03" then
    if _actor:get_name() == "Player" then
      send_open_door("lvl1_door03", _actor)
    end
    
  end
end


function close_door(_trigger, _actor)
  local l_trigger = _trigger:get_name()

  --LEVEL PROVES
  if l_trigger == "TriggerPorta01" then
    send_close_door("Porta01", _actor)
    send_close_door("Porta02", _actor)

  --LEVEL -2    
  elseif l_trigger == "Lab_Trigger_Sortida" then
    if _actor:get_name() == "Player" then
      send_close_door("Porta_Laboratori", _actor)
    end
    
  elseif l_trigger == "Trigger_Porta_Video" then
    if _actor:get_name() == "Player" then
      send_close_door("Porta_Videovigilancia", _actor)
    end
  
  --LEVEL -1 : Sala Central
  elseif l_trigger == "lvl1_trigger_porta_inici" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door_inici", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta04" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door04", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta01" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door01", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta_final" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door_final", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta_claus" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door_claus", _actor)
    end
    
  elseif l_trigger == "lvl1_trigger_porta03" then
    if _actor:get_name() == "Player" then
      send_close_door("lvl1_door03", _actor)
    end
    
  end
end

function send_open_door(_doorName, _actor)
  --Missatge
  local l_message = EM:get_event()

  l_message.msg = Event.obrir
  l_message.sender = _actor:get_guid()
  l_message.dispatch_time = 0
  
  local l_door = EM:get_entity_from_name(_doorName)    
  if l_door then
    l_message.receiver = l_door:get_guid()
    
    EM:send_event(l_message)
  else
    log('Error al buscar la porta: '.. _doorName)
  end
end

function send_close_door(_doorName, _actor)
  --Missatge
  local l_message = EM:get_event()

  l_message.msg = Event.tancar
  l_message.sender = _actor:get_guid()
  l_message.dispatch_time = 0
  
  local l_door = EM:get_entity_from_name(_doorName)    
  if l_door then
    l_message.receiver = l_door:get_guid()
    
    EM:send_event(l_message)
  else
    log('Error al buscar la porta: '.. _doorName)
  end
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Open  !!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Porta_Open['Enter'] = function(_entitat)
  --comprovar que la porta no estigui bloquejada
  local l_door = _entitat:get_component(BaseComponent.door)
  
  if l_door:is_active() then
    --l_door:set_active(false)
    
    --if l_door:open() == false then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
    --  return
    --end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:play_cycle(Porta_Constants["Open"], 0.0, 1.0)
  l_animation:set_animation_state('opened')
end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Exit'] = function(_entitat)
  --local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:clear_all_cycles(0.0)
end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Update'] = function(_entitat, _dt)

end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Receive'] = function(_entitat, _event)
  --local l_sender = EM:get_entity(_event.sender)
  --log('open: event received ' .. l_sender:get_name() )
  
  if _event.msg == Event.tancar then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closing')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Opening!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Porta_Opening['Enter'] = function(_entitat)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = 0.0

  --comprovar que la porta no estigui bloquejada
  if l_door:is_blocked() then
   if l_door:is_active() then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
      return
    else
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:play(Porta_Constants["Open Animation"], 0.0, 1.0, true)
  l_animation:set_animation_state('opened')
  
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Exit'] = function(_entitat)
  --local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:stop(Porta_Constants["Open Animation"])
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Update'] = function(_entitat, _dt)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = l_door.time + _dt
  
  if l_door:is_active() and l_door.time >= l_door:get_open_time() * 0.5 then
    l_door:set_active(false)
  end
  
  if l_door.time >= l_door:get_open_time() then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Receive'] = function(_entitat, _event)
  --local l_sender = EM:get_entity(_event.sender)
  --log('opening: event received ' .. l_sender:get_name() )
  
  if _event.msg == Event.tancar then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closing')
    return
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Closed!!!! -----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Porta_Closed['Enter'] = function(_entitat)
  --comprovar que la porta no estigui oberta
  local l_door = _entitat:get_component(BaseComponent.door)
  if l_door:is_active() == false then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
    return
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:play_cycle(Porta_Constants["Closed"], 0.0, 1.0)
  l_animation:set_animation_state('closed')

end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Exit'] = function(_entitat)
  --animacio
  --local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:clear_all_cycles(0.0)

end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Update'] = function(_entitat, _dt)
 
end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Receive'] = function(_entitat, _event)
  --local l_sender = EM:get_entity(_event.sender)
  --log('closed: event received ' .. l_sender:get_name() )
  
  if _event.msg == Event.obrir then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Opening')
    return
  end
end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Closing!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Porta_Closing['Enter'] = function(_entitat)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = 0.0
  
  --comprovar que la porta no estigui bloquejada
  if l_door:is_blocked() then
    if l_door:is_active() then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
      return
    else
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:play(Porta_Constants["Close Animation"], 0.0, 1.0,true)
  l_animation:set_animation_state('closed')
  
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Exit'] = function(_entitat)
  --animacio
  --local l_animation = _entitat:get_component(BaseComponent.animation)
  --l_animation:stop(Porta_Constants["Close Animation"])
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Update'] = function(_entitat, _dt)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = l_door.time + _dt
  
  if l_door:is_active() == false and l_door.time >= l_door:get_close_time() * 0.5 then
    l_door:set_active(true)
  end
  
  if l_door.time >= l_door:get_close_time() then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Receive'] = function(_entitat, _event)
  --local l_sender = EM:get_entity(_event.sender)
  --log('closing: event received ' .. l_sender:get_name() )
 
  if _event.msg == Event.obrir then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Opening')
    return
  end
end