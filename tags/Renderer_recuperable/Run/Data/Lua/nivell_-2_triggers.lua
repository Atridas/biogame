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