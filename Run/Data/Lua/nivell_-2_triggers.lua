-- activar enemics menjador 1
function activarMenjador01(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activar_enemic('Militar_menjador00')
    activar_enemic('Militar_menjador01')
    activar_enemic('Militar_menjador07')
  end
end

-- activar enemics menjador 2
function activarMenjador02(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activar_enemic('Militar_menjador02')
    activar_enemic('Militar_menjador03')
    activar_enemic('Militar_menjador08')
  end
end

-- activar enemics menjador 3
function activarMenjador03(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activar_enemic('Militar_menjador09')
    activar_enemic('Militar_menjador10')
  end
end

-- activar enemics menjador 4
function activarMenjador04(_EntityTrigger, _Entity)
  if _Entity:get_name() == 'Player' then
    activar_enemic('Militar_menjador05')
    activar_enemic('Militar_menjador06')
    
    activar_enemic('Miner_menjador00')
  end
end