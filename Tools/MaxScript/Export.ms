fileIn "GlobalsAndHelpers.ms"
fileIn "MaterialAndTexture.ms"
fileIn "IndexAndVertex.ms"
fileIn "ObjectControl.ms"

--comprova si l'objecte cumpleix les especificacions
function CheckMesh _object _checkMaterial _normalRadiosity =
(
	local l_aux = "" as stringStream
	--propietats d'objecte
	
	--objecte undefined
	if _object == undefined then
	(
		return "S'est� rebent un objecte no definit. Aquest error no hauria de passar mai, envieu el fitxer al programador."
	)
	
	--si no �s una mesh no fa falta comprovar.
	if (IsMesh _object) == false then
	(
		return true
	)
	
	local l_name = _object.name
	
	--objecte sense nom (pot passar?)
	if l_name == undefined then
	(
		return "S'est� rebent un objecte sense nom. Realment es pot crear un objecte sense nom?"
	)
	
	format "Comprovant l'objecte: %\n" l_name
	
	if classof _object == Editable_Poly or classof _object == Editable_Mesh then
	(
		if GetNumFaces _object < 1 then
		(
			return "L'objecte " + l_name + " no t� cap cara."
		)
	)
	
	local l_material = _object.material
	local l_currentMaterial = undefined
	
	if _checkMaterial then
	(
		--material
		if l_material == undefined then
		(
			return "L'objecte: " + l_name + " no t� material."
		)
		
		if classof l_material == Multimaterial then
		(
			local l_numMaterials = l_material.numsubs
			for i = 1 to l_numMaterials do
			(
				l_currentMaterial = l_material[i]
				if l_currentMaterial == undefined then
				(
					format "El multimaterial n� % de l'objecte: % no est� definit." i l_name to:l_aux
					return l_aux
				)
				
				if classof l_currentMaterial != Standardmaterial then
				(
					format "El multimaterial % de l'objecte: % no �s un material simple." i l_name to:l_aux
					return l_aux
				)
				
				if l_currentMaterial.diffuseMapEnable == true then
				(
					if l_currentMaterial.diffuseMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura difusa activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.diffuseMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura difusa." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.diffuseMap.filename == undefined then
					(
						return "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura difusa." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.diffuseMap.filename == false then
						(
							format "No es troba la textura difusa \"%\" del multimaterial % de l'objecte %." l_material[i].diffuseMap.filename i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_currentMaterial.bumpMapEnable == true then
				(
					if l_currentMaterial.bumpMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura bump activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.bumpMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura bump." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.bumpMap.filename == undefined then
					(
						format "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura bump." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.bumpMap.filename == false then
						(
							format "No es troba la textura bump \"%\" del multimaterial % de l'objecte %." l_material[i].bumpMap.filename i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_currentMaterial.selfIllumMapEnable == true then
				(
					if l_currentMaterial.selfIllumMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura d'auto il�luminaci� activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.selfIllumMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura d'auto il�luminaci�." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.selfIllumMap.filename == undefined then
					(
						format "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura d'auto il�luminaci�." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.selfIllumMap.filename == false then
						(
							format "No es troba la textura d'auto il�luminaci� \"%\" del multimaterial % de l'objecte %." l_material[i].selfIllumMap.filename i l_name to:l_aux
							return l_aux
						)
						--Comprovar radiosity
						if _normalRadiosity then
						(
							local l_filename = l_currentMaterial.selfIllumMap.filename
							local l_path = getFilenamePath l_filename
							local l_name = getFilenameFile l_filename
							local l_type = getFilenameType l_filename
							local l_radiosity = ""
							
							--Red
							l_radiosity = l_path + l_name + "_R" + l_type
							if doesFileExist l_radiosity == false then
							(
								format "No es troba la textura RED del radiosity \"%\" del multimaterial % de l'objecte %." l_material[i].selfIllumMap.filename i l_name to:l_aux
								return l_aux
							)
							
							--Green
							l_radiosity = l_path + l_name + "_G" + l_type
							if doesFileExist l_radiosity == false then
							(
								format "No es troba la textura GREEN del radiosity \"%\" del multimaterial % de l'objecte %." l_material[i].selfIllumMap.filename i l_name to:l_aux
								return l_aux
							)
							
							--Blue
							l_radiosity = l_path + l_name + "_B" + l_type
							if doesFileExist l_radiosity == false then
							(
								format "No es troba la textura BLUE del radiosity \"%\" del multimaterial % de l'objecte %." l_material[i].selfIllumMap.filename i l_name to:l_aux
								return l_aux
							)
						)
					)				
				)
				
				if l_currentMaterial.SpecularLevelMapEnable == true then
				(
					if l_currentMaterial.SpecularLevelMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura especular activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.SpecularLevelMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura de especular." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.SpecularLevelMap.filename == undefined then
					(
						format "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura de especular." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.SpecularLevelMap.filename == false then
						(
							format "No es troba la textura de especular \"%\" del multimaterial % de l'objecte %." l_material[i].SpecularLevelMap.filename i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_currentMaterial.filterMapEnable == true then
				(
					if l_currentMaterial.filterMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura de filtre activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.filterMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura de filtre." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.filterMap.filename == undefined then
					(
						format "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura de filtre." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.filterMap.filename == false then
						(
							format "No es troba la textura de filtre \"%\" del multimaterial % de l'objecte %." l_material[i].filterMap.filename i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_currentMaterial.reflectionMapEnable == true then
				(
					if l_currentMaterial.reflectionMap == undefined then
					(
						format "El multimaterial % de l'objecte % t� la textura de reflexi� activada per� no definida." i l_name to:l_aux
						return l_aux
					)else if classof l_currentMaterial.reflectionMap != Bitmaptexture then
					(
						format "El multimaterial % de l'objecte % no t� un Bitmap com a textura de reflexi�." i l_name to:l_aux
						return l_aux
					)else if l_currentMaterial.reflectionMap.filename == undefined then
					(
						format "El multimaterial % de l'objecte % t� un Bitmap no definit com a textura de reflexi�." i l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_currentMaterial.reflectionMap.filename == false then
						(
							format "No es troba la textura de reflexi� \"%\" del multimaterial % de l'objecte %." l_material[i].reflectionMap.filename i l_name to:l_aux
							return l_aux
						)
					)				
				)
			)
			
		)else
		(
			if classof l_material != Standardmaterial then
			(
				return "El material de l'objecte: " + l_name + " no �s ni Standard ni Multimaterial."
			)
				
			if l_material.diffuseMapEnable == true then
			(
				if l_material.diffuseMap == undefined then
				(
					return "L'objecte " + l_name + " t� la textura difusa activada per� no definida."
				)else if classof l_material.diffuseMap != Bitmaptexture then
				(
					return "L'objecte " + l_name + " no t� un Bitmap com a textura difusa."
				)else if l_material.diffuseMap.filename == undefined then
				(
					return "L'objecte " + l_name + " t� un Bitmap no definit com a textura difusa."
				)else
				(
					if doesFileExist l_material.diffuseMap.filename == false then
					(
						return "No es troba la textura difusa \"" + l_material.diffuseMap.filename + "\" de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.bumpMapEnable == true then
			(
				if l_material.bumpMap == undefined then
				(
					return "L'objecte " + l_name + " t� la textura bump activada per� no definida."
				)else if classof l_material.bumpMap != Bitmaptexture then
				(
					return "L'objecte " + l_name + " no t� un Bitmap com a textura bump."
				)else if l_material.bumpMap.filename == undefined then
				(
					return "L'objecte " + l_name + " t� un Bitmap no definit com a textura de bump."
				)else
				(
					if doesFileExist l_material.bumpMap.filename == false then
					(
						return "No es troba la textura bump \"" + l_material.bumpMap.filename + "\" de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.selfIllumMapEnable == true then
			(
				if l_material.selfIllumMap == undefined then
				(
					return "L'objecte " + l_name + " t� la textura d'auto il�luminaci� activada per� no definida."
				)else if classof l_material.selfIllumMap != Bitmaptexture then
				(
					return "L'objecte " + l_name + " no t� un Bitmap com a textura d'auto il�luminaci�."
				)else if l_material.selfIllumMap.filename == undefined then
				(
					return "L'objecte " + l_name + " t� un Bitmap no definit com a textura d'auto il�luminaci�.."
				)else
				(
					if doesFileExist l_material.selfIllumMap.filename == false then
					(
						return "No es troba la textura d'auto il�luminaci� \"" + l_material.selfIllumMap.filename + "\" de l'objecte " + l_name + "."
					)
					--Comprovar radiosity
					if _normalRadiosity then
					(
						local l_filename = l_material.selfIllumMap.filename
						local l_path = getFilenamePath l_filename
						local l_name = getFilenameFile l_filename
						local l_type = getFilenameType l_filename
						local l_radiosity = ""
						
						--Red
						l_radiosity = l_path + l_name + "_R" + l_type
						if doesFileExist l_radiosity == false then
						(
							return "No es troba la textura RED del radiosity \"" + l_material.selfIllumMap.filename + "\" de l'objecte " + l_name + "."
							return l_aux
						)
						
						--Green
						l_radiosity = l_path + l_name + "_G" + l_type
						if doesFileExist l_radiosity == false then
						(
							return "No es troba la textura GREEN del radiosity \"" + l_material.selfIllumMap.filename + "\" de l'objecte " + l_name + "."
							return l_aux
						)
						
						--Blue
						l_radiosity = l_path + l_name + "_B" + l_type
						if doesFileExist l_radiosity == false then
						(
							return "No es troba la textura BLUE del radiosity \"" + l_material.selfIllumMap.filename + "\" de l'objecte " + l_name + "."
							return l_aux
						)
					)
				)				
			)
			
			if l_material.SpecularLevelMapEnable == true then
			(
				if l_material.SpecularLevelMap == undefined then
				(
					return "L'objecte " + l_name + " t� la textura de especular activada per� no definida."
				)else if classof l_material.SpecularLevelMap != Bitmaptexture then
				(
					return "L'objecte " + l_name + " no t� un Bitmap com a textura de especular."
				)else if l_material.SpecularLevelMap.filename == undefined then
				(
					return "L'objecte " + l_name + " t� un Bitmap no definit com a textura de especular."
				)else
				(
					if doesFileExist l_material.SpecularLevelMap.filename == false then
					(
						return "No es troba la textura de especular \"" + l_material.SpecularLevelMap.filename + "\" de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.filterMapEnable == true then
			(
				if l_material.filterMap == undefined then
				(
					return "L'objecte " + l_name + " t� la textura de filtre activada per� no definida."
				)else if classof l_material.filterMap != Bitmaptexture then
				(
					return "L'objecte " + l_name + " no t� un Bitmap com a textura de filtre."
				)else if l_material.filterMap.filename == undefined then
				(
					return "L'objecte " + l_name + " t� un Bitmap no definit com a textura de filtre."
				)else
				(
					if doesFileExist l_material.filterMap.filename == false then
					(
						return "No es troba la textura de filtre \"" + l_material.filterMap.filename + "\" de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.reflectionMapEnable == true then
				(
					if l_material.reflectionMap == undefined then
					(
						format "L'objecte % t� la textura de reflexi� activada per� no definida." l_name to:l_aux
						return l_aux
					)else if classof l_material.reflectionMap != Bitmaptexture then
					(
						format "L'objecte % no t� un Bitmap com a textura de reflexi�." l_name to:l_aux
						return l_aux
					)else if l_material.reflectionMap.filename == undefined then
					(
						format "L'objecte % t� un Bitmap no definit com a textura de reflexi�." l_name to:l_aux
						return l_aux
					)else
					(
						if doesFileExist l_material.reflectionMap.filename == false then
						(
							format "No es troba la textura de reflexi� \"%\" de l'objecte %." l_material[i].reflectionMap.filename l_name to:l_aux
							return l_aux
						)
					)				
				)
		)
	)
	
	--other
	
	return true
)

-- comprova si un array d'objectes compleix les especificacions
function CheckSceneObjects _objectArray _normalRadiosity = 
(
	--s'evita tornar a comprovar els materials, ja que al mirar les rutes dels fitxers es relenteix molt.
	local l_materials = #()
	local l_mat
	local l_check
	for i = 1 to _objectArray.count do
	(
		local l_mat = _objectArray[i].material
		if (Member l_materials l_mat) == false then
		(
			append l_materials l_mat
			l_check = CheckMesh _objectArray[i] true _normalRadiosity
			
			if l_check != true then
			(
				return l_check
			)
		)else
		(
			l_check = CheckMesh _objectArray[i] false _normalRadiosity
			
			if l_check != true then
			(
				return l_check
			)
		)
	)
	
	return true
)

function ExportMesh _object0 _filename _relativeTexturePath _normalRadiosity = 
(
	format "\nExportant objecte: % com a core %\n" _object0.name (GetCoreName _object0)
	
	if _object0.material == undefined then
	(
		ShowError ("No hi ha material a " + _object0.name)
		return false
	)
	
	_object = copy _object0
	convertToMesh _object
	local l_properties = getUserPropBuffer _object0
	setUserPropBuffer _object l_properties
	UABCalcVertexsNormals _object
	
	local l_header = 0xAAAA
	local l_footer  =  0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	
	local l_materialInfo
	
	local l_numMaterials
	-- llista de vertexTypes, amb un sol element si no �s un multimaterial
	local l_vertexType = #()
	--lista d'indexs de materials de l'objecte. Nom�s aquells dels que tigui v�rtexs.
	local l_objectMaterials = GetMaterials _object
	
	if l_objectMaterials == undefined then
	(
		ShowError (_object0.name + " no t� cap v�rtexs que tingui material")
		delete _object
		
		fflush l_file
		fclose l_file
		deleteFile _filename
		
		return false
	)
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexs
		l_numMaterials = l_material.numsubs
		local l_RealNumMaterials = GetNumGoodMaterials _object
		WriteShort l_file l_RealNumMaterials #unsigned
		
		
		for i=1 to l_objectMaterials.count do
		(
			if l_objectMaterials[i] < 1 or l_objectMaterials[i] > l_numMaterials then
			(
				local l_aux = "" as stringStream
				format "L'objecte % t� una cara amb el material ID %, que no existeix!!!" _object.name l_objectMaterials[i] to:l_aux
				ShowError l_aux
				delete _object
				
				fflush l_file
				fclose l_file
				deleteFile _filename
				
				return false
			)
		)
		
		l_objectMaterials = GetMaterials _object
		
		for i = 1 to l_numMaterials do
		(
			--afegim a la llista el tipus de vertex
			local l_vertexType2 = GetVertexType l_material[i]
			append l_vertexType l_vertexType2
						
			--l_materialInfo = MATERIAL_INFO_VERTEXTYPE
			--l_materialInfo = bit.or l_materialInfo MATERIAL_INFO_TEXTURETYPE
			
			if Member l_objectMaterials i then
			(
				--comprovar que el material tingui les coordenades necessaries
				if ((bit.and l_vertexType2 VERTEX_TYPE_TEXTURE2) != 0) and ((meshop.getNumMaps _object) < 3) then
				(
					local l_aux = "" as stringStream
					format "L'objecte % amb submaterial % t� lightmap activat sense segones coordenades de textura." _object.name i to:l_aux
					ShowError l_aux
					delete _object
					
					fflush l_file
					fclose l_file
					deleteFile _filename
					
					return false
				)
				
				format "Exportant material: % amb v�rtex %\n" i l_vertexType2
		
				--WriteShort l_file l_materialInfo #unsigned
				--tipus de vertex i material en si
				--WriteShort l_file l_vertexType2 #unsigned
				ExportMaterial _object l_material[i] l_vertexType2 l_file _relativeTexturePath _normalRadiosity
			)
		)
	)
	else
	(
		append l_objectMaterials l_material
		--nombre de materials
		l_numMaterials = 1
		WriteShort l_file l_numMaterials #unsigned
		local l_vertexType2 = GetVertexType l_material
		append l_vertexType l_vertexType2
		
		if ((bit.and l_vertexType2 VERTEX_TYPE_TEXTURE2) != 0) and ((meshop.getNumMaps _object) < 3) then
		(
			local l_aux = "" as stringStream
			format "L'objecte % t� lightmap activat sense segones coordenades de textura." _object.name to:l_aux
			ShowError l_aux
			delete _object
			
			fflush l_file
			fclose l_file
			deleteFile _filename
			
			return false
		)
		
		--l_materialInfo = MATERIAL_INFO_VERTEXTYPE
		--l_materialInfo = bit.or l_materialInfo MATERIAL_INFO_TEXTURETYPE
		format "Unimaterial amb v�rtex %\n" l_vertexType2
    
		--WriteShort l_file l_materialInfo #unsigned
		--tipus de vertex i material en si
		--WriteShort l_file l_vertexType2 #unsigned
		ExportMaterial _object l_material l_vertexType2 l_file _relativeTexturePath _normalRadiosity
	)
  
	local l_VertexList =#()
	local l_IndexList =#()
	
	VertexList _object l_vertexType l_numMaterials l_VertexList l_IndexList
	
	if classof l_material == Standardmaterial then
	(
		format "Exportant material simple\n"
		WriteVertexs l_file l_VertexList[1] l_vertexType[1]
		WriteIndexs l_file l_IndexList[1]
	)else
	(
		for i = 1 to l_numMaterials do
		(
			if Member l_objectMaterials i then
			(
				local l_aux = "" as stringStream
				format "Exportant submaterial \n"
				WriteVertexs l_file l_VertexList[i] l_vertexType[i] 
				WriteIndexs l_file l_IndexList[i]
			)
		)
	)
	
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
	
	delete _object
	
	return true;
)