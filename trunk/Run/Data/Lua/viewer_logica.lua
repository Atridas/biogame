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

--selecciona el seg�ent mode
function set_next_mode()
  local l_iState = VIEWER:get_current_mode()
  
  VIEWER:next_mode()
  switch_view(l_iState, VIEWER:get_current_mode())
end

--selecciona un mode concret
function set_view(l_iNewState)
  local l_iState = VIEWER:get_current_mode()
  switch_view(l_iState, l_iNewState)
end

--canvia a la seg�ent vista i actualitza la GUI
function switch_view(_old, _new)
  deactivate_view(_old)
  VIEWER:set_mode(_new)
  show_gui()
end

--seg�ent mesh o animated model
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

--seg�ent animaci�
function next_animation()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.ANIMATED then
    VIEWER:next_animation()
  end
end

--anterior animaci�
function previous_animation()
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.ANIMATED then
    VIEWER:previous_animation()
  end
end