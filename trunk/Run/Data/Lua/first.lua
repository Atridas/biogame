function creaBracet()
  for i = 1, 10 do
    local l_vPos = Vect3f(math.random(-15,15), 0.0, math.random(-15,15))
    local l_pStaticModel = RENDERABLE_OBJECTS_MANAGER:add_static("braç","bracet_"..g_ibracetCount)
    g_ibracetCount = g_ibracetCount + 1
    l_pStaticModel:set_position(l_vPos)
  end
end