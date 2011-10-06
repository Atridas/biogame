function test()
  local l_player = EM:get_entity("Player")
  local l_tmp = EM:get_entity("proves_BoxReflection")
  
  local l_message = EM:get_event()

  --TOP--
  
  l_message.msg = Event.pickup
  l_message.receiver = l_player:get_guid()
  l_message.sender = l_tmp:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str("proves_key_top")
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --RIGHT--
  
  l_message.msg = Event.pickup
  l_message.receiver = l_player:get_guid()
  l_message.sender = l_tmp:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str("proves_key_right")
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --BOT--
  
  l_message.msg = Event.pickup
  l_message.receiver = l_player:get_guid()
  l_message.sender = l_tmp:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str("proves_key_bot")
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --LEFT--
  
  l_message.msg = Event.pickup
  l_message.receiver = l_player:get_guid()
  l_message.sender = l_tmp:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str("proves_key_left")
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
end