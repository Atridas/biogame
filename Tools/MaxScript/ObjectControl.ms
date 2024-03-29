fileIn "GlobalsAndHelpers.ms"

-- Comprova si l'objecte proporcionat �s una mesh
function IsMesh _object =
(
	-- TODO acceptar m�s coses
	if classof _object == Editable_Poly or classof _object == Box or classof _object == Editable_mesh or classof _object == Cylinder or classof _object == GeoSphere then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un model animat
function isAnimatedModel _object =
(
	-- TODO acceptar m�s coses
	if classof _object == Dummy then
	(
		if getUserProp _object "UserType" == "AnimatedModel" then
		(
			return true
		)
		else
		(
			return false
		)
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s una entitat model animat
function IsEntityAnimatedModel _object =
(
	if classof _object == Dummy and (getUserProp _object "resource") != undefined and (getUserProp _object "cycle") != undefined then
	(
		return true
	)
	
	return false
)

-- Comprova si l'objecte proporcionat �s una entitat model animat
function IsCamera _object =
(
	if classof _object == Targetcamera then
	(
		return true
	)
	
	return false
)

-- Comprova si l'objecte proporcionat �s una llum
function IsLight _object =
(
	if classof _object == TargetDirectionallight or classof _object == Omnilight or classof _object == targetSpot then
	(
		return true
	) else (
		return false
	)
)

--Comprova si la llum t� par�metres d'entitat
function IsLightEntity _object =
(
	if (GetUserProp _object "light_entity") != undefined then
	(
		return true
	)
	return false
)

--Comprova si l'objecte proporcionat ha de ser convertit en entitat o no.
function WantsEntity _object =
(
	if (GetUserProp _object "entity") == false then
	(
		return false
	)
	
	return true
)

--Comprova si l'objecte proporcionat t� m�quina d'estats
function HasStateMachine _object =
(
	if (GetUserProp _object "state_machine") != undefined then
	(
		return true
	)
	
	return false
)

-- Comprova si l'objecte proporcionat �s un trigger
function IsTrigger _object =
(
	-- TODO acceptar m�s coses
	if classof _object == Dummy and ((GetUserProp _object "TriggerEnter") != undefined or (GetUserProp _object "TriggerExit") != undefined) then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un dummy amb propietat f�sica
function IsPhysxDummy _object =
(
	-- TODO acceptar m�s coses
	if classof _object == Dummy and (GetUserProp _object "physXBox") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s una covertura
function IsCover _object =
(
	if classof _object == Dummy and ((GetUserProp _object "LowCover") != undefined or (GetUserProp _object "HighCover") != undefined) then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s una porta
function IsDoor _object =
(
	if classof _object == Dummy and (GetUserProp _object "resource") != undefined and (GetUserProp _object "type") == "door" then
	(
		return true
	) else (
		return false
	)
)

--Comprova si l'objecte proporcionat t� rotaci� fixa aplicada
function IsRotative _object =
(
	if (GetUserProp _object "rotative") != undefined then
	(
		return true
	)
	return false
)

-- Comprova si l'objecte proporcionat �s un pick up
function IsPickUp _object =
(
	if classof _object == Dummy and (GetUserProp _object "pickup") != undefined and (GetUserProp _object "resource") != undefined then
	(
		return true
	) else (
		if classof _object != Dummy and (GetUserProp _object "pickup") != undefined then
		(
			return true
		)
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un objecte interactiu
function IsInteractive _object =
(
	if classof _object == Dummy and (GetUserProp _object "interactive") != undefined and (GetUserProp _object "resource") != undefined then
	(
		return true
	) else (
		if classof _object != Dummy and (GetUserProp _object "interactive") != undefined then
		(
			return true
		)
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un objecte destructible
function IsDestroyable _object =
(
	if (GetUserProp _object "onDestroy") != undefined and (GetUserProp _object "resource") != undefined then
	(
		return true
	) else (
		if classof _object != Dummy and (GetUserProp _object "onDestroy") != undefined then
		(
			return true
		)
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un objecte destructible
function IsExplosive _object =
(
	if (GetUserProp _object "explosive") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un node del graf de navegaci�
function IsNavNode _object =
(
	-- TODO [[ IsPortal _object ]]
	if classof _object == Dummy and (GetUserProp _object "navNode") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un emisor de particules
function IsEmiter _object =
(
	if classof _object == Dummy and (GetUserProp _object "emiter") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un emisor de particules
function IsBillboard _object =
(
	if classof _object == Dummy and (GetUserProp _object "billboard") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat �s un spawner d'enemics
function IsSpawner _object =
(
	if classof _object == Dummy and (GetUserProp _object "spawner") != undefined and (GetUserProp _object "spawn_time") != undefined and (GetUserProp _object "total_enemy") != undefined then
	(
		return true
	) else (
		return false
	)
)

-- Cerca l'entitat de player
function GetPlayerEntity _entities =
(
	for i = 1 to _entities.count do
	(
		if (GetUserProp _entities[i] "entity") == "Player" then
		(
			return _entities[i]
		)
	)
	return undefined
)

-- Cerca les entitats d'enemics
function GetEnemiesEntities _entities =
(
	local l_enemies = #()
	local l_property = undefined
	
	for i = 1 to _entities.count do
	(
		l_property = GetUserProp _entities[i] "entity"
		if l_property == "Miner" or l_property == "Militar" then
		(
			append l_enemies _entities[i]
		)
	)
	
	if l_enemies.count == 0 then
	(
		return undefined
	)else
	(
		return l_enemies
	)
)

-- Cerca les entitats d'enemics
function GetEnemiesVigiesEntities _entities =
(
	local l_enemies = #()
	local l_property = undefined
	
	for i = 1 to _entities.count do
	(
		l_property = GetUserProp _entities[i] "entity"
		if l_property == "Vigia" then
		(
			append l_enemies _entities[i]
		)
	)
	
	if l_enemies.count == 0 then
	(
		return undefined
	)else
	(
		return l_enemies
	)
)

-- Cerca l'�ndex de la core segons el nom
function GetCoreName _object =
(
	-- S'agafa la core del par�metre
	local l_name = getUserProp _object "resource"

	if l_name == undefined then
	(
		l_name = _object.name
	)
	return l_name
)

-- Retorna l'�ndex de la core de l'objecte
function GetCoreIndex _cores _object =
(
	-- S'agafa la core del par�metre
	local l_name = getUserProp _object "resource"

	if l_name == undefined then
	(
		l_name = _object.name
	)
	
	for i=1 to _cores.count do
	(
		if (GetCoreName _cores[i]) == l_name then
		(
			return i
		)
	)
	return undefined
)

-- Comprova que el nom no existeixi ja a una array d'objectes
function CheckRepeatedName _array _name =
(
	for i=1 to _array.count do
	(
		if _array[i].name == _name then
		(
			return true
		)
	)
	
	return false
)

-- Afegeix una core a la llista de cores.
-- Aquesta core �s una copia d'una malla, la llista s'ha de netejar un cop feta servir
function AddCore _cores _object _name =
(
	if (CheckRepeatedName _cores _name) == false then
	(
		append _cores _object
	)else
	(
		-- core amb nom repetit, no hauria de passar mai perqu� abans es comprova
		local l_message = "" as stringStream
		format "La core % est� repetida. Si es vol instanciar feu servir la propietat d'usuari." _name to:l_message
		ShowError l_message
	)
)

-- Comprova que el nom de la instancia no estigui repetit
function CheckRepeatedInstanceName _instances _name =
(
	for i=1 to _instances.count do
	(
		if CheckRepeatedName _instances[i] _name then
			return true
	)
	
	return false
)

-- Afegeix una instancia a la llista d'inst�ncies de la posici� _index
function AddInstance _instances _instance _index =
(
	if (CheckRepeatedInstanceName _instances _instance.name) == false then
	(
		if _index == (_instances.count + 1) then
		(
			append _instances #()
		)
		append _instances[_index] _instance
	)else
	(
		-- instancia amb nom repetit
		local l_message = "" as stringStream
		format "La instancia % est� repetida. Canvia-li el nom." _instance.name to:l_message
		ShowError l_message
	)
)

-- Afegeix una nova llista d'inst�ncies i una inst�ncia
function NewInstance _instances _instance =
(
	AddInstance _instances _instance (_instances.count + 1)
)

--retorna l'element amb el nom demanat
function GetObjectByName _name =
(
	local l_objects = $*
	local l_found = undefined
	
	if l_objects != undefined then
	(
		local l_totalObjects = l_objects.count
		local l_current = undefined
		local i = 1
		
		while i <= l_totalObjects and l_found == undefined do
		(
			l_current = l_objects[i]
			
			if l_current.name == _name then
			(
				l_found = l_current
			)
			i = i + 1
		)
	)
	
	return l_found
)

--comprova si l'objecte �s un element d'escenari o control
function IsInfo _object = 
(
	if (IsMesh _object or isAnimatedModel _object or IsLight _object) == true then
	(
		return false
	)else if classof _object == Dummy or classof _object == Container then
	(
		return true
	)else
	(
		return false
	)
)

--retorna la mida x,y,z d'una mesh
function MeshSize _mesh=
(
	local l_transformX = _mesh.transform[1]
	local l_transformY = _mesh.transform[2]
	local l_transformZ = _mesh.transform[3]
	
	local l_sizeX = sqrt(l_transformX[1] * l_transformX[1] + l_transformX[2] * l_transformX[2] + l_transformX[3] * l_transformX[3])
	local l_sizeY = sqrt(l_transformY[1] * l_transformY[1] + l_transformY[2] * l_transformY[2] + l_transformY[3] * l_transformY[3])
	local l_sizeZ = sqrt(l_transformZ[1] * l_transformZ[1] + l_transformZ[2] * l_transformZ[2] + l_transformZ[3] * l_transformZ[3])
	
	local l_max = _mesh.max
	local l_min = _mesh.min
	local boxsize = [abs(l_max.x - l_min.x), abs(l_max.y - l_min.y), abs(l_max.z - l_min.z)]
	
	return [boxsize.x * l_sizeX, boxsize.y * l_sizeY, boxsize.z * l_sizeZ]
)

--retorna la mida x,y,z d'un dummy
function DummySize _dummy =
(
	local l_transformX = _dummy.transform[1]
	local l_transformY = _dummy.transform[2]
	local l_transformZ = _dummy.transform[3]
	
	local l_sizeX = sqrt(l_transformX[1] * l_transformX[1] + l_transformX[2] * l_transformX[2] + l_transformX[3] * l_transformX[3])
	local l_sizeY = sqrt(l_transformY[1] * l_transformY[1] + l_transformY[2] * l_transformY[2] + l_transformY[3] * l_transformY[3])
	local l_sizeZ = sqrt(l_transformZ[1] * l_transformZ[1] + l_transformZ[2] * l_transformZ[2] + l_transformZ[3] * l_transformZ[3])
	
	return [_dummy.boxsize.x * l_sizeX, _dummy.boxsize.y * l_sizeY, _dummy.boxsize.z * l_sizeZ]
)
