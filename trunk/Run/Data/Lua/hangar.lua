-------------------------------------------- FUNCIONS  -------------------------------------------
function hang_porta(_self, _player)
  if _player:get_name() == "Player" then
    
    activate_entity('EnemySpawner01')
    local l_spawner = EM:get_entity('EnemySpawner01')
    if l_spawner then
      local l_SP = l_spawner:get_component(BaseComponent.spawner)
      if l_SP then
        l_SP:set_enemy_type(ComponentSpawner.enemy_miner)
      else
        log("Error: no es troba el component spawner 01!")
      end
    end
    
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
    
    --TODO: obrir porta, finalitzar pantalla.
    
    _self:delete_component(BaseComponent.interactive)
  end
end

-------------------------------------------- TRIGGERS  -------------------------------------------
function hang_trig_enemics_enter00(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner01')
  end
end

function hang_trig_enemics_enter01(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner03')
    activate_entity('Miner04')
  end
end

function hang_trig_enemics_enter02(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner02')
  end
end

function hang_trig_enemics_enter03(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner05')
    activate_entity('Miner09')
  end
end

function hang_trig_enemics_enter04(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner06')
  end
end

function hang_trig_enemics_enter05(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner07')
    activate_entity('Militar01')
    activate_entity('Militar02')
    activate_entity('Militar09')
    activate_entity('Militar10')
  end
end

function hang_trig_enemics_enter06(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Militar03')
    activate_entity('Militar04')
    activate_entity('Militar05')
    activate_entity('Militar06')
    activate_entity('Militar07')
    activate_entity('Militar08')
  end
end

function hang_trig_enemics_enter07(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    activate_entity('Miner08')
    activate_entity('Militar11')
    activate_entity('Militar12')
    activate_entity('Militar13')
  end
end