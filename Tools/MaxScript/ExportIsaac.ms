global VERTEX_TYPE_GEOMETRY =0x0001
global VERTEX_TYPE_NORMAL     =0x0002
global VERTEX_TYPE_TANGENT    =0x0004
global VERTEX_TYPE_BINORMAL  =0x0008
global VERTEX_TYPE_TEXTURE1   =0x0010
global VERTEX_TYPE_TEXTURE2   =0x0020
global VERTEX_TYPE_DIFFUSE      =0x0040

fn GetYaw transform =
(
	return (transform as eulerangles).z
)

fn GetPitch transform =
(
	return (transform as eulerangles).x
)
fn GetRoll transform =
(
	return (transform as eulerangles).y
)

fn RHVectorToLH vector =
(
	return point3 vector.x vector.z vector.y
)

fn ChangeUVCoords UV =
(
	local v = 1-Uv.y
	return point3 UV.x v UV.z
)

function GetVertexType _material =
(
	local mascara = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	if _material.diffuseMapEnable then
	(
		if _material.selfIllumMapEnable then
		(
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE2
		) else (
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE1
		)
		if _material.bumpMapEnable then
		(
			mascara = bit.or mascara VERTEX_TYPE_TANGENT
			mascara = bit.or mascara VERTEX_TYPE_BINORMAL
		)
	) else (
		mascara = bit.or mascara VERTEX_TYPE_DIFFUSE
	)
	
	return mascara
)

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
	
	if _material.diffuseMapEnable == true and  _material.diffuseMap != undefined then
	(
		WriteShort _file _material.diffuseMap.filename.count #unsigned
		WriteString _file _material.diffuseMap.filename
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		WriteShort _file _material.bumpMap.filename.count #unsigned
		WriteString _file _material.bumpMap.filename
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		WriteShort _file _material.selfIllumMap.filename.count #unsigned
		WriteString _file _material.selfIllumMap.filename
	)
	
	if _material.reflectionMapEnable == true and  _material.reflectionMap != undefined then
	(
		WriteShort _file _material.reflectionMap.filename.count #unsigned
		WriteString _file _material.reflectionMap.filename
	)
)

--global VERTEX_TYPE_GEOMETRY =0x0001
--global VERTEX_TYPE_NORMAL     =0x0002
--global VERTEX_TYPE_TANGENT    =0x0004
--global VERTEX_TYPE_BINORMAL  =0x0008
--global VERTEX_TYPE_TEXTURE1   =0x0010
--global VERTEX_TYPE_TEXTURE2   =0x0020
--global VERTEX_TYPE_DIFFUSE      =0x0040

function CreateVertex _Pos _Normal _TUV1 _TUV2 _DiffuseColor _VertexType VertexOut_ =
(
	--local Vtx= #(_Pos.x, _Pos.y, _Pos.z, _Normal.x, _Normal.y, _Normal.z, _DiffuseColor, _TUV1.x, _TUV1.y, _TUV2.x, _TUV2.y)
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
	if bit.and _VertexType VERTEX_TYPE_TEXTURE1 > 0 then
	(
		append VertexOut_ _TUV1.x
		append VertexOut_ _TUV1.y
	)
	if bit.and _VertexType VERTEX_TYPE_TEXTURE2 > 0 then
	(
		append VertexOut_ _TUV1.x
		append VertexOut_ _TUV1.y
		append VertexOut_ _TUV2.x
		append VertexOut_ _TUV2.y
	)
	if bit.and _VertexType VERTEX_TYPE_DIFFUSE > 0 then
	(
		append VertexOut_ _DiffuseColor
	)
	join VertexOut_ Vtx
)

function WriteVertexs _File _VertexList _VertexType =
(
	WriteShort _File _VertexList.count #unsigned
	for i= 1 to _VertexList.count do
	(
		local l_vertex = _VertexList[i]
		
		local l_result =  bit.and _VertexType VERTEX_TYPE_GEOMETRY
		
		for j= 1 to l_vertex.count do
		(
			WriteFloat _File l_vertex[j]
		)
		
		/*
		if bit.and _VertexType VERTEX_TYPE_GEOMETRY > 0 then
		(
			WriteFloat _File l_vertex[1]
			WriteFloat _File l_vertex[2]
			WriteFloat _File l_vertex[3]
		)
		if bit.and _VertexType VERTEX_TYPE_NORMAL > 0 then
		(
			WriteFloat _File l_vertex[4]
			WriteFloat _File l_vertex[5]
			WriteFloat _File l_vertex[6]
		)
		if bit.and _VertexType VERTEX_TYPE_TEXTURE1 > 0 then
		(
			WriteFloat _File l_vertex[8]
			WriteFloat _File l_vertex[9]
		) else if bit.and _VertexType VERTEX_TYPE_TEXTURE2 > 0 then
		(
			WriteFloat _File l_vertex[8]
			WriteFloat _File l_vertex[9]
			WriteFloat _File l_vertex[10]
			WriteFloat _File l_vertex[11]
		) else if bit.and _VertexType VERTEX_TYPE_DIFFUSE > 0 then
		(
			WriteLong _File l_vertex[7] #unsigned
		)*/
	)
)
function WriteIndexs _File _IndexList =
(
	WriteShort _File _IndexList.count #unsigned
	for i= 1 to _IndexList.count do
	(
		WriteShort _File _IndexList[i] #unsigned
	)
)

function SameVertex _Vertex1 _Vertex2=
(
	if _Vertex1.count != _Vertex2.count then
	(
		return false
	)
	for i = 1 to _Vertex1.count do
	(
		if _Vertex1[i] != _Vertex2[i] then
		(
			return false
		)
	)
	return true
)

function AddVertex _VertexList_ _Vertex =
(
	for i = 1 to _VertexList_.count do
	(
		if SameVertex _VertexList_[i] _Vertex then
		(
			return i-1
		)
	)
	append _VertexList_ _Vertex
	return _VertexList_.count - 1
)

function AddIndex _IndexList_ _Index =
(
	append _IndexList_ _Index
)

--construim les llistes de vertexos i indexos
function VertexList _Obj _VertexType _numMaterials VertexList_ IndexList_ =
(
	-- si tenim un multi material, creem les llistes de vertexos i indexos, sino, en creem només una
	-- per simplificar les coses després a l'hora d'escriure'ls
	for i=1 to _numMaterials do
	(
		append VertexList_ #()
		append IndexList_ #()
	)
	
	local l_materialID
	
	local numFaces = getNumFaces _Obj
	for i = 1 to numFaces do
	(
		local face = GetFace _Obj i
		local IdxsMap=meshop.getMapFace _Obj 1 i
		local VertexOut1=#()
		local VertexOut2=#()
		local VertexOut3=#()
		if _numMaterials > 1 then -- agafem el material, en cas de ser multimaterial
		(
			l_materialID = getFaceMatID _Obj i
		)
		else
		(
			l_materialID = 1
		)

		-- vertex 1
		local pos =  GetVert _Obj face.x
		pos = RHVectorToLH pos
		local normal =  getNormal _Obj face.x
		normal = RHVectorToLH normal
		local TUV =  meshop.getMapVert _Obj 1 IdxsMap.x
		TUV = ChangeUVCoords TUV
		
		
		CreateVertex pos normal TUV TUV 0 _VertexType[l_materialID] VertexOut1
		--format "%\n" VertexOut1

		-- vertex 2
		pos =  GetVert _Obj face.y
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.y
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.y
		TUV = ChangeUVCoords TUV

		CreateVertex pos normal TUV TUV 0 _VertexType[l_materialID] VertexOut2
		--format "%\n" VertexOut2

		-- vertex 3
		pos =  GetVert _Obj face.z
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.z
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.z
		TUV = ChangeUVCoords TUV

		CreateVertex pos normal TUV TUV 0 _VertexType[l_materialID] VertexOut3
		--format "%\n" VertexOut3
		
		local index1
		local index2
		local index3
		
		index1 = AddVertex VertexList_[l_materialID] VertexOut1
		index2 = AddVertex VertexList_[l_materialID] VertexOut2
		index3 = AddVertex VertexList_[l_materialID] VertexOut3
		
		AddIndex IndexList_[l_materialID] index1
		AddIndex IndexList_[l_materialID] index3
		AddIndex IndexList_[l_materialID] index2
	)
)

function ExportMesh _object _filename = 
(
	local l_header = 0xAAAA
	local l_footer  =  0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	
	local l_numMaterials --nombre de materials
	local l_vertexType = #() -- llista de vertexTypes, amb un sol element si no és un multimaterial
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexos
		l_numMaterials = l_material.numsubs
		WriteShort l_file l_numMaterials #unsigned
		
		for i = 1 to l_numMaterials do
		(
			--afegim a la llista el tipus de vertex
			local l_vertexType2 = GetVertexType l_material[i]
			append l_vertexType l_vertexType2
		
			--tipus de vertex i material en si
			WriteShort l_file l_vertexType2 #unsigned
			ExportMaterial l_material[i] l_file 
		)
	)
	else
	(
		
		--nombre de materials
		l_numMaterials = 1
		WriteShort l_file l_numMaterials #unsigned
		local l_vertexType2 = GetVertexType l_material
		append l_vertexType l_vertexType2
		
		--tipus de vertex i material en si
		WriteShort l_file l_vertexType2 #unsigned
		ExportMaterial l_material l_file 
	)
	local l_VertexList =#()
	local l_IndexList =#()
	
	VertexList _object l_vertexType l_numMaterials l_VertexList l_IndexList
	
	for i = 1 to l_numMaterials do
	(
		WriteVertexs l_file l_VertexList[i] l_vertexType[i] 
		WriteIndexs l_file l_IndexList[i]
	)
	
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
)

ExportMesh $ "D:\Temp/b.mesh"
/*
list = #()

--VertexList $ 0 list

format "%\n" list


getNumFaces $

face = GetFace $ 1
pos =  GetVert $ face.x
normal =  getNormal $ face.x
IdxsMap=meshop.getMapFace $ 1 1
TUV =  meshop.getMapVert $ 1 IdxsMap.x

--format face
--format pos

*/
