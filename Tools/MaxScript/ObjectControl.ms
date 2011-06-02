fileIn "GlobalsAndHelpers.ms"

-- Comprova si l'objecte proporcionat és una mesh
function IsMesh _object =
(
	-- TODO acceptar més coses
	if classof _object == Editable_Poly or classof _object == Box or classof _object == Editable_mesh or classof _object == Cylinder or classof _object == GeoSphere then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat és un model animat
function isAnimatedModel _object =
(
	-- TODO acceptar més coses
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

-- Comprova si l'objecte proporcionat és una llum
function IsLight _object =
(
	if classof _object == TargetDirectionallight or classof _object == Omnilight or classof _object == targetSpot then
	(
		return true
	) else (
		return false
	)
)

-- Comprova si l'objecte proporcionat és una mesh
function IsTrigger _object =
(
	-- TODO acceptar més coses
	if classof _object == Dummy and ((GetUserProp _object "TriggerEnter") != undefined or (GetUserProp _object "TriggerExit") != undefined) then
	(
		return true
	) else (
		return false
	)
)

-- Cerca l'índex de la core segons el nom
function GetCoreName _object =
(
	-- S'agafa la core del paràmetre
	local l_name = getUserProp _object "resource"

	if l_name == undefined then
	(
		l_name = _object.name
	)
	return l_name
)

-- Retorna l'índex de la core de l'objecte
function GetCoreIndex _cores _object =
(
	-- S'agafa la core del paràmetre
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
-- Aquesta core és una copia d'una malla, la llista s'ha de netejar un cop feta servir
function AddCore _cores _object _name =
(
	if (CheckRepeatedName _cores _name) == false then
	(
		append _cores _object
	)else
	(
		-- core amb nom repetit, no hauria de passar mai perquè abans es comprova
		local l_message = "" as stringStream
		format "La core % està repetida. Si es vol instanciar feu servir la propietat d'usuari." _name to:l_message
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

-- Afegeix una instancia a la llista d'instàncies de la posició _index
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
		format "La instancia % està repetida. Canvia-li el nom." _instance.name to:l_message
		ShowError l_message
	)
)

-- Afegeix una nova llista d'instàncies i una instància
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

--comprova si l'objecte és un element d'escenari o control
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
