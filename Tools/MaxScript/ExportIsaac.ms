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
	if _material.diffuseMapEnable == true and  _material.diffuseMap != undefined then
	(
		WriteShort _file _material.diffuseMap.filename.count
		WriteString _file _material.diffuseMap.filename
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		WriteShort _file _material.bumpMap.filename.count
		WriteString _file _material.bumpMap.filename
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		WriteShort _file _material.selfIllumMap.filename.count
		WriteString _file _material.selfIllumMap.filename
	)
	
	if _material.reflectionMapEnable == true and  _material.reflectionMap != undefined then
	(
		WriteShort _file _material.reflectionMap.filename.count
		WriteString _file _material.reflectionMap.filename
	)
)

function CreateVertex _Pos _Normal _TUV1 _TUV2 _DiffuseColor _VertexType VertexOut_ =
(
	local Vtx= #(_Pos.x, _Pos.y, _Pos.z, _Normal.x, _Normal.y, _Normal.z, _DiffuseColor, _TUV1.x, _TUV1.y, _TUV2.x, _TUV2.y)
	join VertexOut_ Vtx
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

function VertexList _Obj _VertexType VertexList_ IndexList_ =
(
	local numFaces = getNumFaces _Obj
	for i = 1 to numFaces do
	(
		local face = GetFace _Obj i
		local IdxsMap=meshop.getMapFace _Obj 1 i
		local VertexOut1=#()
		local VertexOut2=#()
		local VertexOut3=#()

		-- vertex 1
		local pos =  GetVert _Obj face.x
		pos = RHVectorToLH pos
		local normal =  getNormal _Obj face.x
		normal = RHVectorToLH normal
		local TUV =  meshop.getMapVert _Obj 1 IdxsMap.x
		
		
		CreateVertex pos normal TUV TUV 0 _VertexType VertexOut1
		format "%\n" VertexOut1

		-- vertex 2
		pos =  GetVert _Obj face.y
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.y
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.y

		CreateVertex pos normal TUV TUV 0 _VertexType VertexOut2
		format "%\n" VertexOut2

		-- vertex 3
		pos =  GetVert _Obj face.z
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.z
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.z

		CreateVertex pos normal TUV TUV 0 _VertexType VertexOut3
		format "%\n" VertexOut3
		
		local index
		
		index = AddVertex VertexList_ VertexOut1
		AddIndex IndexList_ index
		
		index = AddVertex VertexList_ VertexOut2
		AddIndex IndexList_ index
		
		index = AddVertex VertexList_ VertexOut3
		AddIndex IndexList_ index
	)
)

function ExportMesh _object _filename = 
(
	local l_header = 0xAAAA
	local l_footer  =  0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	
	local l_vertexType = GetVertexType l_material
	
	ExportMaterial l_material l_file 
	
	local l_VertexList =#()
	local l_IndexList =#()
	
	VertexList _object l_vertexType l_VertexList l_IndexList
	
	format "Vertex Buffer %\nIndex Buffer %\n" l_VertexList l_IndexList
	
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
)

ExportMesh $ "D:/Temp/a.mesh"

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
