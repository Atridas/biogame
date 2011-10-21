fileIn "Config.ms"
fileIn "XML.ms"
fileIn "Export.ms"
fileIn "Physic.ms"
filein "Portalling.ms"
fileIn "Particles.ms"
fileIn "EntitiesXML.ms"

utility BioUtility "Eon-Utility" width:160 height:549
(
	button buttonPath "Set Path" pos:[8,8] width:144 height:24
	label labelPath "Marian" pos:[8,46] width:137 height:24
	checkbox checkMeshes "Export Mesh" pos:[13,74] width:114 height:16
	checkbox checkTextures "Export Textures" pos:[13,90] width:115 height:16
	checkbox checkNormalRadiosity "Normal Radiosity" pos:[13,106] width:112 height:17
	dropdownList dropNivell "Nivell" pos:[11,125] width:131 height:40
	GroupBox grp1 "Export" pos:[11,170] width:131 height:171
	button buttonMesh "Mesh" pos:[23,188] width:100 height:19
	button buttonMeshManager "Mesh Manager" pos:[24,212] width:98 height:22
	button buttonScene "Scene" pos:[27,305] width:94 height:22
	button buttonLight "Light" pos:[27,241] width:93 height:18
	button buttonParticles "Particles" pos:[28,271] width:93 height:23
	GroupBox grp3 "PhysX" pos:[14,360] width:130 height:119
	dropdownList DropListPhysX "Type" pos:[33,385] width:95 height:40
	button ButtonPhysX "Create" pos:[36,437] width:88 height:23
	button ButtonEntities "Exportar Entitats" pos:[24,488] width:116 height:26
	button ButtonNivell "Exportar Portals" pos:[24,520] width:114 height:23
	
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
	
	-- Torna el path relatiu de les particules
	function getRelativeParticlePath =
	(
		return getRelativePath() + "Particles/"
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
			
			--Wrapper Físic de l'objecte
			l_coords = GetPhysxWrapper l_object
			l_coordscount = l_coords.count
			for i = 1 to l_coordscount do
			(
				append l_params l_coords[i]
			)
			
			format "\n\t" to:l_file
			format "%" (InlineTagParamXML "mesh" l_params) to:l_file
			-------------------------------
				
			if checkMeshes.checked then
			(
				makeDir l_MeshTotalPath all:true
				
				local l_outName = l_MeshTotalPath + l_name + ".mesh"
				
				if (ExportMesh l_object l_outName l_relativeTexPath checkNormalRadiosity.checked) == false then
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
				TextureList l_textures l_object checkNormalRadiosity.checked
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
		
		--Default riggle actor at viewer
		if dropNivell.text == "Viewer" then
		(
			format "\n\t" to:l_file
			l_params = #(#("name","riggle_mainchar"),#("class","AnimatedModel"),#("resource","riggle"),#("cycle","idle"),#("position","0.0 0.0 0.0"),#("yaw","-90.0"),#("pitch","0.0"),#("roll","0.0"))
			format (InlineTagParamXML "RenderableObject" l_params) to:l_file
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
				format "Exportant llum direccional %\n" l_light.name
				
			) else if classof l_light == Omnilight then
			(
				--omni
				append l_params #("type","omni")
				format "Exportant llum omni %\n" l_light.name
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
				
				format "Exportant llum spot %\n" l_light.name
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
	
    function ExportPortalsAndRooms =
	(
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		l_pathToFile = l_pathToFile + "Portals.xml"
		local l_file = createfile l_pathToFile
		
		local l_aux
		local l_params
		
		format "%" (GetInitTagXML()) to:l_file
		
		format "\n%" (OpenTagXML "PortalManager") to:l_file
		
		
		local rooms = CreateRooms $*
		local objectes = FillRooms $* rooms
		local portals = CreatePortals $*
		
		format "\n  %" (OpenTagXML "Rooms") to:l_file
		
		for i = 1 to rooms.count do
		(
			local room = rooms[i]
			local roomObjs = objectes[i]
			local roomDumies = CreateRoomDummies roomObjs
			
			if room == undefined then
			(
				--l_params = #(#("undefined", "true"))
				continue;
			)
			else
			(
				--l_params = #(#("undefined", "false"),#("name", room.name))
				l_params = #(#("name", room.name))
			)
			format "\n    %" (OpenTagParamXML "Room" l_params) to:l_file
			
			
			format "\n      %" (OpenTagXML "Boundings") to:l_file
			for j = 1 to roomDumies.count do
			(
				local l_dummy = roomDumies[j]
				l_params = #(#("name", l_dummy.name))
				l_aux = "" as stringStream
					
				local l_size = RHVectorToLH (DummySize l_dummy)
				format "% % %" l_size.x l_size.y l_size.z to:l_aux
				append l_params #("size",l_aux)
				l_aux = "" as stringStream
				
				local l_translation = RHVectorToLH l_dummy.transform.translation
				format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
				append l_params #("position",l_aux)
				
				l_aux = "" as stringStream
				local l_yaw = GetYaw l_dummy.transform
				format "%" l_yaw to:l_aux
				append l_params #("yaw",l_aux)
				
				l_aux = "" as stringStream
				local l_pitch = GetPitch l_dummy.transform
				format "%" l_pitch to:l_aux
				append l_params #("pitch",l_aux)
				
				l_aux = "" as stringStream
				local l_roll = GetRoll l_dummy.transform
				format "%" l_roll to:l_aux
				append l_params #("roll",l_aux)
					
				format "\n        %" (InlineTagParamXML "Box" l_params) to:l_file
			)
			format "\n      %" (CloseTagXML "Boundings") to:l_file
			
			format "\n      %" (OpenTagXML "Objects") to:l_file
			for j = 1 to roomObjs.count do
			(
				if (IsInfo roomObjs[j]) == false then
				(
					l_params = #(#("name", roomObjs[j].name))
					format "\n        %" (InlineTagParamXML "GameObject" l_params) to:l_file
				)
			)
			format "\n      %" (CloseTagXML "Objects") to:l_file
			
			format "\n    %" (CloseTagXML "Room") to:l_file
		)
		format "\n  %" (CloseTagXML "Rooms") to:l_file
		-------------------------------------------------------
		
		format "\n  %" (OpenTagXML "Portals") to:l_file
		
		for i = 1 to portals.count do
		(
			local portal = portals[i]
			local roomA = GetUserProp portal "roomA"
			local roomB = GetUserProp portal "roomB"
			l_params = #(#("name",portal.name),#("roomA",roomA),#("roomB",roomB))
			l_aux = "" as stringStream
				
			local l_size = RHVectorToLH (DummySize portal)
			format "% % %" l_size.x l_size.y l_size.z to:l_aux
			append l_params #("size",l_aux)
				
			l_aux = "" as stringStream
			local l_translation = RHVectorToLH portal.transform.translation
			format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux
			append l_params #("position",l_aux)
			
			l_aux = "" as stringStream
			local l_yaw = GetYaw portal.transform
			format "%" l_yaw to:l_aux
			append l_params #("yaw",l_aux)
			
			l_aux = "" as stringStream
			local l_pitch = GetPitch portal.transform
			format "%" l_pitch to:l_aux
			append l_params #("pitch",l_aux)
			
			l_aux = "" as stringStream
			local l_roll = GetRoll portal.transform
			format "%" l_roll to:l_aux
			append l_params #("roll",l_aux)
			
			format "\n    %" (InlineTagParamXML "Portal" l_params) to:l_file
		)
		
		format "\n  %" (CloseTagXML "Portals") to:l_file
		-------------------------------------------------------
		
		format "\n%" (CloseTagXML "PortalManager") to:l_file
		
		format "\n" to:l_file
		
		close l_file
		
		return true
	)
  
  function ExportEntities _entities =
	(
		local l_pathToFile = getXMLPath() 
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "GameEntities.xml"
		
		local l_file = createfile l_pathToFile
		local l_header = GetInitTagXML()
		
		local l_params = #()
		local l_TagName = ""
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "Entities") to:l_file
		
		------------------------------------------------------
		for i=1 to _entities.count do
		(
			local l_instance = _entities[i]
			
			if WantsEntity l_instance then
			(
			
				if IsMesh l_instance then
				(
					local l_rotative = false
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Mesh %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--RenderableObject
					l_params = XMLEntityRenderableObject l_instance &l_TagName false
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--PhysXBox
					if (GetUserProp l_instance "physXBox") != undefined then
					(
						local l_density = GetUserProp l_instance "physXBox"
						
						if classof l_density != Integer then
						(
							l_density = 0
						)
						local l_colgroup = "escenari"
						if l_density != 0 then
						(
							l_colgroup = "objecte dinamic"
						)
						
						l_params = XMLEntityPhysXBox l_instance &l_TagName l_colgroup true
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						--Rotative
						if l_rotative == false and IsRotative l_instance then
						(
							l_rotative = true
							--Rotative
							l_params = XMLEntityRotation l_instance &l_TagName
							format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						)
					)
					--PhysXMesh
					else if (GetUserProp l_instance "physXMesh") != undefined then
					(
						local l_density = GetUserProp l_instance "physXMesh"
						
						if classof l_density != Integer then
						(
							l_density = 0
						)
						local l_colgroup = "escenari"
						if l_density != 0 then
						(
							l_colgroup = "objecte dinamic"
						)
						
						l_params = XMLEntityPhysXMesh l_instance &l_TagName l_colgroup
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						--Rotative
						if l_rotative == false and IsRotative l_instance then
						(
							l_rotative = true
							--Rotative
							l_params = XMLEntityRotation l_instance &l_TagName
							format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						)
					)
					
					--PickUp
					if IsPickUp l_instance then
					(
						--Trigger
						l_params = XMLEntityTrigger l_instance &l_TagName "pickup" (GetUserProp l_instance "pickup") undefined
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						--Rotative
						if l_rotative == false and IsRotative l_instance then
						(
							l_rotative = true
							--Rotative
							l_params = XMLEntityRotation l_instance &l_TagName
							format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						)
					)
					
					--Interactive
					if IsInteractive l_instance then
					(					
						--PhysXBox
						--l_params = XMLEntityPhysXBox l_instance &l_TagName "escenari" true
						--format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						--Interactive
						l_params = XMLEntityInteractive l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					--Destroyable
					if IsDestroyable l_instance then
					(
						--PhysXBox
						--l_params = XMLEntityPhysXBox l_instance &l_TagName "escenari" true
						--format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						--Trigger
						l_params = XMLEntityDestroyable l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						
					)
					
					--Explosive
					if IsExplosive l_instance then
					(
						--Explosive
						l_params = XMLEntityExplosive l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					--Maquina d'estats
					if HasStateMachine l_instance then
					(
						--StateMachine
						l_params = XMLEntityStateMachine l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsTrigger l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Trigger %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Trigger
					l_params = XMLEntityTrigger l_instance &l_TagName "trigger" (GetUserProp l_instance "TriggerEnter") (GetUserProp l_instance "TriggerExit")
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Emiter de partícules
					if IsEmiter l_instance then
					(
						l_params = XMLEntityEmiter l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					--Rotative
					if IsRotative l_instance then
					(
						--Rotative
						l_params = XMLEntityRotation l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsCover l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name", l_instance.name))) to:l_file
					)
					
					format "\tEntitat Cover %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Cover
					l_params = XMLEntityCover l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsDoor l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name", l_instance.name))) to:l_file
					)
					
					format "\tEntitat Door %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Renderable Object
					l_params = XMLEntityRenderableObject l_instance &l_TagName true
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
					--PhysXBox
					--l_params = XMLEntityPhysXBox l_instance &l_TagName "escenari" false
					--format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
					--Gate
					l_params = XMLEntityDoor l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsPickUp l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat PickUp %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
					--Renderable Object
					l_params = XMLEntityRenderableObject l_instance &l_TagName false
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Trigger
					l_params = XMLEntityTrigger l_instance &l_TagName "objecte interactiu" (GetUserProp l_instance "pickup") undefined
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Rotative
					if IsRotative l_instance then
					(
						--Rotative
						l_params = XMLEntityRotation l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsInteractive l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Interactiva %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
					--Renderable Object
					l_params = XMLEntityRenderableObject l_instance &l_TagName false
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--PhysXBox
					l_params = XMLEntityPhysXBox l_instance &l_TagName "objecte interactiu" true
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Interactiu
					l_params = XMLEntityInteractive l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Maquina d'estats
					if HasStateMachine l_instance then
					(
						--StateMachine
						l_params = XMLEntityStateMachine l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					--Rotative
					if IsRotative l_instance then
					(
						--Rotative
						l_params = XMLEntityRotation l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsDestroyable l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Destroyable %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
					--Renderable Object
					l_params = XMLEntityRenderableObject l_instance &l_TagName false
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--PhysXBox
					l_params = XMLEntityPhysXBox l_instance &l_TagName "escenari" true
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Destroyable
					l_params = XMLEntityDestroyable l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Explosive
					if IsExplosive l_instance then
					(
						l_params = XMLEntityExplosive l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					--Rotative
					if IsRotative l_instance then
					(
						--Rotative
						l_params = XMLEntityRotation l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					)
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsNavNode l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat NavNode %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--NavNode
					l_params = XMLEntityNavNode l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsSpawner l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Spawner %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Spawner
					l_params = XMLEntitySpawner l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsEmiter l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Emiter %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Emiter
					l_params = XMLEntityEmiter l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsBillboard l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Billboard %\n" l_instance.name
					
					l_params = #()
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Emiter
					l_params = XMLEntityBillboard l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsPhysxDummy l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Dummy Físic %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--PhysXBox
					l_params = XMLEntityPhysXBox l_instance &l_TagName "escenari" false
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsEntityAnimatedModel l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Model Animat %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--AnimatedModel
					l_params = XMLEntityRenderableObject l_instance &l_TagName true
					
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
				else if IsLight l_instance then
				(
					if IsLightEntity l_instance then
					(
						if l_instance.name == undefined then
						(
							format "\n  %" (OpenTagXML "Entity") to:l_file
						)
						else
						(
							format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
						)
						
						format "\tEntitat Llum %\n" l_instance.name
					
						--Light
						l_params = XMLEntityLight l_instance &l_TagName
						format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
						
						format "\n  %" (CloseTagXML "Entity") to:l_file
					)
				)
				else if IsCamera l_instance then
				(
					if l_instance.name == undefined then
					(
						format "\n  %" (OpenTagXML "Entity") to:l_file
					)
					else
					(
						format "\n  %" (OpenTagParamXML "Entity" #(#("name",l_instance.name))) to:l_file
					)
					
					format "\tEntitat Camera %\n" l_instance.name
					
					--Object3D
					l_params = XMLEntityObject3D l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					--Camera
					l_params = XMLEntityCamera l_instance &l_TagName
					format "\n    %" (InlineTagParamXML l_TagName l_params) to:l_file
					
					format "\n  %" (CloseTagXML "Entity") to:l_file
				)
			)
		)
		
		local l_player = GetPlayerEntity _entities
		if l_player != undefined then
		(			
			l_params = XMLEntityPlayer l_player &l_TagName
			format "\n  %" (InlineTagParamXML l_TagName l_params) to:l_file
			format "\tPlayer\n"
		)
		
		local l_enemies = GetEnemiesEntities _entities
		if l_enemies != undefined then
		(				
			for i = 1 to l_enemies.count do
			(
				l_params = XMLEntityEnemy l_enemies[i] &l_TagName
				format "\n  %" (InlineTagParamXML l_TagName l_params) to:l_file
				format "\tEnemic %\n" l_enemies[i].name
			)
		)
		
		-------------------------------------------------------
		
		format "\n%\n" (CloseTagXML "Entities") to:l_file
		
		close l_file
		
		return true
	)
	
	function ExportParticleEmitters =
	(
		local l_pathToFile = getXMLPath() 
		local l_particleEmitters = GetParticleEmitters $*
		local l_totalEmitters = l_particleEmitters.count
		local l_current = undefined
		makeDir l_pathToFile all:true
		
		l_pathToFile = l_pathToFile + "ParticleEmitters.xml"
		
		local l_file = createfile l_pathToFile
		local l_header = GetInitTagXML()
		
		local l_aux
		local l_params
		
		format "%" l_header to:l_file
		
		format "\n" to:l_file
		
		format "%" (OpenTagXML "ParticleEmitters") to:l_file
		
		------------------------------------------------------
		for i=1 to l_totalEmitters do
		(
			l_current = l_particleEmitters[i]
			l_params = #(#("id",l_current.name), #("type",(GetUserProp l_current "type")))
			
			/*l_aux = "" as stringStream
			local l_position = l_current.position
			format "% % %" l_position.x l_position.y l_position.z to:l_aux
			append l_params #("position",l_aux)*/
			
			l_aux2 = "" as stringStream	
			local l_translation = RHVectorToLH l_current.transform.translation
			format "% % %" l_translation.x l_translation.y l_translation.z to:l_aux2
			append l_params #("position",l_aux2)
			
			if classof l_current == Point then
			(
				append l_params #("FormEmitter","point")
				
			) else if classof l_current == Plane then
			(
				
				append l_params #("FormEmitter","plane")
				
				l_sizeX = ""as stringStream
				format "%"l_current.length to:l_sizeX
				append l_params #("sizeX",l_sizeX)
				
				l_sizeZ = ""as stringStream
				format "%"l_current.width to:l_sizeZ
				append l_params #("sizeZ",l_sizeZ)
				
				trans=l_current.transform
				my_value=point3 0 0 1
				my_value=my_value*trans
				normal=my_value-l_current.pos
				normal = RHVectorToLH(normalize normal)
				l_auxNormal = ""as stringStream
				format "% % %" normal.x normal.y normal.z to:l_auxNormal
				append  l_params #("Direction",l_auxNormal)
				
				
			) else if classof l_current == Dummy then
			(
				
				append l_params #("FormEmitter","dummy")
				
				l_aux = "" as stringStream
				local l_size = RHVectorToLH (DummySize l_current)
				--format "% % %" l_size.x l_size.y l_size.z to:l_aux
				--append l_params #("size",l_aux)
				
				l_sizeX = ""as stringStream
				format "%"l_size.x to:l_sizeX
				append l_params #("sizeX",l_sizeX)		

				l_sizeY = ""as stringStream
				format "%"l_size.y to:l_sizeY
				append l_params #("sizeY",l_sizeY)	
				
				l_sizeZ = ""as stringStream
				format "%"l_size.z to:l_sizeZ
				append l_params #("sizeZ",l_sizeZ)	
				
				trans=l_current.transform
				my_value=point3 0 0 1
				my_value=my_value*trans
				normal=my_value-l_current.pos
				normal = RHVectorToLH (normalize normal)
				l_auxNormal = ""as stringStream
				format "% % %" normal.x normal.y normal.z to:l_auxNormal
				append  l_params #("Direction",l_auxNormal)				

			) else if classof l_current == line then
			(	
				append l_params #("FormEmitter","line")



				
			) else if classof l_current == Box then
			(	
				append l_params #("FormEmitter","cube")
				l_sizeX = ""as stringStream
				format "%"l_current.length to:l_sizeX
				append l_params #("sizeX",l_sizeX)

				l_sizeY = ""as stringStream
				format "%"l_current.height to:l_sizeY
				append l_params #("sizeX",l_sizeY)
				
				l_sizeZ = ""as stringStream
				format "%"l_current.width to:l_sizeZ
				append l_params #("sizeZ",l_sizeZ)
			
			)
			--append l_params #(#("FormEmitter","plane"),#("sizeX",l_current.length),#("sizeZ",l_current.width))
				
			format "\n  %" (InlineTagParamXML "ParticleEmitterInstance" l_params) to:l_file
		)
		-------------------------------------------------------
		
		format "\n" to:l_file
		format "%" (CloseTagXML "ParticleEmitters") to:l_file
		
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
		if $ != undefined and IsMesh $ then
		(
			local l_check = CheckMesh $ true checkNormalRadiosity.checked
			
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
				
				if (ExportMesh $ l_outName l_relativeTexPath checkNormalRadiosity.checked) == true then
				(
					ShowMessage "File Saved"
				)
				
				-- copiar textures
				if checkTextures.checked == true then
				(
					TextureList l_textures $ checkNormalRadiosity.checked
				)
			)
		) else (
			ShowError "Sel·lecciona un únic objecte a exportar."
		)
	)
	on buttonMeshManager pressed do
	(
		--Check
		local l_check = true
		l_check = CheckSceneObjects $* checkNormalRadiosity.checked
			
		if l_check != true then
		(
			ShowError l_check
			return false
		)
		--Check end
		
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
		l_check = CheckSceneObjects $* checkNormalRadiosity.checked
			
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
	on buttonParticles pressed do
	(
		
		format "Export Particles...\n"
	
		if ExportParticleEmitters() then
		(
			ShowMessage "Export de les partícules finalitzat\n"
		)
		else
		(
			ShowMessage "Hi ha hagut un error"
		)
	)
	on ButtonPhysX pressed do
	(
		local l_cores = #()
		local l_instances = #()
		
		--Check
		local l_check = true
		l_check = CheckSceneObjects $* checkNormalRadiosity.checked
			
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
		
		if ExportEntities $* then
		(
			ShowMessage "Export de les Entities finalitzada\n"
		)
		else
		(
			ShowMessage "Hi ha hagut un error"
		)
	)
	on ButtonNivell pressed do
	(
		
		if ExportPortalsAndRooms() then
		(
			ShowMessage "Export de portals finalitzat\n"
		)
		else
		(
			ShowMessage "Hi ha hagut un error"
		)
	)
)
