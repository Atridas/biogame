fileIn "GlobalsAndHelpers.ms"
fileIn "MaterialAndTexture.ms"
fileIn "IndexAndVertex.ms"
fileIn "ObjectControl.ms"

-- comprova si un array d'objectes compleix les especificacions
function CheckSceneObjects _objectArray = 
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
			l_check = CheckMesh _objectArray[i] true
			
			if l_check != true then
			(
				return l_check
			)
		)else
		(
			l_check = CheckMesh _objectArray[i] false
			
			if l_check != true then
			(
				return l_check
			)
		)
	)
	
	return true
)

--comprova si l'objecte cumpleix les especificacions
function CheckMesh _object _checkMaterial =
(
	local l_aux = "" as stringStream
	--propietats d'objecte
	
	--objecte undefined
	if _object == undefined then
	(
		return "S'està rebent un objecte no definit. Aquest error no hauria de passar mai, envieu el fitxer al programador."
	)
	
	--si no es cap d'aquestes classes no es tracta, per tant es considera correcte. Pot tractar-se de càmares o altres coses que es posin per l'escena.
	if (classof _object == TargetDirectionallight or classof _object == Omnilight or classof _object == targetSpot or classof _object == Dummy or classof _object == Editable_Poly or classof _object == Box or classof _object == Editable_mesh or classof _object == Cylinder or classof _object == GeoSphere) == false then
	(
		return true
	)
	
	local l_name = _object.name
	
	--objecte sense nom (pot passar?)
	if l_name == undefined then
	(
		return "S'està rebent un objecte sense nom. Realment es pot crear un objecte sense nom?"
	)
	
	local l_material = _object.material
	
	if _checkMaterial then
	(
		--material
		if l_material == undefined then
		(
			return "L'objecte: " + l_name + " no té material."
		)
		
		if classof l_material == Multimaterial then
		(		
			for i = 1 to l_material.count do
			(
				if l_material[i] == undefined then
				(
					format "El multimaterial nº % de l'objecte: % no està definit." i l_name to:l_aux
					return l_aux
				)
				
				if classof l_material[i] != Standardmaterial then
				(
					format "El multimaterial % de l'objecte: % no és un material simple." i l_name to:l_aux
					return l_aux
				)
				
				if l_material[i].diffuseMapEnable == true then
				(
					if l_material[i].diffuseMap == undefined then
					(
						format "El multimaterial % de l'objecte % té la textura difusa activada però no definida." i l_name to:l_aux
						return l_aux
					)else
					(
						if OpenBitmap l_material[i].diffuseMap.filename == undefined then
						(
							format "No es troba la textura difusa del multimaterial % de l'objecte %." i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_material[i].bumpMapEnable == true then
				(
					if l_material[i].bumpMap == undefined then
					(
						format "El multimaterial % de l'objecte % té la textura bump activada però no definida." i l_name to:l_aux
						return l_aux
					)else
					(
						if OpenBitmap l_material[i].bumpMap.filename == undefined then
						(
							format "No es troba la textura bump del multimaterial % de l'objecte %." i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_material[i].selfIllumMapEnable == true then
				(
					if l_material[i].selfIllumMap == undefined then
					(
						format "El multimaterial % de l'objecte % té la textura d'auto il·luminació activada però no definida." i l_name to:l_aux
						return l_aux
					)else
					(
						if OpenBitmap l_material[i].selfIllumMap.filename == undefined then
						(
							format "No es troba la textura d'auto il·luminació del multimaterial % de l'objecte %." i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_material[i].reflectionMapEnable == true then
				(
					if l_material[i].reflectionMap == undefined then
					(
						format "El multimaterial % de l'objecte % té la textura de reflexió activada però no definida." i l_name to:l_aux
						return l_aux
					)else
					(
						if OpenBitmap l_material[i].reflectionMap.filename == undefined then
						(
							format "No es troba la textura de reflexió del multimaterial % de l'objecte %." i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
				if l_material[i].filterMapEnable == true then
				(
					if l_material[i].filterMap == undefined then
					(
						format "El multimaterial % de l'objecte % té la textura de filtre activada però no definida." i l_name to:l_aux
						return l_aux
					)else
					(
						if OpenBitmap l_material[i].filterMap.filename == undefined then
						(
							format "No es troba la textura de filtre del multimaterial % de l'objecte %." i l_name to:l_aux
							return l_aux
						)
					)				
				)
				
			)
		)else
		(
			if classof l_material != Standardmaterial then
			(
				return "El material de l'objecte: " + l_name + " no és ni Standard ni Multimaterial."
			)
				
			if l_material.diffuseMapEnable == true then
			(
				if l_material.diffuseMap == undefined then
				(
					return "L'objecte " + l_name + " té la textura difusa activada però no definida."
				)else
				(
					if OpenBitmap l_material.diffuseMap.filename == undefined then
					(
						return "No es troba la textura difusa de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.bumpMapEnable == true then
			(
				if l_material.bumpMap == undefined then
				(
					return "L'objecte " + l_name + " té la textura bump activada però no definida."
				)else
				(
					if OpenBitmap l_material.bumpMap.filename == undefined then
					(
						return "No es troba la textura bump de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.selfIllumMapEnable == true then
			(
				if l_material.selfIllumMap == undefined then
				(
					return "L'objecte " + l_name + " té la textura d'auto il·luminació activada però no definida."
				)else
				(
					if OpenBitmap l_material.selfIllumMap.filename == undefined then
					(
						return "No es troba la textura d'auto il·luminació de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.reflectionMapEnable == true then
			(
				if l_material.reflectionMap == undefined then
				(
					return "L'objecte " + l_name + " té la textura de reflexió activada però no definida."
				)else
				(
					if OpenBitmap l_material.reflectionMap.filename == undefined then
					(
						return "No es troba la textura de reflexió de l'objecte " + l_name + "."
					)
				)				
			)
			
			if l_material.filterMapEnable == true then
			(
				if l_material.filterMap == undefined then
				(
					return "L'objecte " + l_name + " té la textura de filtre activada però no definida."
				)else
				(
					if OpenBitmap l_material.filterMap.filename == undefined then
					(
						return "No es troba la textura de filtre de l'objecte " + l_name + "."
					)
				)				
			)
		)
	)
	
	--other
	
	return true
)

function ExportMesh _object0 _filename _relativeTexturePath = 
(
	format "Exportant objecte: % com a core %\n" _object0.name (GetCoreName _object0)
	
	if _object0.material == undefined then
	(
		ShowError ("No hi ha material a " + _object0.name)
		return false
	)
	
	_object = copy _object0
	convertToMesh _object
	UABCalcVertexsNormals _object
	
	local l_header = 0xAAAA
	local l_footer  =  0xFFFF
	local l_file = fopen _filename "wb"
	
	WriteShort l_file l_header #unsigned
	
	local l_material = _object.material
	
	local l_materialInfo
	
	local l_numMaterials
	-- llista de vertexTypes, amb un sol element si no és un multimaterial
	local l_vertexType = #()
	--lista d'indexs de materials de l'objecte. Només aquells dels que tigui vèrtexs.
	local l_objectMaterials = GetMaterials _object
	
	if l_objectMaterials == undefined then
	(
		ShowError (_object0.name + " no té cap vèrtexs que tingui material")
		return false
	)
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexs
		l_numMaterials = l_material.numsubs
		local l_RealNumMaterials = GetNumGoodMaterials _object
		WriteShort l_file l_RealNumMaterials #unsigned
		
		l_objectMaterials = GetMaterials _object
		
		for i = 1 to l_numMaterials do
		(
			--afegim a la llista el tipus de vertex
			local l_vertexType2 = GetVertexType l_material[i]
			append l_vertexType l_vertexType2
			
			l_materialInfo = MATERIAL_INFO_VERTEXTYPE
			l_materialInfo = bit.or l_materialInfo MATERIAL_INFO_TEXTURETYPE
			
			
			if Member l_objectMaterials i then
			(
				format "Exportant material: % amb Mat. info % i vèrtex %\n" i l_materialInfo l_vertexType2
		
				WriteShort l_file l_materialInfo #unsigned
				--tipus de vertex i material en si
				WriteShort l_file l_vertexType2 #unsigned
				ExportMaterial l_material[i] l_file _relativeTexturePath
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
		
		l_materialInfo = MATERIAL_INFO_VERTEXTYPE
		l_materialInfo = bit.or l_materialInfo MATERIAL_INFO_TEXTURETYPE
		format "Unimaterial amb Mat. info % i vèrtex %\n" l_materialInfo l_vertexType2
    
		WriteShort l_file l_materialInfo #unsigned
		--tipus de vertex i material en si
		WriteShort l_file l_vertexType2 #unsigned
		ExportMaterial l_material l_file _relativeTexturePath
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