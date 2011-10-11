-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Configuració! --------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

Main_Menu_Constants = {}


Main_Menu_Constants["Renderpath inici"] = "Pantalla d'inici"
Main_Menu_Constants["Renderpath howto"] = "howto"
Main_Menu_Constants["Renderpath credits"] = "credits"

Main_Menu_Constants["Total frames"] = 0
Main_Menu_Constants["Current frame"] = 0
Main_Menu_Constants["Frames"]  = {}


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Definició d'estats ---------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
State_Main_Menu_Inici    = {}

State_Main_Menu_Howto    = {}

State_Main_Menu_Credits  = {}

State_Jugant  = {}

State_Paused  = {}

State_Tutorial = {}

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- Inici!!!! ------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Main_Menu_Inici["Enter"] = function(_self)
  RENDERER:set_unique_render_path(Main_Menu_Constants["Renderpath inici"])
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Inici["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Inici['Update'] = function(_self, _dt)

  if ACTION_MANAGER:is_action_active('Init -2') or ACTION_MANAGER:is_action_active('Intro') then
    RENDERER:set_unique_render_path("HDR")
    RENDERER:activate_render_path("Antialiasing")
    set_new_level("Nivell -2")
    return
  elseif ACTION_MANAGER:is_action_active('Init -1') then
    RENDERER:set_unique_render_path("HDR")
    RENDERER:activate_render_path("Antialiasing")
    set_new_level("Nivell -1")
    return
  elseif ACTION_MANAGER:is_action_active('Init Hangar') then
    RENDERER:set_unique_render_path("HDR")
    RENDERER:activate_render_path("Antialiasing")
    set_new_level("Hangar")
    return
    
    
  elseif ACTION_MANAGER:is_action_active('Howto') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Howto')
    return
  elseif ACTION_MANAGER:is_action_active('Credits') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Credits')
    return
    
    
  elseif ACTION_MANAGER:is_action_active('Escape') then
    CORE:set_exit(true)
    return
  end
  
  
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Inici['Receive'] = function(_self, _event)

end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- howto!!!! ------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Main_Menu_Howto["Enter"] = function(_self)
  RENDERER:set_unique_render_path(Main_Menu_Constants["Renderpath howto"])
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Howto["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Howto['Update'] = function(_self, _dt)

  if ACTION_MANAGER:is_action_active('Escape') or ACTION_MANAGER:is_action_active('Menu') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Inici')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Howto['Receive'] = function(_self, _event)

end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- credits!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Main_Menu_Credits["Enter"] = function(_self)
  RENDERER:set_unique_render_path(Main_Menu_Constants["Renderpath credits"])
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Credits["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Credits['Update'] = function(_self, _dt)

  if ACTION_MANAGER:is_action_active('Escape') or ACTION_MANAGER:is_action_active('Menu') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Inici')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Main_Menu_Credits['Receive'] = function(_self, _event)

end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- jugant!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Jugant["Enter"] = function(_self)
  CORE:set_pause(false)
end

-------------------------------------------------------------------------------------------------
State_Jugant["Exit"] = function(_self)
  
end

-------------------------------------------------------------------------------------------------
State_Jugant['Update'] = function(_self, _dt)

  if ACTION_MANAGER:is_action_active('Escape') or ACTION_MANAGER:is_action_active('Pause') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Paused')
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Jugant['Receive'] = function(_self, _event)

end

-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- jugant!!!! ----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Paused["Enter"] = function(_self)
  RENDERER:activate_render_path("Pause")
  CORE:set_pause(true)
end

-------------------------------------------------------------------------------------------------
State_Paused["Exit"] = function(_self)
  RENDERER:deactivate_render_path("Pause")

end

-------------------------------------------------------------------------------------------------
State_Paused['Update'] = function(_self, _dt)

  if  ACTION_MANAGER:is_action_active('Menu')  then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Main_Menu_Inici')
    _self:get_component(BaseComponent.bgm):set_current_song(ComponentBGMController.init_menu, true)
    return
  elseif ACTION_MANAGER:is_action_active('Pause') or ACTION_MANAGER:is_action_active('Escape') then
    _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Jugant')
  end
  
end

-------------------------------------------------------------------------------------------------
State_Paused['Receive'] = function(_self, _event)

end


-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-- tutorial!!!! ---------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

State_Tutorial["Enter"] = function(_self)
  Main_Menu_Constants["Current frame"] = 0
  
  RENDERER:activate_render_path(Main_Menu_Constants["Frames"][0])
  
  CORE:set_pause(true)
end

-------------------------------------------------------------------------------------------------
State_Tutorial["Exit"] = function(_self)
  Main_Menu_Constants["Current frame"] = 0
  Main_Menu_Constants["Total frames"]  = 0
  Main_Menu_Constants["Frames"]        = {}
end

-------------------------------------------------------------------------------------------------
State_Tutorial['Update'] = function(_self, _dt)

  if ACTION_MANAGER:is_action_active('Shoot') then
    
    RENDERER:deactivate_render_path(Main_Menu_Constants["Frames"][Main_Menu_Constants["Current frame"]])
    Main_Menu_Constants["Current frame"] = Main_Menu_Constants["Current frame"] + 1
    
    if Main_Menu_Constants["Current frame"] < Main_Menu_Constants["Total frames"] then
      --següent imatge
      RENDERER:activate_render_path(Main_Menu_Constants["Frames"][Main_Menu_Constants["Current frame"]])
    else
      --fi del tutorial
      _self:get_component(BaseComponent.state_machine):get_state_machine():change_state('State_Jugant')
    end
    
    return
  end
  
end

-------------------------------------------------------------------------------------------------
State_Tutorial['Receive'] = function(_self, _event)

end
