-------------------------------------------- FUNCIONS  -------------------------------------------
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

-------------------------------------------- TRIGGERS  -------------------------------------------
-- activar enemics menjador 1
function activarMenjador01(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador00')
    activate_entity('Militar_menjador01')
    activate_entity('Militar_menjador07')
  end
end

-- activar enemics menjador 2
function activarMenjador02(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador02')
    activate_entity('Militar_menjador03')
    activate_entity('Militar_menjador08')
  end
end

-- activar enemics menjador 3
function activarMenjador03(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador09')
    activate_entity('Militar_menjador10')
  end
end

-- activar enemics menjador 4
function activarMenjador04(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador05')
    activate_entity('Militar_menjador06')
    activate_entity('Miner_menjador00')
  end
end

--canviar de nivell
function change_level_level_2(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    set_new_level("Nivell -1")
  end
end