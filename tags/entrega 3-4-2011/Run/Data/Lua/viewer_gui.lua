--activa o desactiva la GUI
function toggle_gui()  
  if g_toggleGui == false then
	g_toggleGui = true
    show_gui()
  else
	g_toggleGui = false
    hide_gui()
  end

end

--desactiva la GUI
function hide_gui()
  GUI_MANAGER:setVisiblePointerMouse(false)
  VIEWER:activate_gui(false)
  
  deactivate_view(VIEWER:get_current_mode())
end

--activa la GUI a l'estat que toca
function show_gui()
  VIEWER:activate_gui(true)
  GUI_MANAGER:setVisiblePointerMouse(true)
  
  show_common()
  
  -- captura del mode
  local l_iState = VIEWER:get_current_mode()
  
  if l_iState == Viewer.FREE then
    show_free()
  elseif l_iState == Viewer.MESH then
    show_mesh()
  elseif l_iState == Viewer.ANIMATED then
    show_animated()
  end
end

--desactiva una vista indicada
function deactivate_view(_old)

  hide_common()
  
  if _old == Viewer.FREE then
    hide_free()
  elseif _old == Viewer.MESH then
    hide_mesh()
  elseif _old == Viewer.ANIMATED then
    hide_animated()
  end
  
end

--mostra els elements comuns de la interficie
function show_common()
  GUI_MANAGER:setActiveGuiElement( 'FreeMode',true)
  GUI_MANAGER:setVisibleGuiElement('FreeMode',true)
  
  GUI_MANAGER:setActiveGuiElement( 'MeshMode',true)
  GUI_MANAGER:setVisibleGuiElement('MeshMode',true)
  
  GUI_MANAGER:setActiveGuiElement( 'AnimatedMode',true)
  GUI_MANAGER:setVisibleGuiElement('AnimatedMode',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Normals',true)
  GUI_MANAGER:setVisibleGuiElement('Normals',true)
end

--amaga els elements comuns de la interficie
function hide_common()
  GUI_MANAGER:setActiveGuiElement( 'FreeMode',false)
  GUI_MANAGER:setVisibleGuiElement('FreeMode',false)
  
  GUI_MANAGER:setActiveGuiElement( 'MeshMode',false)
  GUI_MANAGER:setVisibleGuiElement('MeshMode',false)
  
  GUI_MANAGER:setActiveGuiElement( 'AnimatedMode',false)
  GUI_MANAGER:setVisibleGuiElement('AnimatedMode',false)

  GUI_MANAGER:setActiveGuiElement( 'Normals',false)
  GUI_MANAGER:setVisibleGuiElement('Normals',false)
end

--mostra els elements free de la interficie
function show_free()
end

--amaga els elements free de la interficie
function hide_free()
end

--mostra els elements mesh de la interficie
function show_mesh()
  GUI_MANAGER:setActiveGuiElement( 'Box',true)
  GUI_MANAGER:setVisibleGuiElement('Box',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Sphere',true)
  GUI_MANAGER:setVisibleGuiElement('Sphere',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Previous',true)
  GUI_MANAGER:setVisibleGuiElement('Previous',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Next',true)
  GUI_MANAGER:setVisibleGuiElement('Next',true)
end

--amaga els elements mesh de la interficie
function hide_mesh()
  GUI_MANAGER:setActiveGuiElement( 'Box',false)
  GUI_MANAGER:setVisibleGuiElement('Box',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Sphere',false)
  GUI_MANAGER:setVisibleGuiElement('Sphere',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Previous',false)
  GUI_MANAGER:setVisibleGuiElement('Previous',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Next',false)
  GUI_MANAGER:setVisibleGuiElement('Next',false)
end

--mostra els elements animated de la interficie
function show_animated()
  GUI_MANAGER:setActiveGuiElement( 'Box',true)
  GUI_MANAGER:setVisibleGuiElement('Box',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Sphere',true)
  GUI_MANAGER:setVisibleGuiElement('Sphere',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Previous',true)
  GUI_MANAGER:setVisibleGuiElement('Previous',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Next',true)
  GUI_MANAGER:setVisibleGuiElement('Next',true)

  GUI_MANAGER:setActiveGuiElement( 'Previous_Action',true)
  GUI_MANAGER:setVisibleGuiElement('Previous_Action',true)
  
  GUI_MANAGER:setActiveGuiElement( 'Next_Action',true)
  GUI_MANAGER:setVisibleGuiElement('Next_Action',true)
end

--amaga els elements animated de la interficie
function hide_animated()
  GUI_MANAGER:setActiveGuiElement( 'Box',false)
  GUI_MANAGER:setVisibleGuiElement('Box',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Sphere',false)
  GUI_MANAGER:setVisibleGuiElement('Sphere',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Previous',false)
  GUI_MANAGER:setVisibleGuiElement('Previous',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Next',false)
  GUI_MANAGER:setVisibleGuiElement('Next',false)

  GUI_MANAGER:setActiveGuiElement( 'Previous_Action',false)
  GUI_MANAGER:setVisibleGuiElement('Previous_Action',false)
  
  GUI_MANAGER:setActiveGuiElement( 'Next_Action',false)
  GUI_MANAGER:setVisibleGuiElement('Next_Action',false)
end

--amaga tots els elements de la GUI
function hide_all()
  hide_common()
  hide_free()
  hide_mesh()
  hide_animated()
  GUI_MANAGER:setVisiblePointerMouse(false)
end