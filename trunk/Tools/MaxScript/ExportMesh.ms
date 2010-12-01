

function ExportMaterial mat file=
(
	if mat.diffuseMapEnable then
	(
		WriteShort file mat.diffuseMap.fileName.count #unsigned
		WriteString file mat.diffuseMap.fileName
	)
	
	if mat.selfIllumMapEnable then
	(
		WriteShort file mat.selfIllumMap.fileName.count  #unsigned
		WriteString file mat.selfIllumMap.fileName
	) 
	
	if mat.bumpMapEnable then
	(
		WriteShort file mat.bumpMap.fileName.count #unsigned
		WriteString file mat.bumpMap.fileName
	)
	if mat.reflectionMapEnable then
	(
		WriteShort file mat.reflectionMap.fileName.count #unsigned
		WriteString file mat.reflectionMap.fileName
	)
	
)

function ExportMesh Obj=
(
	local file = fopen "d:/mesh.mesh" "wb"
	
	WriteShort file 0x5b0f #unsigned
	
	
	ExportMaterial obj.material file
	
	WriteShort file 0x5b0c #unsigned
	fflush file
	fclose file
	
)

ExportMesh $