-------------------------------------------- FUNCIONS  -------------------------------------------

function init_hangar()
  EFFECT_MANAGER:set_exposure(0)
  RENDERER:blend_parameter(Renderer.exposure, 0.5, 3)
  RENDERER:activate_render_path('hangar skybox')
  
  local elevator = EM:get_entity("hangar_Montacarregues")
  
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
    activate_cynematic_camera("hangar_elevator_camera")
    l_player:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Player_Inactiu')
    
    --EM:get_entity("lvl1_physXBox_elevator"):set_active(false)
	deactivate_entity("hangar_physx_box_elevator")
	
    --fade in
    EFFECT_MANAGER:set_exposure(0)
    RENDERER:deactivate_render_path("aim_gui")
    RENDERER:blend_parameter(Renderer.exposure, 0.5, 3)
  else
    log("Error: No es troba l'entitat: hangar_Montacarregues")
  end
end

function hang_porta(_self, _player)
  if _player:get_name() == "Player" then
  
    deactivate_entity('EnemySpawner01')
    activate_entity('EnemySpawner01')
    local l_spawner = EM:get_entity('EnemySpawner01')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
      else
        log("Error: no es troba el component spawner 01!")
      end
    end
    
    deactivate_entity('EnemySpawner02')
    activate_entity('EnemySpawner02')
    l_spawner = EM:get_entity('EnemySpawner02')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
      else
        log("Error: no es troba el component spawner 02!")
      end
    end
  
    deactivate_entity('EnemySpawner03')
    activate_entity('EnemySpawner03')
    l_spawner = EM:get_entity('EnemySpawner03')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
      else
        log("Error: no es troba el component spawner 03!")
      end
    end
    
    deactivate_entity('EnemySpawner04')
    activate_entity('EnemySpawner04')
    l_spawner = EM:get_entity('EnemySpawner04')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
      else
        log("Error: no es troba el component spawner 04!")
      end
    end
    
    deactivate_entity('EnemySpawner05')
    activate_entity('EnemySpawner05')
    l_spawner = EM:get_entity('EnemySpawner05')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
      else
        log("Error: no es troba el component spawner 05!")
      end
    end
    
    --obrir porta.
    local door = EM:get_entity("hangar_puerta_hangar")
    
    if door then
      --Missatge
      local l_message = EM:get_event()

      l_message.msg = Event.obrir
      l_message.sender = _self:get_guid()
      l_message.receiver = door:get_guid()
      l_message.dispatch_time = 0
      
      EM:send_event(l_message)
    else
      log('Error: "hangar_puerta_hangar" no trobada.')
    end
    
    --vigies
    activate_entity('Vigia08')
    activate_entity('Vigia09')
    activate_entity('Vigia10')
    activate_entity('Vigia11')
    activate_entity('Vigia12')
    activate_entity('Vigia13')
    activate_entity('Vigia14')
    activate_entity('Vigia15')
    
    
    _self:delete_component(BaseComponent.interactive)
  end
end

-------------------------------------------- TRIGGERS  -------------------------------------------
--aturar l'ascensor d'entrada
function hang_stop_elevator(_EntityTrigger, _Entity)
  log("CATACROQUER")
  if _Entity:get_name() == "Player" then
  
    local elevator = EM:get_entity("hangar_Montacarregues")
    
    if elevator then
      local pbox = EM:get_entity("hangar_physx_box_elevator")
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
      log("Error: No es troba l'entitat: hangar_Montacarregues")
    end
  end
end

function hang_trig_enemics_enter00(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner01')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_trig_enemics_enter01(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner03')
    --activate_entity('Miner04')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_trig_enemics_enter02(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner02')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_trig_enemics_enter03(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    --activate_entity('Miner05')
    activate_entity('Miner09')
    
    --upper
    activate_entity('Militar18')
    
    --vigies
    activate_entity('Vigia01')
    activate_entity('Vigia02')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_trig_enemics_enter04(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner06')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_trig_enemics_enter05(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    --activate_entity('Miner07')
    activate_entity('Militar01')
    activate_entity('Militar09')
    activate_entity('Militar14')
    activate_entity('Militar15')   
    
    --upper
    activate_entity('Militar19')
    
    --destrucció del trigger
    _EntityTrigger:delete_component(BaseComponent.trigger)
    
    ComponentDelayedScript.add_to_entity(_EntityTrigger, 2.0, "hang_enemics05")
  end
end

function hang_enemics05(_EntityTrigger)
  activate_entity('Militar02')
  activate_entity('Militar10')
  EM:remove_entity(_EntityTrigger)
end

function hang_trig_enemics_enter06(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Militar03')
    activate_entity('Militar05')
    
    --upper
    activate_entity('Militar20')
    activate_entity('Militar21')
    
    --destrucció del trigger
    _EntityTrigger:delete_component(BaseComponent.trigger)
    
    ComponentDelayedScript.add_to_entity(_EntityTrigger, 2.0, "hang_enemics06_1")
  end
end

function hang_enemics06_1(_EntityTrigger)
  activate_entity('Militar04')
  activate_entity('Militar06')
  _EntityTrigger:get_component(BaseComponent.delayed_script):reset(1.5, "hang_enemics06_2")
end

function hang_enemics06_2(_EntityTrigger)
  activate_entity('Militar16')
  _EntityTrigger:get_component(BaseComponent.delayed_script):reset(1.5, "hang_enemics06_3")
end

function hang_enemics06_3(_EntityTrigger)
  activate_entity('Militar07')
  _EntityTrigger:get_component(BaseComponent.delayed_script):reset(1.5, "hang_enemics06_4")
end

function hang_enemics06_4(_EntityTrigger)
  activate_entity('Militar08')
  EM:remove_entity(_EntityTrigger)
end

function hang_trig_enemics_enter07(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    --activate_entity('Miner08')
    activate_entity('Militar11')
    activate_entity('Militar12')
    activate_entity('Militar13')
    activate_entity('Militar17')
    
    --upper
    activate_entity('Militar22')
    activate_entity('Militar23')
    
    --vigies
    activate_entity('Vigia03')
    activate_entity('Vigia04')
    activate_entity('Vigia05')
    activate_entity('Vigia06')
    activate_entity('Vigia07')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end

function hang_final_nivell(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" and _Entity:get_component(BaseComponent.player_controller):is_alive() then
    god_mode(true)
    local controller = EM:get_entity("LevelController")
    if controller then
      CORE:set_pause(true)
      SOUND:stop_sounds()
      set_song(ComponentBGMController.init_menu)
      controller:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Credits')
    else
      log("ERROR: No es pot acabar el joc! No hi ha LevelController !!!!!")
    end
  end
end