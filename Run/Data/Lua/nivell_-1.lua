-------------------------------------------- FUNCIONS  -------------------------------------------
-- Inici
function init_level_menys_1()  
  local elevator = EM:get_entity("lvl1_montacarregues")
  
  local l_player = EM:get_entity("Player")
  
  local physx_controller = l_player:get_component(BaseComponent.physx_controller)
  physx_controller:use_gravity(false)
  
  if elevator then      
    --activar l'ascensor
    local l_message = EM:get_event()

    l_message.msg = Event.obrir
    l_message.sender = elevator:get_guid()
    l_message.receiver = elevator:get_guid()
    l_message.dispatch_time = 0
    
    EM:send_event(l_message)
    
    --canviar la càmara i desactivar el player
    activate_cynematic_camera("lvl1_elevator_camera_init")
    l_player:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Inactiu')
    
    --EM:get_entity("lvl1_physXBox_elevator"):set_active(false)
    deactivate_entity("lvl1_physXBox_elevator")
	
    --fade in
    EFFECT_MANAGER:set_exposure(0)
    RENDERER:deactivate_render_path("aim_gui")
    RENDERER:blend_parameter(Renderer.exposure, 0.5, 3)
    
    SOUND:play_sample("elevator_arrival")
    
  else
    log("Error: No es troba l'entitat: lvl1_montacarregues")
  end
end

-- Granades
function activate_grenades(_self, _actor)
  if _actor:get_name() == "Player" then
    --Rebre la capacitat de tirar granades
    tutorial_grenade()
    _actor:get_component(BaseComponent.player_controller).grenade_active = true
    
    --destruim la granada
    _self:get_component(BaseComponent.renderable_object).remove_renderable_object = true
    EM:remove_entity(_self)
  end
end

-- CLAUS
function get_key_green(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_green")
    
    --obrir porta claus
    send_open_door("lvl1_door_claus", _actor)
    change_to_green("lvl1_Omni117")
    
    tutorial_key()
  end
end

function get_key_blue(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_blue")
    --obrim la porta del passadís
    send_open_door("lvl1_door03", _actor)
    --activem els enemics
    activate_entity("lvl1_miner00")
    activate_entity("lvl1_militar00")
    activate_entity("lvl1_militar01")
    --obrim la porta de claus
    send_open_door("lvl1_door_claus", _actor)
  end
end

function get_key_purple(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_purple")
    --obrim la porta del passadís
    send_open_door("lvl1_door04", _actor)
    activate_entity("lvl1_militar09")
    activate_entity("lvl1_miner03")
    --obrim la porta de claus    
    send_open_door("lvl1_door_claus", _actor)
  end
end

function get_key_yellow(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_yellow")
    --obrim la porta del passadís
    send_open_door("lvl1_door01", _actor)
    activate_entity("lvl1_militar10")
    activate_entity("lvl1_militar11")
    --activate_entity("lvl1_militar12")
    activate_entity("lvl1_militar13")
    activate_entity("lvl1_miner06")
    --obrim la porta de claus
    send_open_door("lvl1_door_claus", _actor)
  end
end

-- OBRIR PORTES
--function validate_key(_self, _player)
--  if _player:get_name() == "Player" then
--    local l_name = _self:get_name()
--    
--    if l_name == "lvl1_unlock_green" then
--      if unlock(_player, "lvl1_key_green", "lvl1_door03") then
--        _self:delete_component(BaseComponent.interactive)
--      end
--
--    elseif l_name == "lvl1_unlock_blue" then
--      if unlock(_player, "lvl1_key_blue", "lvl1_door04") then
--        _self:delete_component(BaseComponent.interactive)
--      end
--    
--    elseif l_name == "lvl1_unlock_purple" then
--      if unlock(_player, "lvl1_key_purple", "lvl1_door01") then
--        _self:delete_component(BaseComponent.interactive)
--      end
--    
--    elseif l_name == "lvl1_unlock_yellow" then
--      if unlock(_player, "lvl1_key_yellow", "lvl1_door_final") then
--        activate_entity("lvl1_miner07")
--        _self:delete_component(BaseComponent.interactive)
--      end
--    end
--  end
--end

function unlock(_player, _keyname, _doorname)
  local player_controller = _player:get_component(BaseComponent.player_controller)
  local l_door = EM:get_entity(_doorname):get_component(BaseComponent.door)
  if l_door and player_controller then
    if player_controller:has_pickup(_keyname) then
      --unlock
      l_door:block(false)
      
      --open
      send_open_door(_doorname, _player)
      
      --remove key
      player_controller:remove_pickup(_keyname)
      
      return true
    end
  else
    log("Error: al buscar la porta ".. _doorname .. " o bé al agafar el component playercontroller de " .. _player:get_name())
  end
  
  return false
end

function lvl1_activate_cynematic(_cynematic_num)
  l_scripted_entity = EM:create_entity()
  
  ComponentDelayedScript.add_to_entity(l_scripted_entity, 3, "lvl1_deactivate_cynematic")
  
  activate_cynematic_camera("lvl1_camera_porta" .. _cynematic_num)
  RENDERER:deactivate_render_path("aim_gui")
  
end

function lvl1_deactivate_cynematic(_self)
  deactivate_cynematic_camera()
  RENDERER:activate_render_path("aim_gui")
  
  EM:remove_entity(_self)
end

--other
function drop_key(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    if _self:get_name() == "lvl1_miner_green" then
      EM:init_pickup("lvl1_key_green", "lvl1_key_green", l_o3d:get_position(), "get_key_green")
    end
  end
end

--entrega totes les claus al Player
function hack_door()
  local l_player = EM:get_entity("Player")
  local l_message = EM:get_event()
  --TOP--
  
  l_message.msg = Event.pickup
  l_message.receiver = l_player:get_guid()
  l_message.sender = l_player:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str(Key_Machine_Constants["KEY Top"])
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --RIGHT--
  l_event:set_str(Key_Machine_Constants["KEY Bot"])
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --BOT--
  l_event:set_str(Key_Machine_Constants["KEY Right"])
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  --LEFT--
  l_event:set_str(Key_Machine_Constants["KEY Left"])
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
end

-------------------------------------------- TRIGGERS  -------------------------------------------
--aturar l'ascensor d'entrada
function lvl1_stop_elevator(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
  
    local elevator = EM:get_entity("lvl1_montacarregues")
    
    if elevator then
      local pbox = EM:get_entity("lvl1_physXBox_elevator")
      pbox:set_active(true)
      pbox = pbox:get_component(BaseComponent.physx_actor)
	  
	  local physx_controller = _Entity:get_component(BaseComponent.physx_controller)
  
	  physx_controller:use_gravity(true)
      
      --aturar l'ascensor
      local l_message = EM:get_event()

      l_message.msg = Event.tancar
      l_message.sender = elevator:get_guid()
      l_message.receiver = elevator:get_guid()
      l_message.dispatch_time = 0
      
      EM:send_event(l_message)
      
      --restaurar la càmara i el player
      deactivate_cynematic_camera()
      RENDERER:activate_render_path("aim_gui")
      _Entity:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
      --megacheat_level_1()
      
      --posem l'ascensor al seu lloc
      local elevator_position = elevator:get_component(BaseComponent.object_3d):get_position()
      Elevator_Constants["Final Position"][0] = elevator_position.x
      Elevator_Constants["Final Position"][1] = pbox:get_position().y
      Elevator_Constants["Final Position"][2] = elevator_position.z
      
      EM:remove_entity(_EntityTrigger)
      
    else
      log("Error: No es troba l'entitat: lvl1_montacarregues")
    end
  end
end

function megacheat_level_1()
  EM:get_entity("Player"):get_component(BaseComponent.physx_controller):set_position( Vect3f( -0.31565, 0.973216474, -23.9925994873) )
end



--activar l'ascensor de sortida
function lvl1_activate_elevator(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" and _Entity:get_component(BaseComponent.player_controller):is_alive() then
    god_mode(true)
    local elevator = EM:get_entity("lvl1_montacarregues01")
    
    local physx_controller = _Entity:get_component(BaseComponent.physx_controller)
    physx_controller:use_gravity(false)
    
    
    if elevator then      
      --aturar l'ascensor
      local l_message = EM:get_event()

      l_message.msg = Event.obrir
      l_message.sender = elevator:get_guid()
      l_message.receiver = elevator:get_guid()
      l_message.dispatch_time = 0
      
      EM:send_event(l_message)
      
      RENDERER:blend_parameter(Renderer.exposure, 0, 10)
      
      --canviar la càmara i desactivar el player
      activate_cynematic_camera("lvl1_elevator_camera_exit")
      _Entity:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Inactiu')
      
      RENDERER:deactivate_render_path("aim_gui")
      EM:remove_entity(_EntityTrigger)
      
      SOUND:play_sample("elevator")
      
    else
      log("Error: No es troba l'entitat: lvl1_montacarregues01")
    end
  end
end

--canviar de nivell
function change_level_level_1(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    deactivate_cynematic_camera()
    set_new_level("Hangar")
  end
end

function activate_green_key(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity("lvl1_miner_green")
    activate_entity("Vigia01")
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

--magatzem

function lvl1_trigger_force(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    _Entity:get_component(BaseComponent.player_controller).force_active = true
    tutorial_force()
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
    deactivate_entity("lvl1_force_billboard")
  end
end
  
function lvl1_trigger_magatzem01(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_purple") then
      activate_entity("lvl1_miner02")
      activate_entity("lvl1_militar07")
      activate_entity("lvl1_militar08")
      
      --destrucció del trigger
      EM:remove_entity(_EntityTrigger)
    end
  end
end

function lvl1_trigger_magatzem02(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_purple") then
      activate_entity("lvl1_militar06")
      activate_entity("lvl1_militar02")
      activate_entity("lvl1_militar05")
      
      --destrucció del trigger
      EM:remove_entity(_EntityTrigger)
    end
  end
end

function lvl1_trigger_magatzem03(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity("Vigia02")
      
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function lvl1_trigger_magatzem04(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity("Vigia03")
      
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function lvl1_trigger_lavabo(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity("Vigia04")
      
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function lvl1_trigger_pass_mag(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_purple") then
      activate_entity("lvl1_miner01")
      activate_entity("lvl1_militar03")
      activate_entity("lvl1_militar04")
      
      --destrucció del trigger
      EM:remove_entity(_EntityTrigger)
    end
  end
end

--dormitoris
function lvl1_trigger_llits(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_yellow") then
      activate_entity("lvl1_miner05")
      activate_entity("lvl1_militar14")
      activate_entity("lvl1_militar15")
      
      --destrucció del trigger
      EM:remove_entity(_EntityTrigger)
    end
  end
end

function lvl1_trigger_pas_llits(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_yellow") then
      --obrim la porta
      send_open_door("lvl1_door01", _Entity)
      --activem els enemics
      activate_entity("lvl1_miner04")
      activate_entity("lvl1_militar16")
      activate_entity("lvl1_militar17")
      activate_entity("lvl1_militar18")
      activate_entity("lvl1_militar19")
      
      --destrucció del trigger
      EM:remove_entity(_EntityTrigger)
    end
  end
end