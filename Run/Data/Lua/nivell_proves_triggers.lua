

-- entres al trigger
function triggerDeProvesEntrar(_EntityTrigger, _Entity)
  log('entrar ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
end

-- surts del trigger
function triggerDeProvesSortir(_EntityTrigger, _Entity)
  log('sortir ' .. _EntityTrigger:get_name() .. ' amb ' .. _Entity:get_name())
end