-- Estats de la porta

Porta_Constants = {}

--animacions
Porta_Constants["Open Animation"] = "obrir"
Porta_Constants["Open"] = "idleopen"
Porta_Constants["Close Animation"] = "tancar"
Porta_Constants["Closed"] = "idleclose"
--temps animacions
Porta_Constants["Open Speed"] = 2.0
Porta_Constants["Close Speed"] = 0.5


State_Porta_Open    = {}
State_Porta_Opening = {}

State_Porta_Closed  = {}
State_Porta_Closing = {}

-------------------------------------------------------------------------------------------------
function open_door(_trigger, _actor)
  local l_trigger = _trigger:get_name()
  
  --Missatge
  local l_message = EM:get_event()

  l_message.msg = Event.obrir
  l_message.sender = _actor:get_guid()
  l_message.dispatch_time = 0
  
  if l_trigger == "TriggerPorta01" then
    local l_door = EM:get_entity_from_name("Porta01")
    
    l_message.receiver = l_door:get_guid()
    
    EM:send_event(l_message)
  --LEVEL -2 : Passadís
  elseif l_trigger == "Trigger_Porta_Video" then
    if _actor:get_name() == "Player" then
      local l_door = EM:get_entity_from_name("Porta_Videovigilancia")
    
      if l_door then
        l_message.receiver = l_door:get_guid()
      
        EM:send_event(l_message)
      end
    end
  end
end


function close_door(_trigger, _actor)
  local l_trigger = _trigger:get_name()
  
  --Missatge
  local l_message = EM:get_event()

  l_message.msg = Event.tancar
  l_message.sender = _actor:get_guid()
  l_message.dispatch_time = 0
  
  if l_trigger == "TriggerPorta01" then
    local l_door = EM:get_entity_from_name("Porta01")
    
    if l_door then
      l_message.receiver = l_door:get_guid()
      
      EM:send_event(l_message)
    else
      log('Error al buscar la porta')
    end
  --LEVEL -2 : Laboratori    
  elseif l_trigger == "Lab_Trigger_Sortida" then
    local l_door = EM:get_entity_from_name("Porta_Laboratori")

    if l_door and (_actor:get_name() == "Player") then
      l_message.receiver = l_door:get_guid()
      
      EM:send_event(l_message)
    else
      log('Error al buscar la porta')
    end
  --LEVEL -2 : Passadís
  elseif l_trigger == "Trigger_Porta_Video" then
    if _actor:get_name() == "Player" then
      local l_door = EM:get_entity_from_name("Porta_Videovigilancia")
    
      if l_door then
        l_message.receiver = l_door:get_guid()
      
        EM:send_event(l_message)
      end
    end
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
  
  if l_door:is_open() == false then
    if l_door:open() == false then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:play(Porta_Constants["Open"], 0.0, 1.0, true)

  --física
  local l_physx = _entitat:get_component(BaseComponent.physx_actor)
  l_physx:activate(false)
end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Exit'] = function(_entitat)
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:stop(Porta_Constants["Open"])
end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Update'] = function(_entitat, _dt)

end

-------------------------------------------------------------------------------------------------
State_Porta_Open['Receive'] = function(_entitat, _event)
  local l_sender = EM:get_entity(_event.sender)
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
  if l_door:is_open() then
    if l_door:close() == false then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:play(Porta_Constants["Open Animation"], 0.0, 1.0, true)

  --física
  local l_physx = _entitat:get_component(BaseComponent.physx_actor)
  l_physx:activate(true)
  
  
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Exit'] = function(_entitat)
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:stop(Porta_Constants["Open Animation"])
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Update'] = function(_entitat, _dt)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = l_door.time + _dt
  
  if l_door.time >= Porta_Constants["Open Speed"] then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Porta_Opening['Receive'] = function(_entitat, _event)
  local l_sender = EM:get_entity(_event.sender)
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
  --comprovar que la porta no estigui bloquejada
  local l_door = _entitat:get_component(BaseComponent.door)
  if l_door:is_open() then
    if l_door:close() == false then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:play(Porta_Constants["Closed"], 0.0, 1.0, true)

  --física
  local l_physx = _entitat:get_component(BaseComponent.physx_actor)
  l_physx:activate(true)
end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Exit'] = function(_entitat)

end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Update'] = function(_entitat, _dt)
 
end

-------------------------------------------------------------------------------------------------
State_Porta_Closed['Receive'] = function(_entitat, _event)
  local l_sender = EM:get_entity(_event.sender)
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
  if l_door:is_open() then
    if l_door:close() == false then
      _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Open')
      return
    end
  end
  
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:play(Porta_Constants["Close Animation"], 0.0, 1.0, true)
  
  --física
  local l_physx = _entitat:get_component(BaseComponent.physx_actor)
  l_physx:activate(true)
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Exit'] = function(_entitat)
  --animacio
  local l_animation = _entitat:get_component(BaseComponent.animation)
  l_animation:stop(Porta_Constants["Close Animation"])
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Update'] = function(_entitat, _dt)
  local l_door = _entitat:get_component(BaseComponent.door)
  l_door.time = l_door.time + _dt
  
  if l_door.time >= Porta_Constants["Close Speed"] then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Closed')
    return
  end
end

-------------------------------------------------------------------------------------------------
State_Porta_Closing['Receive'] = function(_entitat, _event)
  local l_sender = EM:get_entity(_event.sender)
  --log('closing: event received ' .. l_sender:get_name() )
  
  if _event.msg == Event.obrir then
    _entitat:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Porta_Opening')
    return
  end
end