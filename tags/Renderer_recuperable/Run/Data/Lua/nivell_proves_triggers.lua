

-- entres al trigger
function triggerDeProvesEntrar(_EntityTrigger, _Entity)
  log('entrar ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
  
  --aplicar un DoT
  local l_vida = _Entity:get_component(BaseComponent.vida)
  if l_vida then
    l_vida:add_dot(15.0,2.0)
  end
end

-- surts del trigger
function triggerDeProvesSortir(_EntityTrigger, _Entity)
  log('sortir ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
  
  deactivate_entity("EnemySpawner")
  
  --aplicar un HoT
  local l_vida = _Entity:get_component(BaseComponent.vida)
  if l_vida then
    l_vida:add_hot(15.0,2.0)
  end
end

--activar el miner
function activarEnemic(_trigger, _entity)
  activate_entity("Miner")
  activate_entity("EnemySpawner")
  
  local l_spawner = EM:get_entity("EnemySpawner")
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP:set_enemy_type(ComponentSpawner.enemy_miner)
    else
      log("Error: no es troba el component spawner!")
    end
  end
end

--activar l'altre
function activarMilitar(_trigger, _entity)
  activate_entity("Militar")
  activate_entity("EnemySpawner")
  
  local l_spawner = EM:get_entity("EnemySpawner")
  if l_spawner then
    local l_SP = l_spawner:get_component(BaseComponent.spawner)
    if l_SP then
      l_SP:set_enemy_type(ComponentSpawner.enemy_militar)
    else
      log("Error: no es troba el component spawner!")
    end
  end
  
end