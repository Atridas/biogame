fileIn "GlobalsAndHelpers.ms"

-- Color en format long
function CreateColorLong _Color =
(
	local l_r = (floor _Color.r) as Integer64
	local l_g = (floor _Color.g) as Integer64
	local l_b = (floor _Color.b) as Integer64
	local l_a = (floor _Color.a) as Integer64
	return l_r * 256 * 256 * 256 + l_g * 256 * 256 + l_b * 256 + l_a
)

-- Array de normals
global UABVtxsNormals = #()
function UABClearNormalsArray =
(
	while UABVtxsNormals.count > 0 do
	(
		deleteItem UABVtxsNormals 1
	)
)

-- Comprova si el grup smooth est� activat
function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask = 2^(IdGroup-1)
	return (bit.and Value ValueMask) == ValueMask
)

-- Calcula la normal del v�rtex
function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal = false
	local Normal = point3 0 0 0
	for b = 1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal = Normal + UABVtxsNormals[IdVertex][b]
			HasNormal = true
		)
	)
	if HasNormal == false then
	(
		Normal = FaceNormal
	)
	return Normal
)

-- C�lcula les normals dels v�rtex de l'objecte
function UABCalcVertexsNormals obj =
(
	format "Calculant Normals...\n"
	
	UABClearNormalsArray()
	local NumVtxs = getNumVerts obj
	for b = 1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		for t = 1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)
	
	local NumFaces = getNumFaces obj
	local InvTransform = inverse obj.transform
	for IdFace = 1 to NumFaces do
	(
		local IdxsFace = getFace obj IdFace
		local Vtx1 = (getVert obj IdxsFace.x) * InvTransform
		local Vtx2 = (getVert obj IdxsFace.y) * InvTransform
		local Vtx3 = (getVert obj IdxsFace.z) * InvTransform
		local FaceNormal = getFaceNormal obj IdFace
		local SmoothValue = getFaceSmoothGroup obj IdFace
		
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue) == true) then
			(
				UABVtxsNormals[IdxsFace.x][b] = UABVtxsNormals[IdxsFace.x][b] + FaceNormal
				UABVtxsNormals[IdxsFace.y][b] = UABVtxsNormals[IdxsFace.y][b] + FaceNormal
				UABVtxsNormals[IdxsFace.z][b] = UABVtxsNormals[IdxsFace.z][b] + FaceNormal
			)
		)
	)
)

-- Pren el tipus de v�rtex depenent de la informaci� del material
function GetVertexType _material =
(
	local mascara = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	if _material.bumpMapEnable == true then
	(
		mascara = bit.or mascara VERTEX_TYPE_TANGENT
		mascara = bit.or mascara VERTEX_TYPE_BINORMAL
	)
	if _material.diffuseMapEnable then
	(
		if _material.selfIllumMapEnable then
		(
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE2
		) else (
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE1
		)
	) else (
		mascara = bit.or mascara VERTEX_TYPE_DIFFUSE
	)
	
	return mascara
)

-- Crea un v�rtex segons els par�metres i l'estructura a seguir marcada per VertexType
function CreateVertex _Pos _Normal _TUV1 _TUV2 _DiffuseColor _VertexType VertexOut_ =
(
	local Vtx = #()
	if bit.and _VertexType VERTEX_TYPE_GEOMETRY > 0 then
	(
		append VertexOut_ _Pos.x
		append VertexOut_ _Pos.y
		append VertexOut_ _Pos.z
	)
	if bit.and _VertexType VERTEX_TYPE_NORMAL > 0 then
	(
		append VertexOut_ _Normal.x
		append VertexOut_ _Normal.y
		append VertexOut_ _Normal.z
	)
	
	if bit.and _VertexType VERTEX_TYPE_TANGENT > 0 then
	(
		append VertexOut_ 1.0 -- w de la normal
		
		append VertexOut_ 0.0
		append VertexOut_ 0.0
		append VertexOut_ 0.0
		append VertexOut_ 1.0
		
		append VertexOut_ 0.0
		append VertexOut_ 0.0
		append VertexOut_ 0.0
		append VertexOut_ 1.0
	)
	
	if bit.and _VertexType VERTEX_TYPE_TEXTURE1 > 0 then
	(
		append VertexOut_ _TUV1.x
		append VertexOut_ _TUV1.y
	) else if bit.and _VertexType VERTEX_TYPE_TEXTURE2 > 0 then
	(
		append VertexOut_ _TUV1.x
		append VertexOut_ _TUV1.y
		append VertexOut_ _TUV2.x
		append VertexOut_ _TUV2.y
	) else if bit.and _VertexType VERTEX_TYPE_DIFFUSE > 0 then
	(
		append VertexOut_ _DiffuseColor
	)
	
	join VertexOut_ Vtx
)

-- Afegeix el v�rtex en cas que no estigui a la llista.
-- Si es troba a la llista retorna l'�ndex d'on es trobava
function AddVertex _VertexList_ _MaterialID _Vertex =
(
	
	local l_index = uab_add_vtx_on_vertex_buffer _MaterialID _Vertex
	
	if l_index == _VertexList_[_MaterialID].count then
	(
		append _VertexList_[_MaterialID] _Vertex
	)
	return l_index
)

-- Afegeix l'�ndex a la llista
function AddIndex _IndexList_ _Index =
(
	append _IndexList_ _Index
)

--construim les llistes de vertexs i indexs
function VertexList _Obj _VertexType _numMaterials VertexList_ IndexList_ =
(	
	-- si tenim un multi material, creem les llistes de vertexs i indexs, sino, en creem nom�s una
	-- per simplificar les coses despr�s a l'hora d'escriure'ls
	for i = 1 to _numMaterials do
	(
		append VertexList_ #()
		append IndexList_ #()
	)
	
	-- netejem els vertex buffers del plug-in
	uab_clear_vertex_buffer()
	
	local l_materialID
	local l_Color
	local l_invTransform = inverse _Obj.transform
	
	local numFaces = getNumFaces _Obj
	for i = 1 to numFaces do
	(
		local UV2
		local IdxsMap2
		local face = GetFace _Obj i
		local IdxsMap = meshop.getMapFace _Obj 1 i
		local l_FaceNormal = GetFaceNormal _Obj i
		--l_FaceNormal = RHVectorToLH l_FaceNormal
		local l_SmoothValue = getFaceSmoothGroup _Obj i
		local VertexOut1 = #()
		local VertexOut2 = #()
		local VertexOut3 = #()
		if _numMaterials > 1 then -- agafem el material, en cas de ser multimaterial
		(
			l_materialID = getFaceMatID _Obj i
			l_Color = CreateColorLong _Obj.material[l_materialID].diffuse
		)
		else
		(
			l_materialID = 1
			l_Color = CreateColorLong _Obj.material.diffuse
		)
		
		if bit.and _VertexType[l_materialID] VERTEX_TYPE_TEXTURE2 != 0 then
		(
			IdxsMap2 = meshop.getMapFace _Obj 2 i
		)

		-- vertex 1
		local pos =  GetVert _Obj face.x
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		local TUV =  meshop.getMapVert _Obj 1 IdxsMap.x
		TUV = ChangeUVCoords TUV
		--format "Num maps: %\n" (meshop.getNumMaps _Obj)
    
		if bit.and _VertexType[l_materialID] VERTEX_TYPE_TEXTURE2 != 0 then
		(
			TUV2 = meshop.getMapVert _Obj 2 IdxsMap2.x
			TUV2 = ChangeUVCoords TUV2
		)
		local l_Normal = UABGetVertexNormal _Obj face.x l_SmoothValue l_FaceNormal
		l_Normal = l_Normal + _Obj.pos
		l_Normal = l_Normal * l_invTransform
		l_Normal = RHVectorToLH l_Normal
		
		CreateVertex pos l_Normal TUV TUV2 l_Color _VertexType[l_materialID] VertexOut1
		--format "%\n" VertexOut1

		-- vertex 2
		pos = GetVert _Obj face.y
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		TUV = meshop.getMapVert _Obj 1 IdxsMap.y
		TUV = ChangeUVCoords TUV
    
		if bit.and _VertexType[l_materialID] VERTEX_TYPE_TEXTURE2 != 0 then
		(
			TUV2 = meshop.getMapVert _Obj 2 IdxsMap2.y
			TUV2 = ChangeUVCoords TUV2
		)
		local l_Normal = UABGetVertexNormal _Obj face.y l_SmoothValue l_FaceNormal
		l_Normal = l_Normal + _Obj.pos
		l_Normal = l_Normal * l_invTransform
		l_Normal = RHVectorToLH l_Normal

		CreateVertex pos l_Normal TUV TUV2 l_Color _VertexType[l_materialID] VertexOut2
		--format "%\n" VertexOut2

		-- vertex 3
		pos = GetVert _Obj face.z
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		TUV = meshop.getMapVert _Obj 1 IdxsMap.z
		TUV = ChangeUVCoords TUV
    
		if bit.and _VertexType[l_materialID] VERTEX_TYPE_TEXTURE2 != 0 then
		(
			TUV2 =  meshop.getMapVert _Obj 2 IdxsMap2.z
			TUV2 = ChangeUVCoords TUV2
		)
		local l_Normal = UABGetVertexNormal _Obj face.z l_SmoothValue l_FaceNormal
		l_Normal = l_Normal + _Obj.pos
		l_Normal = l_Normal * l_invTransform
		l_Normal = RHVectorToLH l_Normal

		CreateVertex pos l_Normal TUV TUV2 l_Color _VertexType[l_materialID] VertexOut3
		--format "%\n" VertexOut3
		
		local index1
		local index2
		local index3
		
		--format "Face\n"
		--format "Vert %\n" VertexOut1
		--format "Vert %\n" VertexOut2
		--format "Vert %\n" VertexOut3
		
		index1 = AddVertex VertexList_ l_materialID VertexOut1
		index2 = AddVertex VertexList_ l_materialID VertexOut2
		index3 = AddVertex VertexList_ l_materialID VertexOut3
		
		AddIndex IndexList_[l_materialID] index1
		AddIndex IndexList_[l_materialID] index3
		AddIndex IndexList_[l_materialID] index2
	)
	
	-- tornem a netejar els vertex buffers del plug-in
	uab_clear_vertex_buffer()
)

-- Imprimeix els v�rtexs a fitxer
function WriteVertexs _File _VertexList _VertexType =
(
	format "Escribint % v�rtexs...\n" _VertexList.count
	WriteShort _File _VertexList.count #unsigned
	for i= 1 to _VertexList.count do
	(
		local l_vertex = _VertexList[i]
		
		local l_result =  bit.and _VertexType VERTEX_TYPE_GEOMETRY
		
		if bit.and _VertexType VERTEX_TYPE_DIFFUSE > 0 then
		(
			for j= 1 to l_vertex.count-1 do
			(
				WriteFloat _File l_vertex[j]
			)
			WriteLong _File l_vertex[l_vertex.count] #unsigned
		) else (
			for j= 1 to l_vertex.count do
			(
				WriteFloat _File l_vertex[j]
			)
		)
		
	)
)

-- Imprimeix els �ndexs a fitxer
function WriteIndexs _File _IndexList =
(
	format "Escribint % �ndexs...\n" _IndexList.count
	WriteLong _File _IndexList.count #unsigned
	for i= 1 to _IndexList.count do
	(
		WriteShort _File _IndexList[i] #unsigned
	)
)