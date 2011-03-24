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
		format "La core % est� repetida" _name to:l_message
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
		format "La instancia % est� repetida" _instance.name to:l_message
		ShowError l_message
	)
)

-- Afegeix una nova llista d'inst�ncies i una inst�ncia
function NewInstance _instances _instance =
(
	AddInstance _instances _instance (_instances.count + 1)
)