-------------------------------------------- FUNCIONS  -------------------------------------------
function riggle_file(_self, _player)
  if _player:get_name() == "Player" then
  
    --activar billboard de l'arma
    activate_entity("Lab_billboard_weapon")
    
    --crear interactuable per agafar l'arma
    local l_entity = EM:get_entity("lvl2_lab_Taula_DEF")
    if l_entity then
      local l_pCI = l_entity:get_component(BaseComponent.interactive)
      
      if l_pCI then
        -- ja estava creat, així es pot reproduir el tutorial sempre que es vulgui
      else
        --Afegir interactuable a l'arma
        l_pCI = ComponentInteractive.add_to_entity(l_entity, "recollir_arma")

        if l_pCI then
          l_pCI.billboard_y_offset = 0.6
        else
          log("ERROR: No s'ha pogut crear el component interactiu per a recollir_arma")
        end
      end
      
      --tutorial sobre el rigle
      tutorial_riggle()
      
    else
      log("ERROR: No es troba lvl2_lab_Taula_DEF")
    end

  end
end

function recollir_arma(_self, _player)
  if _player:get_name() == "Player" then 
    _player:get_component(BaseComponent.player_controller).shoot_active = true
    _self:delete_component(BaseComponent.interactive)
    ComponentArma.add_to_entity(_player,"ARMA")
    tutorial_weapon()
    activate_entity("Lab_billboard_shootme")
    deactivate_entity("Lab_billboard_weapon")
  end
end

function laboratori_obrir_porta(_self)
  deactivate_entity("Lab_billboard_shootme")
  
  local l_door = EM:get_entity("Porta_Laboratori")
  if l_door then
    local l_message = EM:get_event()

    l_message.msg = Event.obrir
    l_message.receiver = l_door:get_guid()
    l_message.sender = _self:get_guid()
    l_message.dispatch_time = 0
    
    EM:send_event(l_message)
    
    --Mostrar tutorial d'objectes destructibles
    tutorial_barrel()
    
    --Apagar llums
    --local l_light = LM:get_resource("lvl2_lab_Spot05")
    --if l_light then
    --  l_light:set_active(false)
    --else
    --  log("No es troba lvl2_lab_Spot05")
    --end
    --
    --local l_light = LM:get_resource("lvl2_lab_Omni03")
    --if l_light then
    --  l_light:set_active(false)
    --else
    --  log("No es troba lvl2_lab_Omni03")
    --end
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
      
      --desactivar billboard
      deactivate_entity("lvl2_video_billboard_laseroff")
    else
      log('error, no es troba la porta')
    end
    
    
    activate_cynematic_camera('lvl2_pass_CameraPassadis')
    
    ComponentDelayedScript.add_to_entity(_self, 4, 'lvl2_deactivate_camera')
    RENDERER:deactivate_render_path('aim_gui')
      
    _player:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Inactiu')
    
      
    _self:delete_component(BaseComponent.interactive)
  end
end

function lvl2_deactivate_camera(_self)

  --activar el miner del passadís
  activate_entity("pas_miner00")
  
  
  deactivate_cynematic_camera()
  RENDERER:activate_render_path('aim_gui')
  
  local l_player = EM:get_entity("Player")
  if l_player then
    l_player:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Neutre')
  else
      log('error, no es troba el player')
  end
  --_self:delete_component(BaseComponent.delayed_script)
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
--Trigger del GET HERE
function lvl2_got_there(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
  
    local l_entity = EM:get_entity("lvl2_lab_Taula01")  
    if l_entity then
      --Afegir interactuable a la taula
      local l_pCI = ComponentInteractive.add_to_entity(l_entity, "riggle_file")

      if l_pCI then
        l_pCI.billboard_y_offset = 1.6
        
        --destruir billboard gethere
        l_entity = EM:get_entity("Lab_billboard_gethere")
        if l_entity then
          EM:remove_entity(l_entity)
        else
          log("WARNING: No s'ha pogut trobar el component Lab_billboard_gethere")
        end
        
        --tutorial sobre interactuables
        tutorial_interactive()
      else
        log("ERROR: No s'ha pogut crear el component interactiu per a riggle_file")
      end
    else
      log("ERROR: No es troba lvl2_lab_Taula01")
    end
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)  
  end
end

-- Tutorial de cobertures.
function trigger_cobertures(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    tutorial_cover()
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

-- Tutorial de barrils.
--function trigger_barrils(_EntityTrigger, _Entity)
--  if _Entity:get_name() == 'Player' then
--    tutorial_barrel()
--    
--    --destrucció del trigger
--    EM:remove_entity(_EntityTrigger)
--  end
--end

-- activar enemics menjador 1.
function activarMenjador01(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then 
    activate_entity('Militar_menjador00')
    activate_entity('Militar_menjador01')
    activate_entity('Militar_menjador07')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger) 
  end
end

-- activar enemics menjador 2.
function activarMenjador02(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador02')
    activate_entity('Militar_menjador03')
    activate_entity('Militar_menjador08')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger) 
  end
end

-- activar enemics menjador 3
function activarMenjador03(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador09')
    activate_entity('Militar_menjador10')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger) 
  end
end

-- activar enemics menjador 4
function activarMenjador04(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activate_entity('Militar_menjador05')
    activate_entity('Militar_menjador06')
    activate_entity('Miner_menjador00')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger) 
  end
end

--activar el montacàrregues
function activate_elevator_2(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then    
    --activar montacàrregues.
    local elevator = EM:get_entity("Plataforma")
    
    if elevator then
      --Càmera
      activate_cynematic_camera("lvl2_camera_elevator")
      
      --Desactivo player
      _Entity:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Inactiu')
      
      --Missatge
      local l_message = EM:get_event()

      l_message.msg = Event.obrir
      l_message.sender = elevator:get_guid()
      l_message.receiver = elevator:get_guid()
      l_message.dispatch_time = 0
      
      EM:send_event(l_message)
    else
      log('Error: "Plataforma" no trobada.')
    end
    
    EM:remove_entity(_EntityTrigger)
  end
end

--canviar de nivell
function change_level_level_2(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    deactivate_cynematic_camera()
    
    set_new_level("Nivell -1")
  end
end