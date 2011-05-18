fileIn "GlobalsAndHelpers.ms"

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

function CalcMaxMin _object _target =
(
	local l_params = #()
	local l_aux = undefined
	local l_string = undefined
	local l_coord = undefined
	local l_invTransform = inverse _object.transform
	
	l_aux = "" as stringStream
	if _target != undefined then
	(
		l_coord = _target.max
	)else
	(
		l_coord = _object.max
	)
	
	l_coord = l_coord * l_invTransform
	l_coord = RHVectorToLH l_coord
	format "% % %" l_coord.x l_coord.y l_coord.z to:l_aux
	l_string = l_aux as string
	append l_params #("max",l_string)
	
	l_aux = "" as stringStream
	if _target != undefined then
	(
		l_coord = _target.min
	)else
	(
		l_coord = _object.min
	)
	l_coord = l_coord * l_invTransform
	l_coord = RHVectorToLH l_coord
	format "% % %" l_coord.x l_coord.y l_coord.z to:l_aux
	l_string = l_aux as string
	append l_params #("min",l_string)
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