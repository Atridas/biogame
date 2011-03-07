fileIn "GlobalsAndHelpers.ms"
fileIn "MaterialAndTexture.ms"
fileIn "IndexAndVertex.ms"

function ExportMesh _object0 _filename _relativeTexturePath = 
(
	format "Exportant objecte: % \n" _object0.name
	
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
	
	if classof(l_material) == Multimaterial then
	(
		--definim coses que necessitarem quan fem els vertexs		
		l_numMaterials = l_material.numsubs
		local l_RealNumMaterials = GetNumGoodMaterials _object
		WriteShort l_file l_RealNumMaterials #unsigned
		
		for i = 1 to l_numMaterials do
		(
			--afegim a la llista el tipus de vertex
			local l_vertexType2 = GetVertexType l_material[i]
			append l_vertexType l_vertexType2
			
			
			l_materialInfo = MATERIAL_INFO_VERTEXTYPE
			l_materialInfo = bit.or l_materialInfo MATERIAL_INFO_TEXTURETYPE
			
			
			if ((IsMaterialEmpty _object i) == false) then
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
	
	for i = 1 to l_numMaterials do
	(
		if (l_numMaterials == 1 or (IsMaterialEmpty _object i) == false) then
		(
			--format "Vertex Type %\n" l_vertexType[i] 
			WriteVertexs l_file l_VertexList[i] l_vertexType[i] 
			WriteIndexs l_file l_IndexList[i]
		)
	)
	
	WriteShort l_file l_footer #unsigned
	
	fflush l_file
	fclose l_file
	
	delete _object
	
	return true;
)