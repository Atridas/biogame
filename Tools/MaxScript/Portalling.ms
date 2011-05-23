fileIn "GlobalsAndHelpers.ms"
fileIn "ObjectControl.ms"

--Crea una array amb les diferents habitacions de l'escenari.
function CreateRooms _objects = 
(
	local l_rooms = #(undefined)
	if _objects != undefined then
	(
		local l_numObjects = _objects.count
		
		for i = 1 to l_numObjects do
		(
			if classof _objects[i] == Container then
			(
				append l_rooms _objects[i]
			)
		)
	)
	return l_rooms
)

--Crea una array amb els objectes que es troben dins de cada habitació.
--L'índex de la habitació ve determinat pel paràmetre array d'habitacions
function FillRooms _objects _rooms = 
(
	local l_grouped = #()
	
	if classof _rooms == Array then
	(
		--init group array
		local l_arrayLength = _rooms.count
		
		for i = 1 to l_arrayLength do
		(
			append l_grouped #()
		)
		
		--fill array
		if _objects != undefined then
		(
			local l_container
			local l_index
			local l_current = undefined
			l_arrayLength = _objects.count
			
			for i = 1 to l_arrayLength do
			(
				l_current = _objects[i]
				if (IsInfo l_current) == false then
				(
					--container al que pertany
					l_container = Containers.IsInContainer l_current

					--posició del container a l'array de containers
					l_index = MemberPosition _rooms l_container

					if l_index != false then
					(
						append l_grouped[l_index] l_current
					)
				)
			)
		)
	)
	
	return l_grouped
)

--comprova si un element és un portal
function IsPortal _object =
(
	if _object != undefined then
	(
		if classof _object == Dummy then
		(
			local l_port1 = GetUserProp _object "roomA"
			local l_port2 = GetUserProp _object "roomB"
			if l_port1 != undefined and l_port2 != undefined then
			(
				return true
			)
		)
	)
	return false
)

--Crea una array amb els portals de l'escena.
function CreatePortals _objects =
(
	local l_portals = #()
	
	if _objects == undefined or classof _objects != Array then
	(
		local l_total = _objects.count
		local l_current = undefined
		
		for i = 1 to l_total do
		(
			l_current = _objects[i]
			if IsPortal l_current then
			(
				append l_portals l_current
			)
		)
	)
	
	return l_portals
)