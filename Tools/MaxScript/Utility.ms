fileIn "Export.ms"
fileIn "XML.ms"

utility BioUtility "Eon-Utility" width:160 height:312
(
	button buttonPath "Set Path" pos:[8,8] width:144 height:24
	label labelPath "Marian" pos:[8,52] width:137 height:24
	checkbox checkMeshes "Export Mesh" pos:[13,74] width:114 height:16
	checkbox checkTextures "Export Textures" pos:[13,98] width:115 height:16
	dropdownList dropNivell "Nivell" pos:[11,125] width:131 height:40
	GroupBox grp1 "Export" pos:[11,170] width:131 height:126
	button buttonMesh "Mesh" pos:[23,188] width:100 height:19
	button buttonMeshManager "Mesh Manager" pos:[24,212] width:98 height:22
	button buttonScene "Scene" pos:[26,271] width:94 height:22
	button buttonLight "Light" pos:[27,241] width:93 height:18
	-- Contenido de la utility
	
	
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
	

	
	
	function GetCore _cores _object =
	(
		for i=1 to _cores.count do
		(
			if areNodesInstances _cores[i] _object then
			(
				return _cores[i]
			)
		)
		return undefined
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
				
				if ExportMesh l_object l_outName l_relativeTexPath == false then
				(
					close l_file
					return false
				)
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
	
	function ExportRenderableObjectsManager _objects _cores _AnimatedModels =
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
			
			local l_core = GetCore _cores l_object
			
			l_params = #(#("name",l_object.name),#("class","StaticMesh"),#("resource",l_core.name))
		
				
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
		
		for i=1 to _AnimatedModels.count do
		(
			
			local l_object = _AnimatedModels[i]
			local l_resource = getUserProp l_object "Resource"
			
			l_params = #(#("name",l_object.name),#("class","AnimatedModel"),#("resource",l_resource))
		
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
	
	
	
	function ExportLights _lights =
	(			
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "Lights.xml"
		
		
		local l_file = createfile l_pathToFile
		
		local l_header = GetInitTagXML()
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		local l_aux = "" as StringStream
		format "% % %" 0.3 0.3 0.3 to:l_aux
		
		format "%\n" (OpenTagParamXML "lights" #(#("ambient", l_aux))) to:l_file
		
		for i=1 to _lights.count do
		(
			local l_light = _lights[i]
			local l_params = #()
			
			local l_position = "" as StringStream
			local l_color = "" as StringStream 
			local l_translation = "" as StringStream 
			
			if classof l_light == TargetDirectionallight then
			(
				--llum direccional
				append l_params #("type","directional")
				
				local l_dir = l_light.target.pos - l_light.pos
				local l_direction
				l_translation = RHVectorToLH l_dir
				l_aux = "" as StringStream
				format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
				append l_params #("direction",l_aux)
			) else if classof l_light == Omnilight then
			(
				--omni
				append l_params #("type","omni")
			) else if classof l_light == targetSpot then
			(
				-- spot
				append l_params #("type","spot")
				
				local l_dir = l_light.target.pos - l_light.pos
				local l_direction
				l_translation = RHVectorToLH l_dir
				l_aux = "" as StringStream
				format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
				append l_params #("direction",l_aux)
				
				l_aux = "" as StringStream
				format "%" l_light.hotspot to:l_aux
				append l_params #("angle",l_aux)
				
				l_aux = "" as StringStream
				format "%" l_light.falloff to:l_aux
				append l_params #("fall_off",l_aux)
			) else
			(
				--res, és un mesh o algu així
				format "Això no hauria de passar MAI!\n"
				format "%\n" l_light
				continue
			)
			
			append l_params #("name",l_light.name)
			
			l_translation = RHVectorToLH l_light.pos
			format "% % %" l_translation.x l_translation.y l_translation.z to:l_position
			append l_params #("pos",l_position)
			format "% % %" (l_light.rgb.r/255) (l_light.rgb.g/255) (l_light.rgb.b/255)  to:l_color
			append l_params #("color",l_color)
			
			l_aux = "" as StringStream
			format "%" l_light.NearAttenStart to:l_aux
			append l_params #("att_start_range",l_aux)
			l_aux = "" as StringStream
			format "%" l_light.farAttenEnd to:l_aux
			append l_params #("att_end_range",l_aux)
			
			append l_params #("render_shadows","false")
			
			format "\t%\n" (InlineTagParamXML "light" l_params) to:l_file
		)
		format "%" (CloseTagXML "lights") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
	
	on BioUtility open do
	(
		labelPath.text = "D:/treballs/master/biogame/Run/Test/Data"
		--labelPath.text = "C:/Treballs/Directoris v2/Run/Test/Data"
		dropNivell.items = #("","Level -2","Level -1", "Hangar", "Proves")
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
	
	function IsLight _object =
	(
		if classof _object == TargetDirectionallight or classof _object == Omnilight or classof _object == targetSpot then
		(
			return true
		) else (
			return false
		)
	)
	
	function IsMesh _object =
	(
		-- TODO acceptar més coses
		if classof _object == Editable_Poly or classof _object == Box or classof _object == Editable_mesh or classof _object == Cylinder or classof _object == GeoSphere then
		(
			return true
		) else (
			return false
		)
	)
	
	function isAnimatedModel _object =
	(
		
		-- TODO acceptar més coses
		if classof _object == Dummy then
		(
			if getUserProp _object "UserType" == "AnimatedModel" then
			(
				--ShowMessage "Tenim un AnimatedModel"
				return true
			)
			else
			(
				return false
			)
		) else (
			return false
		)
	)
	
	on buttonMeshManager pressed do
	(
		l_cores = #()
		for i=1 to $*.count do
		(
			if IsMesh $*[i] and GetCore l_cores $*[i] == undefined then
			(
				append l_cores $*[i]
			)
		)
		
		if ExportMeshManager l_cores then
		(
			ShowMessage "Export finalitzat"
		)
	)
	
	on buttonLight pressed do
	(
		local l_lights = #()
		for i=1 to $*.count do
		(
			if IsLight $*[i] then
			(
				--llum, no ho exportem aquí
				append l_lights $*[i]
			)
		)
		if ExportLights l_lights then
		(
			ShowMessage "Export finalitzat"
		) else 
		(
			ShowError "Problema inidentificat"
		)
	)
	
	on buttonScene pressed do
	(
		local l_cores = #()
		local l_instances = #()
		local l_lights = #()
		local l_AnimatedModels = #()
		
		for i=1 to $*.count do
		(
			if IsLight $*[i] then
			(
				append l_lights $*[i]
			) else if IsMesh $*[i] then
			(
				append l_instances $*[i]
				if GetCore l_cores $*[i] == undefined then
				(
					append l_cores $*[i]
				)
			) else if isAnimatedModel $*[i] then
			(
				append l_AnimatedModels $*[i]
				--ShowMessage "Tenim un AnimatedModel"
			)
		)
		
		if ExportMeshManager l_cores then
		(
			ShowMessage "Export mesh manager finalitzat"
		)
		if ExportRenderableObjectsManager l_instances l_cores l_AnimatedModels then
		(
			ShowMessage "Export scene finalitzat"
		)
		if ExportLights l_lights then
		(
			ShowMessage "Export lights finalitzat"
		)
		ShowMessage "Fi"
	)
)
