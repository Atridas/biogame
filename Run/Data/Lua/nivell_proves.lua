-------------------------------------------- FUNCIONS  -------------------------------------------
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
  
  tutorial_proves()
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

-------------------------------------------- TRIGGERS  -------------------------------------------
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