--canviar de nivell
function change_level_level_1(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    set_new_level("Hangar")
  end
end

--magatzem
function lvl1_trigger_magatzem01(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_purple") then
    activate_entity("lvl1_miner02")
    activate_entity("lvl1_militar07")
    activate_entity("lvl1_militar08")
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
    end
  end
end

function lvl1_trigger_pass_mag(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    local player_controller = _Entity:get_component(BaseComponent.player_controller)
    if player_controller:has_pickup("lvl1_key_purple") then
      activate_entity("lvl1_miner01")
      activate_entity("lvl1_militar03")
      activate_entity("lvl1_militar04")
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
    end
  end
end