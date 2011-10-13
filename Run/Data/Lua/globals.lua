g_ibracetCount = 0

-------------------------------------------- FUNCIONS  -------------------------------------------
function activate_entity(_name)
  local l_entity = EM:get_entity(_name)
  if l_entity then
    l_entity:set_active(true)
    --log("Activant l'entitat: " .. _name)
  else
    log('Warning: no es troba l\'entitat: ' .. _name)
  end
end

function deactivate_entity(_name)
  local l_entity = EM:get_entity(_name)
  if l_entity then
    l_entity:set_active(false)
  else
    log('Warning: no es troba l\'entitat: ' .. _name)
  end
end

function pick_up(_pickup, _player, _pickupName)
  local l_message = EM:get_event()

  l_message.msg = Event.pickup
  l_message.receiver = _player:get_guid()
  l_message.sender = _pickup:get_guid()
  l_message.dispatch_time = 0
  --Info
  local l_event = l_message:get_info(0)
  l_event.Type = EventInfo.string
  l_event:set_str(_pickupName)
  l_message:set_info(0, l_event)

  --Send
  EM:send_event(l_message)
  
  _pickup:get_component(BaseComponent.renderable_object).remove_renderable_object = true
  
  log("Agafant : " .. _pickupName)
  
  EM:remove_entity(_pickup)
end

function interactive_with_state_machine(_self, _player)
  log("Objecte interactiu [".. _self:get_name() .."] passant missatge a la màquina d'estats.")
end

function bido_toxic(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  if l_o3d then
    local l_position = l_o3d:get_position()
    EM:init_trigger_with_emiter("bubble", l_position, Vect3f(2.0, 3.0, 2.0), get_collision_group("trigger"), "triggerBidoToxic", "", 10, Vect3f(0,1,0))
  end
end

function bido_explosiu(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  local l_exp = _self:get_component(BaseComponent.explosive)
  if l_o3d then
    local l_position = l_o3d:get_position()
    l_exp:explode(l_position,4)
    local l_explosion = EM:init_emiter("explosio foc", l_position+Vect3f(0, 1.0, 0), Vect3f(1.5, 1.5, 1.5), 1.5, Vect3f(0,1,0))
    ComponentOmni.add_to_entity(l_explosion, Vect3f(0.0, 0.0, 0.0), Color(2.0,2.0,2.0, 1.0), 3, 10,"llum_bido_explosiu_update")
    
    --SOUND:play_sample_3D('explosion', l_position)
    SOUND:play_sample('explosion')
  end
end

function granada(_self)
  local l_o3d = _self:get_component(BaseComponent.object_3d)
  local l_exp = _self:get_component(BaseComponent.explosive)
  local l_render = _self:get_component(BaseComponent.renderable_object)
  local l_sphere = _self:get_component(BaseComponent.physx_sphere)
  local l_lifetime = _self:get_component(BaseComponent.life_time)
  local l_omni = _self:get_component(BaseComponent.omni)
  if l_o3d then
    local l_position = l_o3d:get_position()
    l_exp:explode(l_position,4)
    --l_lifetime.kill_entity = false
    l_lifetime.target_time = l_lifetime.target_time + 0.5
    --l_lifetime.script = ""
	
	
    local l_explosion = EM:init_emiter("explosio foc", l_position+Vect3f(0, 1.0, 0), Vect3f(1.5, 1.5, 1.5), 1.5, Vect3f(0,1,0))
    ComponentOmni.add_to_entity(l_explosion, Vect3f(0.0, 0.0, 0.0), Color(2.0,2.0,2.0, 1.0), 3, 10,"llum_bido_explosiu_update")
	
	  l_render:set_active(false)
	  l_exp:set_active(false)
	  l_omni:set_active(false)
    
    
    --log('bum')
    SOUND:play_sample('explosion')
    
    --aixo peta
    --l_sphere:set_active(false)
  end
end

function llum_bido_explosiu_update(_Entity, delta)
  local l_life_time = _Entity:get_component(BaseComponent.life_time)
  
  if l_life_time then
  
    if l_life_time.time > 0.1 then
      local l_omni_component = _Entity:get_component(BaseComponent.omni)
      l_omni_component:set_active(false)
      
      --local l_omni_light = l_omni_component:get_omni_light()
      
      --local l_end_att = l_omni_light:get_end_att()
      
      --l_omni_light:set_end_att(l_end_att - 3*delta)
      
    end
    
  end
  
end

-------------------------------------------- TRIGGERS  -------------------------------------------
-- aplica un DoT a qui entri
function triggerBidoToxic(_EntityTrigger, _Entity)
  --aplicar un DoT
  local l_vida = _Entity:get_component(BaseComponent.vida)
  if l_vida then
    l_vida:add_dot(30.0,2.0)
  end
end