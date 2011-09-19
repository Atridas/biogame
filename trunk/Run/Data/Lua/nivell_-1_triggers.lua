--canviar de nivell
function change_level_level_1(_EntityTrigger, _Entity)
  if _Entity:get_name() == "Player" then
    set_new_level("Hangar")
  end
end