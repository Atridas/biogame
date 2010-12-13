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
	
	--- lo mismo
	
	WriteShort _file l_cont #unsigned
	
	if _material.diffuseMapEnable and  _material.diffuseMap != undefined then
	(
		WriteShort _file _material.diffuseMap.filename.count #unsigned
		WriteString _file _material.diffuseMap.filename
	)
	
	if _material.bumpMapEnable and  _material.bumpMap != undefined then
	(
		WriteShort _file _material.bumpMap.filename.count #unsigned
		WriteString _file _material.bumpMap.filename
	)
	
	if _material.selfIllumMapEnable and  _material.selfIllumMap != undefined then
	(
		WriteShort _file _material.selfIllumMap.filename.count #unsigned
		WriteString _file _material.selfIllumMap.filename
	)
	
	if _material.reflectionMapEnable and  _material.reflectionMap != undefined then
	(
		WriteShort _file _material.reflectionMap.filename.count #unsigned
		WriteString _file _material.reflectionMap.filename
	)
)

function CreateVertex _pos _normal _tuv1 _tuv2 _diffuseColor _vertexType vertexOut_ =
(
	local l_vertexOut = #(_pos.x,_pos.y,_pos.z,_normal.x,_normal.y,_normal.z,_tuv1.x,1.0-_tuv1.y)
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
		
	)else	(
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
-		(
			WriteShort l_file (GetVertexType l_material[i]) #unsigned
			ExportMaterial l_material[i] l_file
			append l_vertexType (GetVertexType l_material[i])
		) else (
			WriteShort l_file (GetVertexType l_material) #unsigned
			ExportMaterial l_material l_file
			append l_vertexType (GetVertexType l_material)
		)
		
		append l_vertexList #()
		append l_indexList #()
	)
	
	local l_vertexCount = getNumVerts _object
	local l_facesCount = getNumFaces _object
	
	for l_faceIterator = 1 to l_facesCount do
	(
		local l_idxsFace = getFace _object l_faceIterator
		local l_materialID = 1
		
		if l_multimaterial then
		(
			l_materialID = getFaceMatID _object l_idxsFace
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
		
		--format "%\n" l_outVert1
		--format "%\n" l_outVert2
		--format "%\n\n" l_outVert3
		
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