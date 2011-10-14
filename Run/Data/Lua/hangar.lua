-------------------------------------------- FUNCIONS  -------------------------------------------
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
    
    --TODO: obrir porta, finalitzar pantalla.
    
    _self:delete_component(BaseComponent.interactive)
  end
end

-------------------------------------------- TRIGGERS  -------------------------------------------
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
    activate_entity('Militar02')
    activate_entity('Militar09')
    activate_entity('Militar10')
    activate_entity('Militar14')
    activate_entity('Militar15')
    
    --upper
    activate_entity('Militar19')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
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
    activate_entity('Militar16')
    
    --upper
    activate_entity('Militar20')
    activate_entity('Militar21')
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
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
    
    --destrucció del trigger
    EM:remove_entity(_EntityTrigger)
  end
end