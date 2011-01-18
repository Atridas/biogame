global VERTEX_TYPE_GEOMETRY =0x0001
global VERTEX_TYPE_NORMAL     =0x0002
global VERTEX_TYPE_TANGENT    =0x0004
global VERTEX_TYPE_BINORMAL  =0x0008
global VERTEX_TYPE_TEXTURE1   =0x0010
global VERTEX_TYPE_TEXTURE2   =0x0020
global VERTEX_TYPE_DIFFUSE      =0x0040


--global GAME_PATH = "Data/Assets/Textures/"

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

function ExportMaterial _material _file _relativePath = 
(
	/*
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
	*/
	local l_texture
	
	if _material.diffuseMapEnable == true and  _material.diffuseMap != undefined then
	(
		l_texture = _material.diffuseMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		l_texture = _material.bumpMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		l_texture = _material.selfIllumMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.reflectionMapEnable == true and  _material.reflectionMap != undefined then
	(
		l_texture = _material.reflectionMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
)

function TextureListOne _material = 
(
	local l_texture
	local l_paths = #()
	
	if _material.diffuseMapEnable == true and  _material.diffuseMap != undefined then
	(
		l_texture = _material.diffuseMap.filename
		l_texture = (OpenBitmap l_texture).filename
		append l_paths l_texture
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		l_texture = _material.bumpMap.filename
		l_texture = (OpenBitmap l_texture).filename
		append l_paths l_texture
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		l_texture = _material.selfIllumMap.filename
		l_texture = (OpenBitmap l_texture).filename
		append l_paths l_texture
	)
	
	if _material.reflectionMapEnable == true and  _material.reflectionMap != undefined then
	(
		l_texture = _material.reflectionMap.filename
		l_texture = (OpenBitmap l_texture).filename
		append l_paths l_texture
	)
	return l_paths
)

function TextureList _object =
(
	local l_material = _object.material
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexos
		local l_numMaterials = l_material.numsubs
		
		local l_textures = #() 
		
		for i = 1 to l_numMaterials do
		(
			local l_texTemp = TextureListOne l_material[i]
			join l_textures l_texTemp
		)
		return l_textures
	)
	else
	(
		return TextureListOne l_material
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

function WriteVertexs _File _VertexList _VertexType =
(
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
function WriteIndexs _File _IndexList =
(
	WriteLong _File _IndexList.count #unsigned
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

function AddVertex _VertexList_ _MaterialID _Vertex =
(
	/*
	for i = 1 to _VertexList_.count do
	(
		if SameVertex _VertexList_[i] _Vertex then
		(
			return i-1
		)
	)*/
	
	local l_index = uab_add_vtx_on_vertex_buffer _MaterialID _Vertex
	
	if l_index == _VertexList_[_MaterialID].count then
	(
		append _VertexList_[_MaterialID] _Vertex
	)
	return l_index
)

function AddIndex _IndexList_ _Index =
(
	append _IndexList_ _Index
)

function CreateColorLong _Color =
(
	local l_r = (floor _Color.r) as Integer64
	local l_g = (floor _Color.g) as Integer64
	local l_b = (floor _Color.b) as Integer64
	local l_a = (floor _Color.a) as Integer64
	return l_r * 256 * 256 * 256 + l_g * 256 * 256 + l_b * 256 + l_a
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
	
	-- netejem els vertex buffers del plug-in
	uab_clear_vertex_buffer()
	
	local l_materialID
	local l_Color
	local l_invTransform = inverse _Obj.transform
	
	local numFaces = getNumFaces _Obj
	for i = 1 to numFaces do
	(
		local face = GetFace _Obj i
		local IdxsMap=meshop.getMapFace _Obj 1 i
		local l_FaceNormal =  GetFaceNormal _Obj i
		l_FaceNormal = RHVectorToLH l_FaceNormal
		local l_SmoothValue = getFaceSmoothGroup _Obj i
		local VertexOut1=#()
		local VertexOut2=#()
		local VertexOut3=#()
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

		-- vertex 1
		local pos =  GetVert _Obj face.x
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		local TUV =  meshop.getMapVert _Obj 1 IdxsMap.x
		TUV = ChangeUVCoords TUV
		-- normals
		--UABGetVertexNormal obj IdVertex SmoothValue FaceNormal
		local l_Normal = UABGetVertexNormal _Obj face.x l_SmoothValue l_FaceNormal
		
		
		CreateVertex pos l_Normal TUV TUV l_Color _VertexType[l_materialID] VertexOut1
		--format "%\n" VertexOut1

		-- vertex 2
		pos =  GetVert _Obj face.y
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.y
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.y
		TUV = ChangeUVCoords TUV
		local l_Normal = UABGetVertexNormal _Obj face.y l_SmoothValue l_FaceNormal

		CreateVertex pos l_Normal TUV TUV l_Color _VertexType[l_materialID] VertexOut2
		--format "%\n" VertexOut2

		-- vertex 3
		pos =  GetVert _Obj face.z
		pos = pos * l_invTransform
		pos = RHVectorToLH pos
		normal =  getNormal _Obj face.z
		normal = RHVectorToLH normal
		TUV =  meshop.getMapVert _Obj 1 IdxsMap.z
		TUV = ChangeUVCoords TUV
		local l_Normal = UABGetVertexNormal _Obj face.z l_SmoothValue l_FaceNormal

		CreateVertex pos l_Normal TUV TUV l_Color _VertexType[l_materialID] VertexOut3
		--format "%\n" VertexOut3
		
		local index1
		local index2
		local index3
		
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

function ExportMesh _object0 _filename _relativeTexturePath = 
(
	format "Exporting % \n" _object0.name
	
	_object = copy _object0
	convertToMesh _object
	UABCalcVertexsNormals _object
	
	local l_header = 0xAAAA
	local l_footer  =  0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	
	if l_material == undefined then
	(
		ShowError ("No hi ha material a " + _object.name)
		return false
	)
	
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
			ExportMaterial l_material[i] l_file _relativeTexturePath
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
		ExportMaterial l_material l_file  _relativeTexturePath
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
	
	delete _object
	
	return true;
)

function ExportPop =
(
	local l_outName = GetSaveFileName caption:"Selecciona on guardar la malla" types:"mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
	if l_outName != undefined then
	(
		ExportMesh $ l_outName
	)
	
)

function ShowError _text =
(
	messageBox _text title:"Attention"
)

function ShowMessage _text =
(
	messageBox _text title:"Fine" beep:false
)


global UABVtxsNormals=#()

function UABClearNormalsArray =
(
	while UABVtxsNormals.count>0 do
	(
		deleteItem UABVtxsNormals 1
	)
)


function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask=2^(IdGroup-1)
	return (bit.and Value ValueMask)==ValueMask
)


function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal=false
	local Normal=point3 0 0 0
	for b=1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal=Normal+UABVtxsNormals[IdVertex][b]
			HasNormal=true
		)
	)
	if HasNormal==false then
	(
		Normal=FaceNormal
	)
	return Normal
)


function UABCalcVertexsNormals obj =
(
	format "Calculant Normals...\n"
	
	UABClearNormalsArray()
	local NumVtxs=getNumVerts obj
	for b=1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		for t=1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)
	
	local NumFaces=getNumFaces obj
	local InvTransform=inverse obj.transform
	for IdFace=1 to NumFaces do
	(
		local IdxsFace=getFace obj IdFace
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform
		local FaceNormal=getFaceNormal obj IdFace
		local SmoothValue=getFaceSmoothGroup obj IdFace
		
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + FaceNormal
			)
		)
	)
)
