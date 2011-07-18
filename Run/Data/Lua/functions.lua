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
  end
end