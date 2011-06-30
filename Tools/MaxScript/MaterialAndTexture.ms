-- Escriu a fitxer la informació del material
function ExportMaterial _object _material _vertexType _file _relativePath _normalRadiosity = 
(
	local l_materialMask = 0
	local l_numTextures = 0
	
	local l_numProperties = 0
	local l_bump = false
	local l_bumpPercent = 0
	local l_parallax = false
	local l_parallaxHeight = 0
	local l_specularParams = false
	local l_opacityMap = false
	local l_specularLevel = 0
	local l_glow = false
	local l_glowness = 1.0f
	
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
		if _material.OpacityMapEnable then
		(
			l_numProperties = l_numProperties + 1
			l_opacityMap = true
		)
		format "	->	DIFFUSE\n"
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_NORMALMAP
		l_numTextures = l_numTextures + 1
		l_numProperties = l_numProperties + 1
		
		--propietats per a bumpmap
		l_bump = true
		--l_bumpPercent = getUserProp _object "bump"
		l_bumpPercent = _material.bumpMapAmount / 100.0
		if l_bumpPercent == undefined or l_bumpPercent < 0 or l_bumpPercent > 1 then
		(
			l_bumpPercent = DEFAULT_BUMP_VALUE
		)
		format "	->	NORMAL\n"
		
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
				l_parallaxHeight = DEFAULT_PARALLAX_VALUE
			)
			format "	->	PARALLAX\n"
		)
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_LIGHTMAP
		l_numTextures = l_numTextures + 1
		
		if _normalRadiosity then
		(
			l_numProperties = l_numProperties + 1
			format "	-> NORMAL RADIOSITY\n"
		)
		else
		(
			format "	->	LIGHTMAP\n"
		)
	)
	
	if _material.SpecularLevelMapEnable == true and _material.SpecularLevelMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_ENVIRONMENT
		l_numTextures = l_numTextures + 1
		format "	->	SPECULAR\n"
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		--l_materialMask = bit.or l_materialMask TEXTURE_TYPE_GLOW
		l_numTextures = l_numTextures + 1
		l_numProperties = l_numProperties + 1
		format "	->	GLOW\n"
		
		l_glow = true
		local tmp = _material.filtermapamount * .02
		if tmp < 0 or tmp > 2 then
		(
			l_glowness = 1.0
		)
		else
		(
			l_glowness = tmp
		)
	)
	
	--volcat a fitxer
	WriteShort _file _vertexType #unsigned
	WriteShort _file l_numTextures #unsigned
	
	local l_texture
	
	if _material.diffuseMapEnable == true and _material.diffuseMap != undefined then
	(
		l_texture = filenameFromPath _material.diffuseMap.filename
		l_texture = _relativePath + l_texture
		
		WriteShort _file DIFFUSE_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.bumpMapEnable == true and _material.bumpMap != undefined then
	(
		l_texture = filenameFromPath _material.bumpMap.filename
		l_texture = _relativePath + l_texture
		
		WriteShort _file NORMALMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
	(
		l_texture = filenameFromPath _material.selfIllumMap.filename
		l_texture = _relativePath + l_texture
		
		WriteShort _file LIGHTMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
	)
	
	if _material.SpecularLevelMapEnable == true and _material.SpecularLevelMap != undefined then
	(
		l_texture = filenameFromPath _material.SpecularLevelMap.filename
		l_texture = _relativePath + l_texture
		
		WriteShort _file SPECULARMAP_MATERIAL_MASK #unsigned
		WriteShort _file l_texture.count #unsigned
		WriteString _file l_texture
		
		l_specularLevel = _material.SpecularLevelMapAmount * 0.03
	)
	else
	(
		l_specularLevel = _material.specularLevel
	)
	
	if _material.filterMapEnable == true and _material.filterMap != undefined then
	(
		l_texture = filenameFromPath _material.filterMap.filename
		l_texture = _relativePath + l_texture
		
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
			WriteFloat _file l_specularLevel
			format "	->	Specular properties Gloss:% Factor:%\n" _material.glossiness l_specularLevel
		)
		
		if l_opacityMap then
		(
			WriteShort _file ALPHA_PROPERTY_TYPE #unsigned
			format "	->	Diffuse has alpha\n"
		)
		
		if l_glow then
		(
			WriteShort _file GLOW_PROPERTY_TYPE #unsigned
			WriteFloat _file l_glowness
			format "	->	Glowness: %\n" l_glowness
		)
		
		if _normalRadiosity and _material.selfIllumMapEnable == true and _material.selfIllumMap != undefined then
		(
			WriteShort _file RADIOSITY_NORMAL_PROPERTY_TYPE #unsigned
			format "	->	Radiosity Normalmap:\n"
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
		append l_paths l_texture
	)
	
	if _material.bumpMapEnable == true and  _material.bumpMap != undefined then
	(
		l_texture = _material.bumpMap.filename
		append l_paths l_texture
	)
	
	if _material.selfIllumMapEnable == true and  _material.selfIllumMap != undefined then
	(
		l_texture = _material.selfIllumMap.filename
		append l_paths l_texture
	)
	
	if _material.SpecularLevelMapEnable == true and  _material.SpecularLevelMap != undefined then
	(
		l_texture = _material.SpecularLevelMap.filename
		append l_paths l_texture
	)
	
	if _material.filterMapEnable == true and  _material.filterMap != undefined then
	(
		l_texture = _material.filterMap.filename
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