fileIn "Export.ms"
fileIn "XML.ms"

utility BioUtility "Eon-Utility" width:160 height:312
(
	button buttonPath "Set Path" pos:[8,8] width:144 height:24
	label labelPath "Marian" pos:[8,52] width:137 height:24
	checkbox checkMeshes "Export Mesh" pos:[13,74] width:114 height:16
	checkbox checkTextures "Export Textures" pos:[13,98] width:115 height:16
	dropDownList dropNivell "Nivell" pos:[11,125] width:131 height:40
	GroupBox grp1 "Export" pos:[11,170] width:131 height:126
	button buttonMesh "Mesh" pos:[23,188] width:100 height:19
	button buttonMeshManager "Mesh Manager" pos:[24,212] width:98 height:22
	button buttonScene "Scene" pos:[26,241] width:94 height:22
	-- Contenido de la utility
	
	on BioUtility open do
	(
		labelPath.text = "D:/treballs/master/biogame/Run/Test/Data"
		--labelPath.text = "C:/Treballs/Directoris v2/Run/Test/Data"
		dropNivell.items = #("","Level -2","Level -1", "Hangar")
	)
	
	function getDataPath =
	(
		local l_path = labelPath.text + "/"
		
		if dropNivell.selection != 1 then
		(
			l_path = l_path + "Levels/" + dropNivell.items[dropNivell.selection] + "/"
		)
		return l_path
	)
	
	function getRelativePath =
	(
		local l_path = labelPath.text
		local l_index = l_path.count
		while l_path[l_index] != "/" and l_path[l_index] != "\\" and l_index > 1 do
		(
			l_index = l_index - 1
		)
		
		l_index = l_index + 1
		
		l_path = substring l_path l_index (l_path.count-l_index+1)
		
		l_path = l_path + "/"
		
		if dropNivell.selection != 1 then
		(
			l_path = l_path + "Levels/" + dropNivell.items[dropNivell.selection] + "/"
		)
		
		return l_path
	)
	
	function getXMLPath =
	(
		local l_path = getDataPath()
		l_path = l_path + "XML/"
		
		return l_path
	)
	
	function getRelativeXMLPath =
	(
		local l_path = getRelativePath()
		l_path = l_path + "XML/"
		
		return l_path
	)
	
	function getTexturePath =
	(
		return getDataPath() + "Textures/"
	)
	function getMeshPath =
	(
		return getDataPath() + "Meshes/"
	)
	
	function getRelativeTexturePath =
	(
		return getRelativePath() + "Textures/"
	)
	function getRelativeMeshPath =
	(
		return getRelativePath() + "Meshes/"
	)
	

	on buttonPath pressed do
	(
		local l_outPath=GetSavePath initialDir:labelPath.text caption:"Select output file to export file" types:" mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
		if l_outPath!=undefined then
		(
			labelPath.text = l_outPath
		)
	)
	
	on buttonMesh pressed do
	(
		if $ != undefined then
		(
			local l_meshPath = getMeshPath()
			local l_TextureTotalPath = getTexturePath()
			makeDir l_meshPath all:true
			makeDir l_TextureTotalPath all:true
			local l_outName=GetSaveFileName fileName:(l_meshPath + $.name + ".mesh") caption:"Select output file to export file" types:" mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
			if l_outName != undefined then
			(
				
				
				local l_relativeTexPath = getRelativeTexturePath()
				
				if (ExportMesh $ l_outName l_relativeTexPath) == true then
				(
					ShowMessage "File Saved"
				)
				
				-- copiar textures
				if checkTextures.checked == true then
				(
					local l_textures = TextureList $
					for i=1 to l_textures.count do
					(
						path1 = l_textures[i]
						path2 = getTexturePath() + (filenameFromPath path1)
						
						if stricmp (getFilenamePath path1) (getFilenamePath path2) != 0 then
						(
							deleteFile path2
							
							copyFile path1 path2
						)
					)
				)
			)
		) else (
			ShowError "No hi ha cap objecte seleccionat"
		)
	)
	
	function ExportMeshManager _objects =
	(
		local l_MeshTotalPath = getMeshPath()
		local l_TextureTotalPath = getTexturePath()
		local l_relativeTexPath = getRelativeTexturePath()
			
		local l_level = dropNivell.items[dropNivell.selection]
		
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "StaticMeshes.xml"
		
		local l_file = createfile l_pathToFile
		
		local l_header = GetInitTagXML()
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "meshes") to:l_file
		
		for i=1 to _objects.count do
		(
			local l_object = _objects[i]
			
			local l_relPath = getRelativeMeshPath() + l_object.name + ".mesh"
			
			l_params = #(#("name",l_object.name),#("path",l_relPath))
			
			format "\n\t" to:l_file
			format "%" (InlineTagParamXML "mesh" l_params) to:l_file
				
			if checkMeshes.checked then
			(
				makeDir l_MeshTotalPath all:true
				
				local l_outName = l_MeshTotalPath + l_object.name + ".mesh"
				
				ExportMesh l_object l_outName l_relativeTexPath
			)
			
			if checkTextures.checked then
			(
				makeDir l_TextureTotalPath all:true
				local l_textures = TextureList l_object
				for i=1 to l_textures.count do
				(
					path1 = l_textures[i]
					path2 = getTexturePath() + (filenameFromPath path1)
					
					if stricmp (getFilenamePath path1) (getFilenamePath path2) != 0 then
					(
						deleteFile path2
						
						copyFile path1 path2
					)
				)
			)
		)
		
		format "\n" to:l_file
		format "%" (CloseTagXML "meshes") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
	function ExportRenderableObjectsManager _objects =
	(
			
		local l_level = dropNivell.items[dropNivell.selection]
		
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "RenderableObjects.xml"
		
		
		local l_file = createfile l_pathToFile
		
		local l_header = GetInitTagXML()
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "RenderableObjects") to:l_file
		
		for i=1 to _objects.count do
		(
			local l_object = _objects[i]
			
			l_params = #(#("name",l_object.name),#("class","StaticMesh"),#("resource",l_object.name))
		
				
			local l_aux = "" as stringStream
			
				
			local l_translation = RHVectorToLH l_object.transform.translation
			format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
			append l_params #("position",l_aux)
			
			l_aux = "" as stringStream
			local l_yaw = GetYaw l_object.transform
			format "%" l_yaw to:l_aux
			append l_params #("yaw",l_aux)
			
			l_aux = "" as stringStream
			local l_pitch = GetPitch l_object.transform
			format "%" l_pitch to:l_aux
			append l_params #("pitch",l_aux)
			
			l_aux = "" as stringStream
			local l_roll = GetRoll l_object.transform
			format "%" l_roll to:l_aux
			append l_params #("roll",l_aux)
			
			format "\n\t" to:l_file
			format "%" (InlineTagParamXML "RenderableObject" l_params) to:l_file
				
			
		)
		
		format "\n" to:l_file
		format "%" (CloseTagXML "RenderableObjects") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
	on buttonMeshManager pressed do
	(
		if ExportMeshManager $* then
		(
			ShowMessage "Export finalitzat"
		)
	)
	
	on buttonScene pressed do
	(
		if ExportMeshManager $* then
		(
			ShowMessage "Export mesh manager finalitzat"
		)
		if ExportRenderableObjectsManager $* then
		(
			ShowMessage "Export scene finalitzat"
		)
	)
)
