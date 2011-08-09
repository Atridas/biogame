function pick_up_tetera(_trigger, _actor)
  if _actor:get_name() == "Player" then
    local l_message = EM:get_event()

    l_message.msg = Event.pickup
    l_message.receiver = _actor:get_guid()
    l_message.sender = _trigger:get_guid()
    l_message.dispatch_time = 0
    --Info
    local l_event = l_message:get_info(0)
    l_event.Type = EventInfo.string
    l_event:set_str("tetera")
    l_message:set_info(0, l_event)

    --Send
    EM:send_event(l_message)
    
    _trigger:get_component(BaseComponent.renderable_object).remove_renderable_object = true
    
    EM:remove_entity(_trigger)
  end
end

function esfera_interactiva(_self, _player)
  if _player:get_name() == "Player" then
    local player_controller = _player:get_component(BaseComponent.player_controller)
    
    if player_controller:has_pickup("tetera") then
      log('tens tetera? doncs s\'obre la porta!')
      
      --missatge d'apertura de porta
      local l_door = EM:get_entity("Porta01")
      if l_door then
        local l_message = EM:get_event()

        l_message.msg = Event.obrir
        l_message.receiver = l_door:get_guid()
        l_message.sender = _self:get_guid()
        l_message.dispatch_time = 0
        
        EM:send_event(l_message)
        
        --s'esborra el pickup
        player_controller:remove_pickup("tetera")
      else
        log('error, no es troba la porta')
      end
    else
      log('agafa una tetera abans!')
    end
  end
end

function esfera_destructible(_self)
  log('S�c ' .. _self:get_name() .. ' i m\'ha matat ')
end


function laboratori_petar_llum_porta(_self)
  local l_door = EM:get_entity("Porta_Laboratori")
  if l_door then
    local l_message = EM:get_event()

    l_message.msg = Event.obrir
    l_message.receiver = l_door:get_guid()
    l_message.sender = _self:get_guid()
    l_message.dispatch_time = 0
    
    EM:send_event(l_message)
  else
    log('error, no es troba la porta')
  end
end

function salavideo_palanca(_self, _player)
  if _player:get_name() == "Player" then
    local player_controller = _player:get_component(BaseComponent.player_controller)
    
    --missatge d'apertura de porta
    local l_door = EM:get_entity("Porta_Laser")
    if l_door then
      local l_message = EM:get_event()

      l_message.msg = Event.obrir
      l_message.receiver = l_door:get_guid()
      l_message.sender = _self:get_guid()
      l_message.dispatch_time = 0
      
      EM:send_event(l_message)
    else
      log('error, no es troba la porta')
    end
  end
end