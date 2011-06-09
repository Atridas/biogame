fileIn "GlobalsAndHelpers.ms"
fileIn "ObjectControl.ms"

-- Comprova el tipus d'element bounding de l'objecte
-- a type es retorna el tipus de bounding
-- a target es retorna l'objecte que representa aquesta bounding, en cas de ser undefined es tracta del propi objecte
function GetBoundingType _object &_type &_target =
(
	local l_property = undefined
	if _object != undefined then
	(
		l_property = GetUserProp _object "BoundingBox"
		if l_property == undefined then
		(
			l_property = GetUserProp _object "BoundingSphere"
			if l_property == undefined then
			(
				_type = "Default"
			)else(
				_type = "BoundingSphere"
			)
		)else(
			_type = "BoundingBox"
		)
		
		_target = GetObjectByName l_property
		
		return 1
	)
	return 0
)

--agafa els vèrtexs màxim i mínim de l'objecte en coordenades absolutes.
--si es facilita un _target, aquest ha de ser un dummy i se n'exportarà el max-min en coordenades relatives.
function CalcMaxMin _object _target =
(
	local l_params = #()
	local l_aux = undefined
	local l_string = undefined
	local l_coord = undefined
	local l_invTransform = inverse _object.transform
	local l_max = undefined
	local l_min = undefined
	
	l_aux = "" as stringStream
	if _target != undefined then
	(
		--size/rotation/position
		local l_size = DummySize _target
		--transformada relativa a l'objecte
		local l_transform = _target.transform * l_invTransform
		
		l_max = RHVectorToLH(0.5 * l_size * l_transform)
		l_min = RHVectorToLH(-0.5 * l_size * l_transform)
		
	)else
	(
		--max / min
		l_max = RHVectorToLH (_object.max * l_invTransform)
		l_min = RHVectorToLH (_object.min * l_invTransform)
	)
	
	if l_max != undefined and l_min != undefined then
	(
		format "% % %" l_max.x l_max.y l_max.z to:l_aux
		l_string = l_aux as string
		append l_params #("max",l_string)
		
		l_aux = "" as stringStream
		
		format "% % %" l_min.x l_min.y l_min.z to:l_aux
		l_string = l_aux as string
		append l_params #("min",l_string)
	)
	
	return l_params	
)


--Calcula la box o esfera de l'objecte i en retorna
-- els paràmetres llestos per a l'exportació XML
function GetPhysxWrapper _object =
(
	local l_params = #()
	
	if _object != undefined then
	(
		local l_type = undefined
		local l_target = undefined
		
		if (GetBoundingType _object &l_type &l_target) != 0 then
		(			
			if l_type == "BoundingBox" then
			(
				l_params = CalcMaxMin _object l_target
			)else if l_type == "BoundingSphere" then
			(
			)else if l_type == "Default" then
			(
				l_params = CalcMaxMin _object _target
			)
		)
	)
	
	return l_params
)