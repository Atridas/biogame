global VERTEX_TYPE_GEOMETRY = 0x0001
global VERTEX_TYPE_NORMAL = 0x0002
global VERTEX_TYPE_TANGENT = 0x0004
global VERTEX_TYPE_BINORMAL = 0x0008
global VERTEX_TYPE_TEXTURE1 = 0x0010
global VERTEX_TYPE_TEXTURE2 = 0x0020
global VERTEX_TYPE_DIFFUSE = 0x0040

function ExportMaterial _material _file = 
(
	if _material.diffuseMapEnable and  _material.diffuseMap != undefined then
	(
		WriteShort _file _material.diffuseMap.filename.count
		WriteString _file _material.diffuseMap.filename
	)
	
	if _material.bumpMapEnable and  _material.bumpMap != undefined then
	(
		WriteShort _file _material.bumpMap.filename.count
		WriteString _file _material.bumpMap.filename
	)
	
	if _material.selfIllumMapEnable and  _material.selfIllumMap != undefined then
	(
		WriteShort _file _material.selfIllumMap.filename.count
		WriteString _file _material.selfIllumMap.filename
	)
	
	if _material.reflectionMapEnable and  _material.reflectionMap != undefined then
	(
		WriteShort _file _material.reflectionMap.filename.count
		WriteString _file _material.reflectionMap.filename
	)
)

function CreateVertex _pos _normal _tuv1 _tuv2 _diffuseColor _vertexType vertexOut_ =
(
	local l_vertexOut = #(_pos.x,_pos.y,_pos.z,_normal.x,_normal.y,_normal.z,_diffuseColor,_tuv1.x,_tuv1.y,_tuv2.x,_tuv2.y)
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
	ExportMaterial l_material l_file
	
	local l_vertexCount = getNumVerts _object
	local l_facesCount = getNumFaces _object
	
	local l_vertexList = #()
	local l_indexList = #()
	
	local l_vertexType = GetVertexType l_material
	
	for l_faceIterator = 1 to l_facesCount do
	(
		local l_idxsFace=getFace _object l_faceIterator
		
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
		
		CreateVertex l_vtx1 l_norm1 l_tuv11 l_tuv21 l_diffuse1 l_vertexType l_outVert1
		CreateVertex l_vtx2 l_norm2 l_tuv12 l_tuv22 l_diffuse2 l_vertexType l_outVert2
		CreateVertex l_vtx3 l_norm3 l_tuv13 l_tuv23 l_diffuse3 l_vertexType l_outVert3
		
		format "%\n" l_outVert1
		format "%\n" l_outVert2
		format "%\n\n" l_outVert3
		
		--append l_vertexList l_outVert1
		--append l_vertexList l_outVert2
		--append l_vertexList l_outVert3
	)
	
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
)

ExportMesh $ "D:/a.txt"