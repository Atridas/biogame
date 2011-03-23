function load_level(_szLevel)
  
  if(_szLevel == "") then
    l_szLevelPath = 'Data/XML/'
  else
    l_szLevelPath = 'Data/Levels/' .. _szLevel .. '/XML/'
  end
  
  STATIC_MESH_MANAGER:add_file(l_szLevelPath .. 'StaticMeshes.xml')
  STATIC_MESH_MANAGER:reload()
  
  RENDERABLE_OBJECTS_MANAGER:reload(l_szLevelPath .. 'RenderableObjects.xml')
end

function gui_exit_game()
  ENGINE:set_exit(true)
end

function gui_init_game()
  --GUI_MANAGER:setVisibleGuiElement('Play', false)
  --GUI_MANAGER:setVisibleGuiElement('Exit', false)
  
  --GUI_MANAGER:setImage('Menu_Background', 'img2')
  
  load_level('Laboratori')
  
  GUI_MANAGER:activeWindows('Game.xml')
end