fileIn "Config.ms"
fileIn "XML.ms"
fileIn "Export.ms"

utility BioUtility "Eon-Utility" width:160 height:549
(
	button buttonPath "Set Path" pos:[8,8] width:144 height:24
	label labelPath "Marian" pos:[8,52] width:137 height:24
	checkbox checkMeshes "Export Mesh" pos:[13,74] width:114 height:16
	checkbox checkTextures "Export Textures" pos:[13,98] width:115 height:16
	dropdownList dropNivell "Nivell" pos:[11,125] width:131 height:40
	GroupBox grp1 "Export" pos:[11,170] width:131 height:135
	button buttonMesh "Mesh" pos:[23,188] width:100 height:19
	button buttonMeshManager "Mesh Manager" pos:[24,212] width:98 height:22
	button buttonScene "Scene" pos:[26,271] width:94 height:22
	button buttonLight "Light" pos:[27,241] width:93 height:18
	GroupBox grp3 "PhysX" pos:[12,323] width:130 height:119
	dropdownList DropListPhysX "Type" pos:[29,346] width:95 height:40
	button ButtonPhysX "Create" pos:[33,404] width:88 height:23
	button ButtonEntities "Exportar Entitats" pos:[21,460] width:116 height:26
	-- Control de Paths
  
	-- Torna el path de Data
	function getDataPath =
	(
		local l_path = labelPath.text + "/"
		return l_path
	)
	
	-- Torna el path de Data segons el nivell
	function getLevelPath =
	(
		local l_path = labelPath.text + "/"
		
		if dropNivell.selection != 1 then
		(
			l_path = l_path + "Levels/" + dropNivell.items[dropNivell.selection] + "/"
		)
		return l_path
	)

	-- Torna el path relatiu
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

		return l_path
	)
	
	-- Torna el path relatiu segons el nivell
	function getRelativeLevelPath =
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

	-- Torna el path dels XML
	function getXMLPath =
	(
		local l_path = getLevelPath()
		l_path = l_path + "XML/"

		return l_path
	)

	-- Torna el path relatiu dels XML
	function getRelativeXMLPath =
	(
		local l_path = getRelativeLevelPath()
		l_path = l_path + "XML/"

		return l_path
	)

	-- Torna el path de les textures
	function getTexturePath =
	(
		return getDataPath() + "Textures/"
	)

	-- Torna el path de les Mesh
	function getMeshPath =
	(
		return getDataPath() + "Meshes/"
	)

	-- Torna el path relatiu de les textures
	function getRelativeTexturePath =
	(
		return getRelativePath() + "Textures/"
	)

	-- Torna el path relatiu de les mesh
	function getRelativeMeshPath =
	(
		return getRelativePath() + "Meshes/"
	)
  
	-- Fi path
  
	-- Exportadors
	
	--Exporta les textures al path especificat
	function ExportTextures _TextureTotalPath _textures =
	(
		makeDir _TextureTotalPath all:true
				
		for i=1 to _textures.count do
		(
			path1 = _textures[i]
			path2 = _TextureTotalPath + (filenameFromPath path1)
			
			if stricmp (getFilenamePath path1) (getFilenamePath path2) != 0 then
			(
				deleteFile path2
				copyFile path1 path2
			)
		)
	)
  
	-- Exporta un array de cores, creant les mesh i l'xml de mesh
	function ExportMeshManager _objects =
	(
		local l_MeshTotalPath = getMeshPath()
		local l_TextureTotalPath = getTexturePath()
		local l_relativeTexPath = getRelativeTexturePath()
		local l_textures = #()
			
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
			local l_name  = GetCoreName _objects[i]
			
			local l_relPath = getRelativeMeshPath() + l_name + ".mesh"
			
			-------------------------------
			--paràmetres
			l_params = #()
			
			append l_params #("name",l_name)
			append l_params #("path",l_relPath)
			
			local l_invTransform = inverse l_object.transform
			
			l_aux = "" as stringStream
			local l_MaxCoord = l_object.max
			l_MaxCoord = l_MaxCoord * l_invTransform
			l_MaxCoord = RHVectorToLH l_MaxCoord
			format "% % %" l_MaxCoord.x l_MaxCoord.y l_MaxCoord.z to:l_aux
			append l_params #("max",l_aux)
			
			l_aux = "" as stringStream
			local l_MinCoord = l_object.min
			l_MinCoord = l_MinCoord * l_invTransform
			l_MinCoord = RHVectorToLH l_MinCoord
			format "% % %" l_MinCoord.x l_MinCoord.y l_MinCoord.z to:l_aux
			append l_params #("min",l_aux)
			
			format "\n\t" to:l_file
			format "%" (InlineTagParamXML "mesh" l_params) to:l_file
			-------------------------------
				
			if checkMeshes.checked then
			(
				makeDir l_MeshTotalPath all:true
				
				local l_outName = l_MeshTotalPath + l_name + ".mesh"
				
				if ExportMesh l_object l_outName l_relativeTexPath == false then
				(
					--close l_file
					
					local l_text = "" as stringStream
					format "La core % ha causat un error d'exportació" l_name to:l_text
					ShowError l_text
					
					--return false
				)
			)
			
			if checkTextures.checked then
			(
				TextureList l_textures l_object
				
				/*
				makeDir l_TextureTotalPath all:true
				
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
				*/
			)
		)
		
		if checkTextures.checked then
		(
			ExportTextures l_TextureTotalPath l_textures
		)
		
		format "\n" to:l_file
		format "%" (CloseTagXML "meshes") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
	-- Crea l'xml de renderable objects
	function ExportRenderableObjectsManager _instances _cores _AnimatedModels =
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
		
		for i=1 to _instances.count do
		(      
			for j=1 to _instances[i].count do
			(
				local l_instance = _instances[i][j]
				
				l_params = #(#("name",l_instance.name),#("class","StaticMesh"),#("resource", (GetCoreName l_instance)))
					
				l_aux = "" as stringStream
					
				local l_translation = RHVectorToLH l_instance.transform.translation
				format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
				append l_params #("position",l_aux)
				
				l_aux = "" as stringStream
				local l_yaw = GetYaw l_instance.transform
				format "%" l_yaw to:l_aux
				append l_params #("yaw",l_aux)
				
				l_aux = "" as stringStream
				local l_pitch = GetPitch l_instance.transform
				format "%" l_pitch to:l_aux
				append l_params #("pitch",l_aux)
				
				l_aux = "" as stringStream
				local l_roll = GetRoll l_instance.transform
				format "%" l_roll to:l_aux
				append l_params #("roll",l_aux)
				
				format "\n\t" to:l_file
				format "%" (InlineTagParamXML "RenderableObject" l_params) to:l_file
			)
		)
		
		for i=1 to _AnimatedModels.count do
		(
			local l_object = _AnimatedModels[i]
			local l_resource = getUserProp l_object "resource"
			local l_animation = getUserProp l_object "cycle"
			local l_altura = getUserProp l_object "altura"
			
			l_params = #(#("name",l_object.name),#("class","AnimatedModel"),#("resource",l_resource),#("cycle",l_animation))
		
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
			
			l_aux = "" as stringStream
			format "%" l_altura to:l_aux
			append l_params #("Altura",l_aux)
			
			format "\n\t" to:l_file
			format "%" (InlineTagParamXML "RenderableObject" l_params) to:l_file
		)
		format "\n" to:l_file
		format "%" (CloseTagXML "RenderableObjects") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
  -- Crea l'xml de les llums de l'escena
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
				--res, és un mesh o algo així
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
	
	
	
	function ExportPhysX _instances =
	(
			
		local l_PhysXType = DropListPhysX.items[DropListPhysX.selection]
		
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "GameObjects.xml"
		
		
		local l_file = createfile l_pathToFile
		
		local l_header = GetInitTagXML()
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "GameObjectManager") to:l_file
		
		for i=1 to _instances.count do
		(
			for j=1 to _instances[i].count do
			(
				local l_instance = _instances[i][j]
				
				l_params = #(#("name",l_instance.name),#("Physx","true"),#("PhysxType",l_PhysXType),#("RenderableObject",l_instance.name),#("PhysxActor",l_instance.name),#("Pushable","false"))
					
				l_aux = "" as stringStream
				l_Body = 0
				
				format "%" l_Body to:l_aux
				append l_params #("Body",l_aux)
				
				format "\n\t" to:l_file
				format "%" (InlineTagParamXML "GameObject" l_params) to:l_file
			)
			
		)
		
		format "\n" to:l_file
		format "%" (CloseTagXML "GameObjectManager") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	
	function ExportMeshEntities _instances =
	(
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "GameEntities.xml"
		
		local l_file = createfile l_pathToFile
		local l_header = GetInitTagXML()
		
		local l_aux
		local l_params
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "Entities") to:l_file
		
		------------------------------------------------------
		for i=1 to _instances.count do
		(
			for j=1 to _instances[i].count do
			(
				local l_instance = _instances[i][j]
				
				format "\n  %" (OpenTagXML "Entity") to:l_file
				
				format "\n    %" (InlineTagXML "Object3D") to:l_file
				  
				l_params = #(#("name",l_instance.name))
				format "\n    %" (InlineTagParamXML "RenderableObject" l_params) to:l_file
				
				if (GetUserProp l_instance "physXBox") != undefined then
				(
					l_params = #(#("fromRenderableObject","true"),#("density","0"))
					format "\n    %" (InlineTagParamXML "PhysXBox" l_params) to:l_file
				)
				
				if (GetUserProp l_instance "physXMesh") != undefined then
				(
					l_params = #(#("density","0"))
					format "\n    %" (InlineTagParamXML "PhysXMesh" l_params) to:l_file
				)
				
				format "\n  %" (CloseTagXML "Entity") to:l_file
			)
		)
		-------------------------------------------------------
		
		format "\n" to:l_file
		format "%" (CloseTagXML "Entities") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
	-- Fi exportadors
	
	-- Funcionalitats botons	
	
	
	
	
	-- Fi funcionalitats botons

	on BioUtility open do
	(
		labelPath.text = g_DefaultPath
		dropNivell.items = g_LevelArray
		DropListPhysX.items = g_PhysXArray
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
			local l_check = CheckMesh $ true
			
			if l_check != true then
			(
				ShowError l_check
				return false
			)
			
			local l_meshPath = getMeshPath()
			local l_TextureTotalPath = getTexturePath()
			local l_textures = #()
			
			makeDir l_meshPath all:true
			makeDir l_TextureTotalPath all:true
			
			local l_text = "" as stringStream
			format "%.mesh" (GetCoreName $) to:l_text
			
			local l_outName=GetSaveFileName fileName:(l_meshPath + l_text) caption:"Select output file to export file" types:" mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
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
					TextureList l_textures $
					/*
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
					*/
				)
			)
		) else (
			ShowError "No hi ha cap objecte seleccionat"
		)
	)
	on buttonMeshManager pressed do
	(
		l_cores = #()
		for i=1 to $*.count do
		(
			local l_index = GetCoreIndex l_cores $*[i]
				
			if IsMesh $*[i] and l_index == undefined then
			(
				local l_corename = GetCoreName $*[i]
				
				if l_corename != undefined then
				(
					AddCore l_cores $*[i] l_corename
				) else
				(
					local l_text = "" as stringStream
					format "L'objecte % no té core especificada" $*[i].name to:l_text
					ShowError l_text
				)
			)
		)
		
		format "Export mesh manager...\n"
		if ExportMeshManager l_cores then
		(
			format "Export finalitzat\n"
		)
	
	)
	on buttonScene pressed do
	(
		local l_cores = #()
		local l_instances = #()
		local l_lights = #()
		local l_AnimatedModels = #()
		local l_aux = "" as stringStream
		
		--Check
		local l_check = true
		l_check = CheckSceneObjects $*
			
		if l_check != true then
		(
			ShowError l_check
			return false
		)
		--Check end
		
		for i=1 to $*.count do
		(
			if IsLight $*[i] then
			(
				append l_lights $*[i]
			) else if IsMesh $*[i] then
			(
				--instances és una array d'arrays, on cada una d'aquestes subarrays conté totes les instancies d'un core.
				--l'índex de l'array on es troben les instancies coincideix amb l'índex de l'array de cores.
				local l_index = GetCoreIndex l_cores $*[i]
				
				if l_index == undefined then
				(
					local l_corename = GetCoreName $*[i]
					 
					if l_corename != undefined then
					(
						AddCore l_cores $*[i] l_corename
						NewInstance l_instances $*[i]
					) else
					(
						local l_text = "" as stringStream
						format "L'objecte % no té core especificada o no te nom..." $*[i].name to:l_text
						ShowError l_text
					)
				) else
				(
					AddInstance l_instances $*[i] l_index
				)
	
			) else if isAnimatedModel $*[i] then
			(
				append l_AnimatedModels $*[i]
			)
		)
		
		format "Export mesh manager...\n"
		if ExportMeshManager l_cores then
		(
			format "Export mesh manager finalitzat\n"
		)
		format "Export renderable objects...\n"
		if ExportRenderableObjectsManager l_instances l_cores l_AnimatedModels then
		(
			format "Export renderable objects finalitzat\n"
		)
		format "Export lights...\n"
		if ExportLights l_lights then
		(
			format "Export lights finalitzat\n"
		)
		
		ShowMessage "Fi"
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
		
		format "Export lights...\n"
		if ExportLights l_lights then
		(
			ShowMessage "Export finalitzat"
		) else 
		(
			ShowError "Problema inidentificat"
		)
	)
	
	on ButtonPhysX pressed do
	(
		
		local l_cores = #()
		local l_instances = #()
		
		--Check
		local l_check = true
		l_check = CheckSceneObjects $*
			
		if l_check != true then
		(
			ShowError l_check
			--return false
		)
		
		for i=1 to $*.count do
		(
			if IsMesh $*[i] then
			(
				--instances és una array d'arrays, on cada una d'aquestes subarrays conté totes les instancies d'un core.
				--l'índex de l'array on es troben les instancies coincideix amb l'índex de l'array de cores.
				local l_index = GetCoreIndex l_cores $*[i]
				
				if l_index == undefined then
				(
					NewInstance l_instances $*[i]
				) 
				else
				(
					AddInstance l_instances $*[i] l_index
				)
	
			) 
		)
		
		if ExportPhysX l_instances then
		(
			ShowMessage "Export de la Fisica finalitzat\n"
		)
	)
	
	on ButtonEntities pressed do
	(
		
		local l_cores = #()
		local l_instances = #()
		
		--Check
		local l_check = true
		l_check = CheckSceneObjects $*
			
		if l_check != true then
		(
			ShowError l_check
			--return false
		)
		
		for i=1 to $*.count do
		(
			if IsMesh $*[i] then
			(
				--instances és una array d'arrays, on cada una d'aquestes subarrays conté totes les instancies d'un core.
				--l'índex de l'array on es troben les instancies coincideix amb l'índex de l'array de cores.
				local l_index = GetCoreIndex l_cores $*[i]
				
				if l_index == undefined then
				(
					NewInstance l_instances $*[i]
				) 
				else
				(
					AddInstance l_instances $*[i] l_index
				)
	
			) 
		)
		
		if ExportMeshEntities l_instances then
		(
			ShowMessage "Export de les Entities finalitzada\n"
		)
	)
)
