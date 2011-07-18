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
	if (l_aux = GetUserProp _instance "PhysXMesh") != undefined then
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
function XMLEntityDoor _instance &_TagName =
(
	local l_params = #()
	
	_TagName = "Door"
	
	local l_open = GetUserProp _instance "open"
	if l_open != undefined then
	(
		l_open = "true"
	)else
	(
		l_open = "false"
	)
	
	append l_params #("open",l_open)
	
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
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Miner/Militar/Robot. Conté:
-- name
-- position
-- yaw
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
		
		--Enemy Type
		if (GetUserProp _instance "entity") != "Player" then
		(
			_TagName = GetUserProp _instance "entity"
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
	
	return l_params
)

--Genera els paràmetres d'un tag XML d'entitat Destructible. Conté:
-- onDestroy
-- destroyedResource
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
	l_ss = "" as stringStream
	format "%" (GetUserProp _instance "destroyedResource") to:l_ss
	append l_params #("destroyedResource", l_ss)
	
	return l_params
)