

function load_level(level)
  
  STATIC_MESH_MANAGER:add_file('Data/Levels/' .. level .. '/XML/StaticMeshes.xml')
  STATIC_MESH_MANAGER:reload()
  
  --LIGHT_MANAGER:load('Data/Levels/' .. level .. '/XML/Lights.xml')
  
  RENDERABLE_OBJECTS_MANAGER:reload('Data/Levels/' .. level .. '/XML/RenderableObjects.xml')
  
  ENGINE:get_active_process():reset_viewer()
end