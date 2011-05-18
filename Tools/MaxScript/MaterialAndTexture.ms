-- Escriu a fitxer la informació del material
function ExportMaterial _object _material _vertexType _file _relativePath = 
(
	local l_materialMask = 0
	local l_numTextures = 0
	
	local l_numProperties = 0
	local l_bump = false
	local l_bumpPercent = 0
	local l_parallax = false
	local l_parallaxHeight = 0
	local l_specularParams = false
	
	if _material.glossiness != undefined and _material.specularLevel != undefined then
	(
		l_numProperties = l_numProperties + 1
		l_specularParams = true
	)
	
	--informació del material
	if _material.diffuseMapEnable == true and _material.diffuseMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_DIFFUSSE
		l_numTextures = l_numTextures + 1
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_NORMALMAP
		l_numTextures = l_numTextures + 1
		l_numProperties = l_numProperties + 1
		
		--propietats per a bumpmap
		l_bump = true
		l_bumpPercent = getUserProp _object "bump"
		if l_bumpPercent == undefined or l_bumpPercent < 0 or l_bumpPercent > 1 then
		(
			l_bumpPercent = DEFAULT_BUMP_VALUE
		)
		
		--propietats per parallax
		l_parallaxHeight = getUserProp _object "parallax"
		if l_parallaxHeight == undefined then
		(
			l_parallax = false
		)else
		(
			l_parallax = true
			l_numProperties = l_numProperties + 1
			
			if l_parallaxHeight < 0 or l_parallaxHeight > 1 then
			(
				l_bumpPercent = DEFAULT_PARALLAX_VALUE
			)
		)
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_LIGHTMAP
		l_numTextures = l_numTextures + 1
	)
	
	if _material.reflectionMapEnable == true and _material.reflectionMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_ENVIRONMENT
		l_numTextures = l_numTextures + 1
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_GLOW
		l_numTextures = l_numTextures + 1
	)
	
	--volcat a fitxer
	WriteShort _file _vertexType #unsigned
	WriteShort _file l_numTextures #unsigned
	
	local l_texture
	
	if _material.diffuseMapEnable == true and _material.diffuseMap != undefined then
	(
		l_texture = _material.diffuseMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		format "	->	DIFFUSE\n"
		
		WriteShort _file DIFFUSE_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		l_texture = _material.bumpMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		format "	->	NORMAL\n"
		
		WriteShort _file NORMALMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		l_texture = _material.selfIllumMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		format "	->	LIGHTMAP\n"
		
		WriteShort _file LIGHTMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.reflectionMapEnable == true and _material.reflectionMap != undefined then
	(
		l_texture = _material.reflectionMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		format "	->	SPECULAR\n"
		
		WriteShort _file SPECULARMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		l_texture = _material.filterMap.filename
		l_texture = filenameFromPath (OpenBitmap l_texture).filename
		l_texture = _relativePath + l_texture
		format "	->	GLOW\n"
		
		WriteShort _file GLOW_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	--propietats dels materials	
	WriteShort _file l_numProperties #unsigned	
	
	if l_numProperties != 0 then
	(
		if l_bump then
		(
			WriteShort _file BUMP_PROPERTY_TYPE #unsigned
			WriteFloat _file l_bumpPercent		
			format "	->	Bump percent:%\n" l_bumpPercent	
		)
		
		if l_parallax then
		(
			WriteShort _file PARALLAX_PROPERTY_TYPE #unsigned
			WriteFloat _file l_parallaxHeight
			format "	->	Parallax Property:%\n" l_parallaxHeight
		)
		
		if l_specularParams then
		(
			WriteShort _file SPECULAR_PROPERTY_TYPE #unsigned
			WriteFloat _file _material.glossiness
			WriteFloat _file _material.specularLevel
			format "	->	Specular properties Gloss:% Factor:%\n" _material.glossiness _material.specularLevel
		)
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
	else if classof(l_material) == Standardmaterial then
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