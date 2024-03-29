fileIn "GlobalsAndHelpers.ms"
fileIn "ObjectControl.ms"


--comprova si un element �s un emisor de part�cules
function IsParticleEmitter _object =
(
	if _object != undefined then
	(
		if classof _object == Point or classof _object == Plane or classof _object==line or classof _object==box or classof _object==Dummy then
		(
			local l_tmp = GetUserProp _object "type"
			if l_tmp != undefined then
			(
				return true
			)
		)
	)
	return false
)

--Crea una array amb els diferents particle emitters.
function GetParticleEmitters _objects = 
(
	local l_emitters = #()
	
	if _objects != undefined then
	(
		local l_numObjects = _objects.count
		local l_current = undefined
		for i = 1 to l_numObjects do
		(
			l_current = _objects[i]
			if IsParticleEmitter l_current then
			(
				append l_emitters l_current
			)
		)
	)
	return l_emitters
)