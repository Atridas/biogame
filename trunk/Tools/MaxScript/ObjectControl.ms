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

-- Retorna l'índex de la core de l'objecte
function GetCoreIndex _cores _object =
(
	-- S'agafa la core del paràmetre
	local l_name = getUserProp _object "resource"

	if l_name != undefined then
	(
		for i=1 to _cores.count do
		(
			if _cores[i].name == l_name then
			(
				return i
			)
		)
	)
	return undefined
)

-- Cerca l'índex de la core segons el nom
function GetCoreName _object =
(
	return getUserProp _object "resource"
)

-- Afegeix una core a la llista de cores.
-- Aquesta core és una copia d'una malla, la llista s'ha de netejar un cop feta servir
function AddCore _cores _object _name =
(
	if (CheckRepeatedName _cores _name) == false then
	(
		l_copy = copy _object
		l_copy.name = _name
		append _cores l_copy
	)else
	(
		-- core amb nom repetit, no hauria de passar mai perquè abans es comprova
		local l_message = "" as stringStream
		format "La core % està repetida" _name to:l_text
		ShowMessage l_message
	)
)

-- Allibera la llista de cores
function ClearCores _cores =
(
	for i=1 to _cores.count do
	(
		delete _cores[i]
	)
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
		format "La instancia % està repetida" _instance.name to:l_text
		ShowMessage l_message
	)
)

-- Afegeix una nova llista d'instàncies i una instància
function NewInstance _instances _instance =
(
	AddInstance _instances _instance (_instances.count + 1)
)