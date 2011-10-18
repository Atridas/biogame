-- NIVELL DE PROVES
function tutorial_proves()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 2
      Main_Menu_Constants["Frames"][0]     = "tutorial movement"
      Main_Menu_Constants["Frames"][1]     = "tutorial shoot"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end

-- PLANTA -2
function tutorial_movement()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial movement"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  --Desactivem els Billboards i objectes que inicialment no es poden usar
  deactivate_entity("Lab_billboard_weapon")
  deactivate_entity("Lab_billboard_shootme")
  
  --SOUND:stop_sounds()
end

function tutorial_interactive()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial interactive"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  --TODO: Activar Objecte interactuable File
  
  --SOUND:stop_sounds()
end

function tutorial_riggle()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 2
      Main_Menu_Constants["Frames"][0]     = "tutorial life"
      Main_Menu_Constants["Frames"][1]     = "tutorial energy"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  --TODO: Activar Objecte interactuable Arma i Billboard Gun
  
  --SOUND:stop_sounds()
end

function tutorial_weapon()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial shoot"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  --TODO: Activar Objecte destructible Caixa de llums i BillBoard ShootMe

  --SOUND:stop_sounds()
end

function tutorial_cover()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial cover"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end

function tutorial_barrel()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 2
      Main_Menu_Constants["Frames"][0]     = "tutorial barrel explosive"
      Main_Menu_Constants["Frames"][1]     = "tutorial barrel toxic"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end

-- PLANTA -1
function tutorial_key()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial key"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end

function tutorial_grenade()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial grenade"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end

function tutorial_force()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm and l_sm:get_state_machine():current_state() == 'State_Jugant' then
      Main_Menu_Constants["Total frames"]  = 1
      Main_Menu_Constants["Frames"][0]     = "tutorial force"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
  
  --SOUND:stop_sounds()
end