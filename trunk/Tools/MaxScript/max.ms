global VERTEX_TYPE_GEOMETRY = 0x0001
global VERTEX_TYPE_NORMAL = 0x0002
global VERTEX_TYPE_TANGENT = 0x0004
global VERTEX_TYPE_BINORMAL = 0x0008
global VERTEX_TYPE_TEXTURE1 = 0x0010
global VERTEX_TYPE_TEXTURE2 = 0x0020
global VERTEX_TYPE_DIFFUSE = 0x0040

function ExportMaterial _material _file = 
(
	local l_cont = 0
	if _material.diffuseMapEnable == true and  _material.diffuseMap != undefined then
	(
		l_cont = l_cont + 1
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		l_cont = l_cont + 1
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		l_cont = l_cont + 1
	)
	
	if _material.reflectionMapEnable == true and  _material.reflectionMap != undefined then
	(
		l_cont = l_cont + 1
	)
	
	WriteShort _file l_cont #unsigned
	
	if _material.diffuseMapEnable and  _material.diffuseMap != undefined then
	(
		WriteShort _file (filenameFromPath (openBitmap _material.diffuseMap.bitmap.filename).filename).count #unsigned
		WriteString _file (filenameFromPath (openBitmap _material.diffuseMap.bitmap.filename).filename)
	)
	
	if _material.bumpMapEnable and  _material.bumpMap != undefined then
	(
		WriteShort _file (filenameFromPath (openBitmap _material.bumpMap.bitmap.filename).filename).count #unsigned
		WriteString _file (filenameFromPath (openBitmap _material.bumpMap.bitmap.filename).filename)
	)
	
	if _material.selfIllumMapEnable and  _material.selfIllumMap != undefined then
	(
		WriteShort _file (filenameFromPath (openBitmap _material.selfIllumMap.bitmap.filename).filename).count #unsigned
		WriteString _file (filenameFromPath (openBitmap _material.selfIllumMap.bitmap.filename).filename)
	)
	
	if _material.reflectionMapEnable and  _material.reflectionMap != undefined then
	(
		WriteShort _file (filenameFromPath (openBitmap _material.reflectionMap.bitmap.filename).filename).count #unsigned
		WriteString _file (filenameFromPath (openBitmap _material.reflectionMap.bitmap.filename).filename)
	)
)

function CreateVertex _pos _normal _tuv1 _tuv2 _diffuseColor _vertexType vertexOut_ =
(
	--local l_vertexOut = #(_pos.x,_pos.y,_pos.z,_normal.x,_normal.y,_normal.z,_tuv1.x,1.0-_tuv1.y)
	local l_vertexOut = #()
	
	if (bit.and _vertexType VERTEX_TYPE_GEOMETRY) > 0 then
	(
		append l_vertexOut _pos.x
		append l_vertexOut _pos.y
		append l_vertexOut _pos.z
	)
	
	if (bit.and _vertexType VERTEX_TYPE_NORMAL) > 0 then
	(
		append l_vertexOut _normal.x
		append l_vertexOut _normal.y
		append l_vertexOut _normal.z
	)
	
	if (bit.and _vertexType VERTEX_TYPE_TEXTURE1) > 0 then
	(
		append l_vertexOut _tuv1.x
		append l_vertexOut (1.0-_tuv1.y)
	)
	
	if (bit.and _vertexType VERTEX_TYPE_TEXTURE2) > 0 then
	(
		append l_vertexOut _tuv1.x
		append l_vertexOut (1.0-_tuv1.y)
		append l_vertexOut _tuv2.x
		append l_vertexOut (1.0-_tuv2.y)
	)
	
	if (bit.and _vertexType VERTEX_TYPE_DIFFUSE) > 0 then
	(
		append l_vertexOut _diffuseColor
	)
	
	join vertexOut_ l_vertexOut
)

function RHTranslationToLH _vector =
(
	return point3 _vector.x _vector.z _vector.y
)

function GetVertexType _material =
(
	local l_vertexType = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	
	if _material.diffuseMapEnable and  _material.diffuseMap != undefined then
	(
		
		if _material.selfIllumMapEnable and _material.selfIllumMap!=undefined then
			l_vertexType = bit.or l_vertexType VERTEX_TYPE_TEXTURE2
		else
			l_vertexType = bit.or l_vertexType VERTEX_TYPE_TEXTURE1
		
		if _material.bumpMapEnable and  _material.bumpMap != undefined then
		(
			l_vertexType = bit.or l_vertexType VERTEX_TYPE_TANGENT
			l_vertexType = bit.or l_vertexType VERTEX_TYPE_BINORMAL
		)
		
	) else (
		l_vertexType = bit.or l_vertexType VERTEX_TYPE_DIFFUSE
	)
	
	return l_vertexType
)

function AreEqualVertexs _vertex1 _vertex2 =
(
	local l_vertexLength = _vertex1.count
	local l_iterator = 1
	
	while l_iterator <= l_vertexLength do
	(
		if _vertex1[l_iterator] != _vertex2[l_iterator] then
			return false
		
		l_iterator = l_iterator+1
	)
	
	return true
	
)

function FindVertex _vertexList _vertex =
(
	local l_vertexLength = _vertexList.count
	local l_iterator = 1
	
	while l_iterator <= l_vertexLength do
	(
		if AreEqualVertexs _vertexList[l_iterator] _vertex then
			return l_iterator
		l_iterator = l_iterator+1
	)
	
	return -1
)

function AddVertex _vertexList _vertex =
(
	local l_index = FindVertex _vertexList _vertex
	
	if l_index != -1 then
	(
		return l_index-1
	)else(
		append _vertexList _vertex
		return _vertexList.count-1
	)
)

function AddIndex _indexList _index =
(
	append _indexList _index
)

function ExportMesh _object _filename = 
(
	local l_header = 0xAAAA
	local l_footer = 0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	local l_materialCount = 1
	local l_multimaterial = false
	
	local l_vertexList = #()
	local l_indexList = #()
	local l_vertexType = #()
		

	if classof(l_material) == Multimaterial then
	(
		l_multimaterial = true
		l_materialCount = l_material.count
	)
	
	WriteShort l_file l_materialCount #unsigned
	
	for i = 1 to l_materialCount do
	(
		if l_multimaterial then
		(
			local l_VertexTypeSingle = GetVertexType l_material[i]
			WriteShort l_file l_VertexTypeSingle #unsigned
			ExportMaterial l_material[i] l_file
			append l_vertexType l_VertexTypeSingle
		) else (
			WriteShort l_file (GetVertexType l_material) #unsigned
			ExportMaterial l_material l_file
			append l_vertexType (GetVertexType l_material)
		)
		
		local new_vertex_list=#()
		local new_index_list=#()
		
		append l_vertexList new_vertex_list
		append l_indexList new_index_list
	)
	
	local l_vertexCount = getNumVerts _object
	local l_facesCount = getNumFaces _object
	
	for l_faceIterator = 1 to l_facesCount do
	(
		local l_idxsFace = getFace _object l_faceIterator
		local l_materialID = 1
		
		if l_multimaterial then
		(
			l_materialID = getFaceMatID _object l_faceIterator
		)
		
		local l_vtx1 = RHTranslationToLH (getVert _object l_idxsFace.x)
		local l_vtx2 = RHTranslationToLH (getVert _object l_idxsFace.y)
		local l_vtx3 = RHTranslationToLH (getVert _object l_idxsFace.z)
		
		-- TODO GetNormal!
		local l_norm1 = RHTranslationToLH (getVert _object l_idxsFace.x)
		local l_norm2 = RHTranslationToLH (getVert _object l_idxsFace.y)
		local l_norm3 = RHTranslationToLH (getVert _object l_idxsFace.z)
		
		local l_idxsMap = meshop.getMapFace _object 1 l_faceIterator
		
		local l_tuv11 = meshop.getMapVert _object 1 l_idxsMap.x
		local l_tuv12 = meshop.getMapVert _object 1 l_idxsMap.y
		local l_tuv13 = meshop.getMapVert _object 1 l_idxsMap.z
		
		--TODO tex2
		local l_tuv21 = l_tuv11
		local l_tuv22 = l_tuv12
		local l_tuv23 = l_tuv13
		
		local l_diffuse1 = 0
		local l_diffuse2 = 0
		local l_diffuse3 = 0
		
		local l_outVert1 = #()
		local l_outVert2 = #()
		local l_outVert3 = #()
		
		CreateVertex l_vtx1 l_norm1 l_tuv11 l_tuv21 l_diffuse1 l_vertexType[l_materialID] l_outVert1
		CreateVertex l_vtx2 l_norm2 l_tuv12 l_tuv22 l_diffuse2 l_vertexType[l_materialID] l_outVert2
		CreateVertex l_vtx3 l_norm3 l_tuv13 l_tuv23 l_diffuse3 l_vertexType[l_materialID] l_outVert3
		
		local l_indexVert1 = AddVertex l_vertexList[l_materialID] l_outVert1
		local l_indexVert2 = AddVertex l_vertexList[l_materialID] l_outVert2
		local l_indexVert3 = AddVertex l_vertexList[l_materialID] l_outVert3
		
		AddIndex l_indexList[l_materialID] l_indexVert1
		AddIndex l_indexList[l_materialID] l_indexVert3
		AddIndex l_indexList[l_materialID] l_indexVert2
		
	)

	for l_materialIterator = 1 to l_materialCount do
	(
		WriteShort l_file l_vertexList[l_materialIterator].count #unsigned
		
		for l_vertexIterator = 1 to l_vertexList[l_materialIterator].count do
		(
			for l_iterator = 1 to l_vertexList[l_materialIterator][l_vertexIterator].count do
			(
				WriteFloat l_file (l_vertexList[l_materialIterator][l_vertexIterator])[l_iterator]
			)
			
		)
	
		WriteShort l_file l_indexList[l_materialIterator].count #unsigned
		
		--Exportem la llista de index
		for l_indexIterator = 1 to l_indexList[l_materialIterator].count do
		(
			WriteShort l_file l_indexList[l_materialIterator][l_indexIterator]
		)
		
		format "%\n" l_indexList[l_materialIterator].count
		format "%\n" l_indexList[l_materialIterator]
		format "%\n" l_vertexList[l_materialIterator].count
		format "%\n" l_vertexList[l_materialIterator]		
	)
		
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
)

ExportMesh $ "D:/a.mesh"