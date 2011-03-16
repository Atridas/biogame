--activa o desactiva la GUI
function toggle_gui()  
  if VIEWER:get_view_mode() then
    show_gui()
  else
    hide_gui()
  end
end

--desactiva la GUI
function hide_gui()
  GUI_MANAGER:setVisiblePointerMouse(false)
  GUI_MANAGER:activeWindows('Main.xml')
  VIEWER:set_view_mode(true)
end

--activa la GUI a l'estat que toca
function show_gui()
  VIEWER:set_view_mode(false)
  GUI_MANAGER:setVisiblePointerMouse(true)
  
  -- captura del mode
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.FREE then
    GUI_MANAGER:activeWindows('Main.xml')
    GUI_MANAGER:activeWindows('Free.xml')
  elseif l_iState == Viewer.MESH then
    GUI_MANAGER:activeWindows('Main.xml')
    GUI_MANAGER:activeWindows('Mesh.xml')
  elseif l_iState == Viewer.ANIMATED then
    GUI_MANAGER:activeWindows('Main.xml')
    GUI_MANAGER:activeWindows('Animated.xml')
  end
end

--carrega un nivell
function load_level(level)
  
  STATIC_MESH_MANAGER:add_file('Data/Levels/' .. level .. '/XML/StaticMeshes.xml')
  STATIC_MESH_MANAGER:reload()
  
  RENDERABLE_OBJECTS_MANAGER:reload('Data/Levels/' .. level .. '/XML/RenderableObjects.xml')
  
  VIEWER_PROCESS:reset_viewer()
end