

-- entres al trigger
function triggerDeProvesEntrar(_EntityTrigger, _Entity)
  log('entrar ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
end

-- surts del trigger
function triggerDeProvesSortir(_EntityTrigger, _Entity)
  log('sortir ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
  
  local l_spawner = EM:get_entity("EnemySpawner")
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP.active = false
    else
      log("Error: no es troba el component spawner!")
    end
  else
    log("Error: no es troba l'entitat EnemySpawner!")
  end
end

--activar el militar
function activarEnemic(_trigger, _entity)
  local l_miner = EM:get_entity("Miner")
  if l_miner then
    local l_SM = l_miner:get_component(BaseComponent.state_machine)
    if l_SM then
      l_SM:set_active(true)
    else
      log('El miner no té màquina d\'estats')
    end
  else
    log('Error: no es troba el miner!!!!')
  end
  
  local l_spawner = EM:get_entity("EnemySpawner")
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP.active = true
      l_SP:set_enemy_type(ComponentSpawner.enemy_miner)
    else
      log("Error: no es troba el component spawner!")
    end
  else
    log("Error: no es troba l'entitat EnemySpawner!")
  end
end

--activar l'altre
function activarMilitar(_trigger, _entity)
  local l_militar = EM:get_entity("Militar")
  if l_militar then
    local l_SM = l_militar:get_component(BaseComponent.state_machine)
    if l_SM then
      l_SM:set_active(true)
    else
      log('El militar no té màquina d\'estats')
    end
  else
    log('Error: no es troba el militar!!!!')
  end
  
  local l_spawner = EM:get_entity("EnemySpawner")
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP.active = true
      l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
    else
      log("Error: no es troba el component spawner!")
    end
  else
    log("Error: no es troba l'entitat EnemySpawner!")
  end
end