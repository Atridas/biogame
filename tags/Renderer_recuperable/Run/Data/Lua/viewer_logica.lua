--carrega un nivell donat a la textbox
function load_level(_szLevel)
  
  if(_szLevel == "") then
    l_szLevelPath = 'Data/XML/'
  else
    l_szLevelPath = 'Data/Levels/' .. _szLevel .. '/XML/'
  end
  
  STATIC_MESH_MANAGER:add_file(l_szLevelPath .. 'StaticMeshes.xml')
  STATIC_MESH_MANAGER:reload()
  
  RENDERABLE_OBJECTS_MANAGER:reload(l_szLevelPath .. 'RenderableObjects.xml')
  
  VIEWER_PROCESS:reset_viewer()
  hide_all()
end

--selecciona el següent mode
function set_next_mode()
  local l_iState = VIEWER:get_current_mode()
  
  VIEWER:next_mode()
  
  if g_toggleGui then
    switch_view(l_iState, VIEWER:get_current_mode())
  end
end

--selecciona un mode concret
function set_view(l_iNewState)
  local l_iState = VIEWER:get_current_mode()
  switch_view(l_iState, l_iNewState)
end

--canvia a la següent vista i actualitza la GUI
function switch_view(_old, _new)
  deactivate_view(_old)
  VIEWER:set_mode(_new)
  show_gui()
end

--Puja el nivell de glow de l'objecte actual
function glow_up()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:glow_inc_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:glow_inc_anim()
  end
end

--Disminueix el nivell de glow de l'objecte actual
function glow_down()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:glow_dec_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:glow_dec_anim()
  end
end

--Puja el nivell d'especular de l'objecte actual
function spec_up()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:spec_inc_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:spec_inc_anim()
  end
end

--Disminueix el nivell d'especular de l'objecte actual
function spec_down()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:spec_dec_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:spec_dec_anim()
  end
end

--Puja el nivell de glossiness de l'objecte actual
function gloss_up()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:gloss_inc_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:gloss_inc_anim()
  end
end

--Disminueix el nivell de glossiness de l'objecte actual
function gloss_down()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:gloss_dec_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:gloss_dec_anim()
  end
end

--Puja el nivell de bump de l'objecte actual
function bump_up()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:bump_inc_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:bump_inc_anim()
  end
end

--Puja el nivell de bump de l'objecte actual
function bump_down()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:bump_dec_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:bump_dec_anim()
  end
end

--següent mesh o animated model
function next()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:next_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:next_model()
  end
end

--anterior mesh o animated model
function previous()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.MESH then
    VIEWER:previous_mesh()
  elseif l_iState == Viewer.ANIMATED then
    VIEWER:previous_model()
  end
end

--següent animació
function next_animation()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.ANIMATED then
    VIEWER:next_animation()
  end
end

--anterior animació
function previous_animation()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.ANIMATED then
    VIEWER:previous_animation()
  end
end