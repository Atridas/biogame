-- aplica un DoT a qui entri
function triggerBidoToxic(_EntityTrigger, _Entity)
  --aplicar un DoT
  local l_vida = _Entity:get_component(BaseComponent.vida)
  if l_vida then
    l_vida:add_dot(15.0,2.0)
  end
end