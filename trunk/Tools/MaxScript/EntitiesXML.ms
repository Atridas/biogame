fileIn "GlobalsAndHelpers.ms"
fileIn "XML.ms"
fileIn "ObjectControl.ms"

--Genera els paràmetres d'un tag XML d'entitat Object3D. Conté:
-- posició
-- yaw
-- pitch
-- roll
function XMLEntityObject3D _instance &_TagName =
(
	local l_params = #()
	local l_aux
	
	_TagName = "Object3D"
	
	l_aux = "" as stringStream
	local l_translation = RHVectorToLH _instance.transform.translation
	format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
	append l_params #("position",l_aux)
	
	l_aux = "" as stringStream
	local l_yaw = GetYaw _instance.transform
	format "%" l_yaw to:l_aux
	append l_params #("yaw",l_aux)
	
	l_aux = "" as stringStream
	local l_pitch = GetPitch _instance.transform
	format "%" l_pitch to:l_aux
	append l_params #("pitch",l_aux)
	
	l_aux = "" as stringStream
	local l_roll = GetRoll _instance.transform
	format "%" l_roll to:l_aux
	append l_params #("roll",l_aux)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat RenderableObject. Conté:
-- animated (true/false)
-- name (identificador de la core)
-- [cycle]
function XMLEntityRenderableObject _instance &_TagName _animated =
(
	local l_params = #()
	
	_TagName = "RenderableObject"
	
	--animated
	if _animated == true then
	(
		append l_params #("animated","true")
	)else
	(
		append l_params #("animated","false")
	)
	
	--name
	local l_resource = GetUserProp _instance "resource"
	if l_resource == undefined or classof l_resource != String then
	(
		l_resource = _instance.name
	)
	append l_params #("name", l_resource)
	
	--cycle
	local l_cycle = GetUserProp _instance "cycle"
	if l_cycle != undefined then
	(
		append l_params #("cycle",l_cycle)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat PhysXBox. Conté:
-- fromRenderableObject:false
	-- shape=box
	-- size
	-- midpoint
-- fromRenderableObject:true
-- density
-- collision_group
function XMLEntityPhysXBox _instance &_TagName _colGroup _fromRenderableObject =
(
	local l_params = #()
	local l_ss
	local l_aux
	
	_TagName = "PhysXBox"
	
	--renderableobject
	if _fromRenderableObject then
	(
		append l_params #("fromRenderableObject","true")
	)else
	(
		append l_params #("fromRenderableObject","false")

		--shape
		append l_params #("shape","box")
		
		--size
		l_ss = "" as stringstream
		l_aux = RHVectorToLH (DummySize _instance)
		format "% % %" l_aux.x l_aux.y l_aux.z to:l_ss
		
		append l_params #("size", l_ss)
		
		--midpoint
		l_ss = "" as stringstream
		format "0 0 0" to:l_ss
		
		append l_params #("midpoint", l_ss)
	)
	
	--density
	l_ss = "" as stringstream
	if (l_aux = GetUserProp _instance "physXBox") != undefined then
	(
		if classof l_aux != Integer then
		(
			l_aux = 0
		)
	)else
	(
		l_aux = 0
	)
	
	format "%" l_aux to:l_ss
	append l_params #("density", l_ss)
	
	--collision group
	append l_params #("collision_group", _colGroup)
		
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat PhysXMesh. Conté:
-- density
-- collision_group
function XMLEntityPhysXMesh _instance &_TagName _colGroup =
(
	local l_params = #()
	local l_ss
	local l_aux
	
	_TagName = "PhysXMesh"

	--density
	l_ss = "" as stringstream
	l_aux = GetUserProp _instance "physXMesh"

	if l_aux != undefined then
	(
		if (classof l_aux) != Integer then
		(
			l_aux = 0
		)
	)else
	(
		l_aux = 0
	)
	
	format "%" l_aux to:l_ss
	append l_params #("density", l_ss)
	
	--collision group
	append l_params #("collision_group", _colGroup)
		
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat PhysXMesh. Conté:
-- shape=box
-- size
-- collision_group
-- onEnter
-- onExit
function XMLEntityTrigger _instance &_TagName _colGroup _onEnter _onExit =
(
	local l_params = #()
	local l_aux = "" as stringStream
	
	_TagName = "Trigger"
	
	--size
	if classof _instance != Dummy then
	(
		local l_size = RHVectorToLH (MeshSize _instance)
		format "% % %" l_size.x l_size.y l_size.z to:l_aux
	)else
	(
		local l_size = RHVectorToLH (DummySize _instance)
		format "% % %" l_size.x l_size.y l_size.z to:l_aux
	)
	
	--shape, size, collision
	l_params = #(#("shape","box"),#("size",l_aux), #("collision_group", _colGroup))
	
	--onEnter
	if _onEnter != undefined and classof _onEnter  ==  String then
	(
		append l_params #("onEnter", _onEnter)
	)
	
	--onExit
	if _onExit != undefined  and classof _onExit  ==  String then
	(
		append l_params #("onExit", _onExit)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat HighCover/LowCover. Conté:
-- shape=box
-- size
-- collision_group
-- cover_size
function XMLEntityCover _instance &_TagName =
(
	local l_params = #()
	local l_aux = "" as stringStream

	--size
	local l_size = RHVectorToLH (DummySize _instance)
	format "% % %" l_size.x l_size.y l_size.z to:l_aux
	
	--shape, size, collision
	l_params = #(#("shape","box"),#("size",l_aux),#("collision_group", "cobertura"))
		
	if (GetUserProp _instance "LowCover") != undefined then
	(
		_TagName = "LowCover"
		
		l_aux = "" as stringStream
		l_size = GetUserProp _instance "LowCover"
		if classof l_size != Integer then
		(
			l_size = 1
		)
		format "%" l_size to:l_aux
		append l_params #("cover_size",l_aux)
	)
	else
	(
		_TagName = "HighCover"
		
		l_aux = "" as stringStream
		l_size = GetUserProp _instance "HighCover"
		if classof l_size != Integer then
		(
			l_size = 1
		)
		format "%" l_size to:l_aux
		append l_params #("cover_size",l_aux)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Door. Conté:
-- open
-- [block]
-- [open_time]
-- [close_time]
function XMLEntityDoor _instance &_TagName =
(
	local l_params = #()
	
	_TagName = "Door"
	
	--open
	local l_tmp = GetUserProp _instance "open"
	if l_tmp != undefined then
	(
		l_tmp = "true"
	)else
	(
		l_tmp = "false"
	)
	
	append l_params #("open",l_tmp)
	
	--block
	local l_tmp = GetUserProp _instance "block"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("block",l_aux)
	)
	
	--open_time
	local l_tmp = GetUserProp _instance "open_time"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("open_time",l_aux)
	)
	
	--close_time
	local l_tmp = GetUserProp _instance "close_time"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("close_time",l_aux)
	)
	
	--size
	l_aux = "" as stringStream
	local l_size = RHVectorToLH (DummySize _instance)
	format "% % %" l_size.x l_size.y l_size.z to:l_aux
	append l_params #("size",l_aux)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Rotation. Conté:
-- [yaw]
-- [pitch]
-- [roll]
function XMLEntityRotation _instance &_TagName =
(
	local l_params = #()
	
	_TagName = "Rotative"
	
	--yaw
	local l_tmp = GetUserProp _instance "yaw"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("yaw",l_aux)
	)
	
	--pitch
	local l_tmp = GetUserProp _instance "pitch"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("pitch",l_aux)
	)
	
	--roll
	local l_tmp = GetUserProp _instance "roll"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("roll",l_aux)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Player. Conté:
-- position
-- yaw
function XMLEntityPlayer _instance &_TagName =
(
	local l_params = #()
	local l_aux = "" as stringStream
	
	_TagName = "Player"
	
	--position
	local l_tmp = RHVectorToLH _instance.position
	format "% % %" l_tmp.x l_tmp.y l_tmp.z to:l_aux
	append l_params #("position",l_aux)
	
	--yaw
	l_aux = "" as stringStream
	l_tmp = GetYaw (_instance.transform)
	format "%" l_tmp to:l_aux
	append l_params #("yaw",l_aux)
	
	--shoot
	local l_tmp = GetUserProp _instance "shoot"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("shoot_active",l_aux)
	)
	
	--force
	local l_tmp = GetUserProp _instance "force"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("force_active",l_aux)
	)
	
	--grenade
	local l_tmp = GetUserProp _instance "grenade"
	if l_tmp != undefined then
	(
		l_aux = "" as stringStream
		format "%" l_tmp to:l_aux
		append l_params #("grenade_active",l_aux)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Miner/Militar. Conté:
-- name
-- position
-- yaw
-- [on_death]
-- [destiny]
function XMLEntityEnemy _instance &_TagName =
(
	local l_params = #()
	local l_aux
	local l_tmp
	
	_TagName = "UnknownEnemy"
	
	if (GetUserProp _instance "entity") != undefined then
	(
		--name
		l_tmp = _instance.name
		append l_params #("name",l_tmp)
		
		--position
		l_aux = "" as stringStream
		l_tmp = RHVectorToLH _instance.position
		format "% % %" l_tmp.x l_tmp.y l_tmp.z to:l_aux
		append l_params #("position",l_aux)
		
		--yaw
		l_aux = "" as stringStream
		l_tmp = GetYaw (_instance.transform)
		format "%" l_tmp to:l_aux
		append l_params #("yaw",l_aux)
		
		--active
		local l_active = GetUserProp _instance "active"
		if l_active == undefined then
		(
			l_active = "false"
		)
		l_aux = "" as stringStream
		format "%" l_active to:l_aux
		append l_params #("active", l_aux)
		
		--on_death
		l_tmp = GetUserProp _instance "onDeath"
		if l_tmp != undefined then
		(
			l_aux = "" as stringStream
			format "%" l_tmp to:l_aux
			append l_params #("on_death", l_aux)
		)
		
		--destiny
		l_tmp = GetUserProp _instance "destiny"
		if l_tmp != undefined then
		(
			l_aux = "" as stringStream
			format "%" l_tmp to:l_aux
			append l_params #("destiny", l_aux)
		)
		
		--Enemy Type
		if (GetUserProp _instance "entity") != "Player" then
		(
			_TagName = GetUserProp _instance "entity"
		)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Vigia. Conté:
-- name
-- position
-- yaw
-- zone_size
-- zone_position
-- zone_yaw
-- [on_death]
function XMLEntityVigia _instance _entities &_TagName =
(
	local l_params = #()
	local l_aux
	local l_tmp
	local l_zona = undefined
	
	_TagName = "Vigia"
	
	if (GetUserProp _instance "entity") != undefined and (GetUserProp _instance "ZonaVigia") != undefined then
	(
		l_aux = GetUserProp _instance "ZonaVigia"
		
		
		l_zona = GetObjectByName l_aux
		
		if l_zona != undefined then
		(
			--name
			l_tmp = _instance.name
			append l_params #("name",l_tmp)
			
			--position
			l_aux = "" as stringStream
			l_tmp = RHVectorToLH _instance.position
			format "% % %" l_tmp.x l_tmp.y l_tmp.z to:l_aux
			append l_params #("position",l_aux)
			
			--yaw
			l_aux = "" as stringStream
			l_tmp = GetYaw (_instance.transform)
			format "%" l_tmp to:l_aux
			append l_params #("yaw",l_aux)
			
			--active
			local l_active = GetUserProp _instance "active"
			if l_active == undefined then
			(
				l_active = "false"
			)
			l_aux = "" as stringStream
			format "%" l_active to:l_aux
			append l_params #("active", l_aux)
			
			--on_death
			l_tmp = GetUserProp _instance "onDeath"
			if l_tmp != undefined then
			(
				l_aux = "" as stringStream
				format "%" l_tmp to:l_aux
				append l_params #("on_death", l_aux)
			)
			
			l_aux = "" as stringStream
			local l_size = RHVectorToLH (DummySize l_zona)
			format "% % %" l_size.x l_size.y l_size.z to:l_aux
			append l_params #("zone_size", l_aux)
			
			l_aux = "" as stringStream
			local l_translation = RHVectorToLH l_zona.transform.translation
			format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
			append l_params #("zone_position",l_aux)
			
			l_aux = "" as stringStream
			local l_yaw = GetYaw l_zona.transform
			format "%" l_yaw to:l_aux
			append l_params #("zone_yaw",l_aux)
			
		)
		else
		(
			format "no s'ha trobat la zona %\n" l_aux
		)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Interactuable. Conté:
-- action
function XMLEntityInteractive _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Interactive"
	
	--action
	l_ss = "" as stringStream
	format "%" (GetUserProp _instance "interactive") to:l_ss
	append l_params #("action", l_ss)
	
	--billboard offset
	if (GetUserProp _instance "interactive_billboard_y_offset") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "interactive_billboard_y_offset") to:l_ss
		append l_params #("interactive_billboard_y_offset", l_ss)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Màquina d'estats. Conté:
-- initial_state
function XMLEntityStateMachine _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "StateMachine"
	
	--initial_state
	l_ss = "" as stringStream
	format "%" (GetUserProp _instance "state_machine") to:l_ss
	append l_params #("initial_state", l_ss)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Destructible. Conté:
-- onDestroy
-- destroyedResource
-- hitPoints
function XMLEntityDestroyable _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Destroyable"
	
	--onDestroy
	l_ss = "" as stringStream
	format "%" (GetUserProp _instance "onDestroy") to:l_ss
	append l_params #("onDestroy", l_ss)
	
	--destroyedResource
	if (GetUserProp _instance "destroyedResource") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "destroyedResource") to:l_ss
		append l_params #("destroyedResource", l_ss)
	)
	
	--hit points
	if (GetUserProp _instance "hitPoints") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "hitPoints") to:l_ss
		append l_params #("hitPoints", l_ss)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Explosiva.
function XMLEntityExplosive _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Explosive"
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat NavNode. Conté:
-- autoroute
-- maxDistance
function XMLEntityNavNode _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "NavNode"
	
	if  (GetUserProp _instance "autoroute") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "autoroute") to:l_ss
		append l_params #("autoroute", l_ss)
	)
	if (GetUserProp _instance "maxNavDistance") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "maxNavDistance") to:l_ss
		append l_params #("maxDistance", l_ss)
	)
	if (GetUserProp _instance "nodeCobertura") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "nodeCobertura") to:l_ss
		append l_params #("cobertura", l_ss)
	)
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Spawner. Conté:
-- spawner
-- spawn_time
-- max_enemy
-- active
function XMLEntitySpawner _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "EnemySpawner"
	
	if  (GetUserProp _instance "spawner") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "spawner") to:l_ss
		append l_params #("enemy_type", l_ss)
	)
	
	if (GetUserProp _instance "spawn_time") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "spawn_time") to:l_ss
		append l_params #("spawn_time", l_ss)
	)
	
	if (GetUserProp _instance "total_enemy") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "total_enemy") to:l_ss
		append l_params #("total_enemy", l_ss)
	)
	
	l_ss = "" as stringStream
	if (GetUserProp _instance "active") != undefined then
	(
		format "%" (GetUserProp _instance "active") to:l_ss
		append l_params #("active", l_ss)
	)
	else
	(
		append l_params #("active", "false")
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Emiter (de partícules). Conté:
-- core
-- volume
function XMLEntityEmiter _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Emiter"
	
	if  (GetUserProp _instance "emiter") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "emiter") to:l_ss
		append l_params #("core", l_ss)
	)
	
	if  (GetUserProp _instance "max_particles") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "max_particles") to:l_ss
		append l_params #("max_particles", l_ss)
	)
	
	--size
	l_ss = "" as stringstream
	l_aux = RHVectorToLH (DummySize _instance)
	format "% % %" l_aux.x l_aux.y l_aux.z to:l_ss
	append l_params #("volume", l_ss)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Billboard. Conté:
-- core
function XMLEntityBillboard _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Billboard"
	
	if  (GetUserProp _instance "billboard") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "billboard") to:l_ss
		append l_params #("core", l_ss)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Light. Conté:
-- resource
-- type
-- [tickle]
function XMLEntityLight _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "Light"
	append l_params #("resource", _instance.name)
	
	--Omni properties
	if (classof _instance) == Omnilight then
	(
		append l_params #("type", "omni")
	)
	
	--Directional properties
	if (classof _instance) == TargetDirectionallight then
	(
		append l_params #("type", "directional")
	)
	
	-- properties
	if (classof _instance) == targetSpot then
	(
		append l_params #("type", "spot")
	)
	
	if (GetUserProp _instance "tickle") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "tickle") to:l_ss
		append l_params #("tickle", l_ss)
	)
	
	if (GetUserProp _instance "script") != undefined then
	(
		l_ss = "" as stringStream
		format "%" (GetUserProp _instance "script") to:l_ss
		append l_params #("script", l_ss)
	)
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Camera. Conté:
-- target
-- fov
-- near
-- far
function XMLEntityCamera _instance &_TagName =
(
	local l_params = #()
	local l_ss
	
	_TagName = "CynematicCamera"
	l_ss = "" as stringstream
	l_aux = RHVectorToLH (_instance.target.pos)
	format "% % %" l_aux.x l_aux.y l_aux.z to:l_ss
	append l_params #("target", l_ss)
	
	l_ss = "" as stringstream
	format "%" _instance.fov to:l_ss
	append l_Params #("fov", l_ss)
	
	l_ss = "" as stringstream
	format "%" _instance.nearclip to:l_ss
	append l_Params #("near", l_ss)
	
	l_ss = "" as stringstream
	format "%" _instance.farclip to:l_ss
	append l_Params #("far", l_ss)
	
	return l_params
)