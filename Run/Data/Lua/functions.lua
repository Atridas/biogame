-------------------------------------------- GLOBALS  -------------------------------------------
function activate_entity(_name)
  local l_entity = EM:get_entity(_name)
  if l_entity then
    l_entity:set_active(true)
    --log("Activant l'entitat: " .. _name)
  else
    log('Warning: no es troba l\'entitat: ' .. _name)
  end
end

function deactivate_entity(_name)
  local l_entity = EM:get_entity(_name)
  if l_entity then
    l_entity:set_active(false)
  else
    log('Warning: no es troba l\'entitat: ' .. _name)
  end
end

function pick_up(_pickup, _player, _pickupName)
  local l_message = EM:get_event()

  l_message.msg = Event.pickup
  l_message.receiver = _player:get_guid()
  l_message.sender = _pickup:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str(_pickupName)
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  _pickup:get_component(BaseComponent.renderable_object).remove_renderable_object = true
  
  log("Agafant : " .. _pickupName)
  
  EM:remove_entity(_pickup)
end

function bido_toxic(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    local l_position = l_o3d:get_position()
    EM:init_trigger_with_emiter("bubble", l_position, Vect3f(2.0, 3.0, 2.0), get_collision_group("trigger"), "triggerBidoToxic", "", 10, Vect3f(0,1,0))
  end
end

function bido_explosiu(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  local l_exp = _self:get_component(BaseComponent.explosive)
  if l_o3d then
    local l_position = l_o3d:get_position()
    l_exp:explode(l_position,4)
    local l_explosion = EM:init_emiter("explosio foc", l_position+Vect3f(0, 1.0, 0), Vect3f(1.5, 1.5, 1.5), 1.5, Vect3f(0,1,0))
    ComponentOmni.add_to_entity(l_explosion, Vect3f(0.0, 0.0, 0.0), Color(2.0,2.0,2.0, 1.0), 3, 10,"llum_bido_explosiu_update")
    
    --SOUND:play_sample_3D('explosion', l_position)
    SOUND:play_sample('explosion')
  end
end

function granada(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  local l_exp = _self:get_component(BaseComponent.explosive)
  local l_render = _self:get_component(BaseComponent.renderable_object)
  local l_sphere = _self:get_component(BaseComponent.physx_sphere)
  local l_lifetime = _self:get_component(BaseComponent.life_time)
  local l_omni = _self:get_component(BaseComponent.omni)
  if l_o3d then
    local l_position = l_o3d:get_position()
    l_exp:explode(l_position,4)
	--l_lifetime.kill_entity = false
	l_lifetime.target_time = l_lifetime.target_time + 0.5
	
	
    local l_explosion = EM:init_emiter("explosio foc", l_position+Vect3f(0, 1.0, 0), Vect3f(1.5, 1.5, 1.5), 1.5, Vect3f(0,1,0))
    ComponentOmni.add_to_entity(l_explosion, Vect3f(0.0, 0.0, 0.0), Color(2.0,2.0,2.0, 1.0), 3, 10,"llum_bido_explosiu_update")
	
	l_render:set_active(false)
	l_exp:set_active(false)
	l_omni:set_active(false)
	l_sphere:set_active(false)
    
    SOUND:play_sample('explosion')
  end
end

function llum_bido_explosiu_update(_Entity, delta)
  local l_life_time = _Entity:get_component(BaseComponent.life_time)
  
  if l_life_time then
  
    if l_life_time.time > 0.1 then
      local l_omni_component = _Entity:get_component(BaseComponent.omni)
      l_omni_component:set_active(false)
      
      --local l_omni_light = l_omni_component:get_omni_light()
      
      --local l_end_att = l_omni_light:get_end_att()
      
      --l_omni_light:set_end_att(l_end_att - 3*delta)
      
    end
    
  end
  
end

-------------------------------------- NIVELL PROVES  -------------------------------------------

function pick_up_tetera(_trigger, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_trigger, _actor, "proves_TeteraPickUp")
  end
  test()
end

function esfera_interactiva(_self, _player)
  if _player:get_name() == "Player" then
    local player_controller = _player:get_component(BaseComponent.player_controller)
    
    if player_controller:has_pickup("proves_TeteraPickUp") then
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
  log('Sóc ' .. _self:get_name() .. ' i m\'ha matat ')
end

--prova d'explosio
function prova_explosio(_self)  
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    EM:init_trigger_with_emiter("bubble", l_o3d:get_position(), Vect3f(2.0, 3.0, 2.0), get_collision_group("trigger"), "triggerDeProvesEntrar", "triggerDeProvesSortir", 10, Vect3f(0,1,0))
  end
end

--prova drop tetera
function drop_tetera(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    log('estic creant una tetera')
    EM:init_pickup("tetera_" .. _self:get_name(), "proves_TeteraPickUp", l_o3d:get_position(), "pick_up_tetera")
  end
end

-------------------------------------------- LEVEL -2 -------------------------------------------
function laboratori_petar_llum_porta(_self)
  local l_door = EM:get_entity("Porta_Laboratori")
  if l_door then
    local l_message = EM:get_event()

    l_message.msg = Event.obrir
    l_message.receiver = l_door:get_guid()
    l_message.sender = _self:get_guid()
    l_message.dispatch_time = 0
    
    EM:send_event(l_message)
    
    --Apagar llums
    local l_light = LM:get_resource("lvl2_lab_Spot05")
    if l_light then
      l_light:set_active(false)
    else
      log("No es troba lvl2_lab_Spot05")
    end
    
    local l_light = LM:get_resource("lvl2_lab_Omni03")
    if l_light then
      l_light:set_active(false)
    else
      log("No es troba lvl2_lab_Omni03")
    end
  else
    log('error, no es troba la porta')
  end
end

function salavideo_palanca(_self, _player)
  if _player:get_name() == "Player" then    
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
    
    --activar el miner del passadís
    activate_entity("pas_miner00")
      
    _self:delete_component(BaseComponent.interactive)
  end
end

function recollir_arma(_self, _player)
  if _player:get_name() == "Player" then 
    _player:get_component(BaseComponent.player_controller).shoot_active = true
    _self:delete_component(BaseComponent.interactive)
    ComponentArma.add_to_entity(_player,"ARMA")
  end
end

function lvl2_lab_update_light(_entity, _dt)

  local l_random = math.random()
  
  local l_light = LM:get_resource("lvl2_lab_Spot05")
  local l_light2 = LM:get_resource("lvl2_lab_Omni03")
  
  if l_light and l_light2 then
  
    if l_random > 0.5 then
      l_light:set_active(false)
      l_light2:set_active(false)
    else
      l_light:set_active(true)
      l_light2:set_active(true)
    end

  end
    
end

------------------------------------------- LEVEL -1 ------------------------------------------

function lvl1_maquina_claus(_self, _player)
  log('usant màquina')
end

-- CLAUS
function get_key_green(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_green")
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
    
    _actor:get_component(BaseComponent.player_controller).force_active = true
  end
end

function get_key_yellow(_self, _actor)
  if _actor:get_name() == "Player" then
    pick_up(_self, _actor, "lvl1_key_yellow")
    --obrim la porta del passadís
    send_open_door("lvl1_door01", _actor)
    activate_entity("lvl1_militar10")
    activate_entity("lvl1_militar11")
    activate_entity("lvl1_militar12")
    activate_entity("lvl1_militar13")
    activate_entity("lvl1_miner06")
    --obrim la porta de claus
    send_open_door("lvl1_door_claus", _actor)
  end
end

-- OBRIR PORTES
function validate_key(_self, _player)
  if _player:get_name() == "Player" then
    local l_name = _self:get_name()
    
    if l_name == "lvl1_unlock_green" then
      if unlock(_player, "lvl1_key_green", "lvl1_door03") then
        _self:delete_component(BaseComponent.interactive)
      end

    elseif l_name == "lvl1_unlock_blue" then
      if unlock(_player, "lvl1_key_blue", "lvl1_door04") then
        _self:delete_component(BaseComponent.interactive)
      end
    
    elseif l_name == "lvl1_unlock_purple" then
      if unlock(_player, "lvl1_key_purple", "lvl1_door01") then
        _self:delete_component(BaseComponent.interactive)
      end
    
    elseif l_name == "lvl1_unlock_yellow" then
      if unlock(_player, "lvl1_key_yellow", "lvl1_door_final") then
        activate_entity("lvl1_miner07")
        _self:delete_component(BaseComponent.interactive)
      end
    end
  end
end

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

--other
function drop_key(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    if _self:get_name() == "lvl1_miner_green" then
      EM:init_pickup("lvl1_key_green", "lvl1_key_green", l_o3d:get_position(), "get_key_green")
    end
  end
end

-------------------------------------------- HANGAR -------------------------------------------
function hang_porta(_self, _player)
  if _player:get_name() == "Player" then
    activate_entity('EnemySpawner01')
  local l_spawner = EM:get_entity('EnemySpawner01')
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP:set_enemy_type(ComponentSpawner.enemy_miner)
    else
      log("Error: no es troba el component spawner!")
    end
  end
  
    activate_entity('EnemySpawner02')
    activate_entity('EnemySpawner03')
    
    --TODO: obrir porta, finalitzar pantalla.
  end
end