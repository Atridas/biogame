-- NIVELL DE PROVES
function tutorial_proves()
  local l_LvLC = EM:get_entity("LevelController")
  if l_LvLC then
    local l_sm = l_LvLC:get_component(BaseComponent.state_machine)
    if l_sm then
      Main_Menu_Constants["Total frames"]  = 2
      Main_Menu_Constants["Frames"][0]     = "tutorial moviment"
      Main_Menu_Constants["Frames"][1]     = "tutorial dispar"
      l_sm:get_state_machine():change_state('State_Tutorial')
    end
  end
end