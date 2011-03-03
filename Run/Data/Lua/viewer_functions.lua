

function load_level(level)
  
  static_mesh_manager = CORE:get_static_mesh_manager()
  light_manager       = CORE:get_light_manager()
  
  static_mesh_manager:add_file('Data/Levels/' .. level .. '/XML/StaticMeshes.xml')
  static_mesh_manager:reload()
  
  light_manager:load('Data/Levels/' .. level .. '/XML/Lights.xml')
  
  ROM:reload('Data/Levels/' .. level .. '/XML/RenderableObjects.xml')
end