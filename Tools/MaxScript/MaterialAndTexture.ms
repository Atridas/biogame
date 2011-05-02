-- Escriu a fitxer la informació del material
function ExportMaterial _material _file _relativePath = 
(
	local l_materialMask = 0
	if _material.diffuseMapEnable == true and _material.diffuseMap != undefined then
	(
		l_materialMask = bit.or l_materialMask TEXTURE_TYPE_DIFFUSSE
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		l_materialMask = bit.or l_materialMask TEXTURE_TYPE_NORMALMAP
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		l_materialMask = bit.or l_materialMask TEXTURE_TYPE_LIGHTMAP
	)
	
	if _material.reflectionMapEnable == true and _material.reflectionMap != undefined then
	(
		l_materialMask = bit.or l_materialMask TEXTURE_TYPE_ENVIRONMENT
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		l_materialMask = bit.or l_materialMask TEXTURE_TYPE_GLOW
	)
	
	WriteShort _file l_materialMask #unsigned
	
	local l_texture
	
	if _material.diffuseMapEnable == true and _material.diffuseMap != undefined then
	(
		l_texture = _material.diffuseMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		l_texture = _material.bumpMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		l_texture = _material.selfIllumMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.reflectionMapEnable == true and _material.reflectionMap != undefined then
	(
		l_texture = _material.reflectionMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		l_texture = _material.filterMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
)

-- Comprova si el material especificat està buit
function IsMaterialEmpty _object _IDmaterial =
(
	local l_materialID
	local numFaces = getNumFaces _object
	
	for i = 1 to numFaces do
	(
		l_materialID = getFaceMatID _object i
		if (l_materialID == _IDmaterial) then
		(
				return false
		)
	
	)
	return true
)

--Retorna un array amb els índexs dels submaterials que tinguin vèrtexs a l'objecte.
function GetMaterials _object =
(
	local l_indexs = #()
	
	if _object.material == undefined then
	(
		l_indexs = undefined
		
	)else if classof _object.material == Standardmaterial then
	(
		append l_indexs 1
	)else
	(
		local l_numFaces = getNumFaces _object
	
		for i = 1 to l_numFaces do
		(
			l_materialID = getFaceMatID _object i
			if (Member l_indexs l_materialID) == false  then
			(
				append l_indexs l_materialID
			)
		
		)
	)
	
	return l_indexs
)

-- Retorna el nombre de materials amb vèrtexs per a l'objecte actual
function GetNumGoodMaterials _object =
(
	local l_num = _object.material.numsubs
	local l_total = 0
	
	for i = 1 to l_num do
	(
		if ((IsMaterialEmpty _object i) == false) then
		(
			l_total = l_total + 1	
		)
	)
	return l_total
)

-- Unkown

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
	
	if _material.filterMapEnable == true and  _material.filterMap != undefined then
	(
		l_texture = _material.filterMap.filename
		l_texture = (OpenBitmap l_texture).filename
		append l_paths l_texture
	)
	return l_paths
)

function TextureList _textures _object =
(
	local l_material = _object.material
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexos
		local l_numMaterials = l_material.numsubs
		
		for i = 1 to l_numMaterials do
		(
			local l_texTemp = TextureListOne l_material[i]
			
			for j = 1 to l_texTemp.count do
			(
				if (Member _textures l_texTemp[j]) == false then
				(
					append _textures l_texTemp[j]
				)
			)
		)
	)
	else
	(
		local l_texTemp = TextureListOne l_material
			
		for j = 1 to l_texTemp.count do
		(
			if (Member _textures l_texTemp[j]) == false then
			(
				append _textures l_texTemp[j]
			)
		)
	)
)