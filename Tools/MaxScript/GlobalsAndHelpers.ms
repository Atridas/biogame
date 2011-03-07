global MATERIAL_INFO_VERTEXTYPE   = 0X0001
global MATERIAL_INFO_TEXTURETYPE = 0x0002

global VERTEX_TYPE_GEOMETRY        = 0x0001
global VERTEX_TYPE_NORMAL            = 0x0002
global VERTEX_TYPE_TANGENT          = 0x0004
global VERTEX_TYPE_BINORMAL         = 0x0008
global VERTEX_TYPE_TEXTURE1         = 0x0010
global VERTEX_TYPE_TEXTURE2         = 0x0020
global VERTEX_TYPE_DIFFUSE           = 0x0040
                                 
                                 
global TEXTURE_TYPE_DIFFUSSE        = 0X0001
global TEXTURE_TYPE_NORMALMAP     = 0X0002
global TEXTURE_TYPE_LIGHTMAP        = 0X0004
global TEXTURE_TYPE_ENVIRONMENT  = 0X0008
global TEXTURE_TYPE_GLOW             = 0X0010

--global GAME_PATH = "Data/Assets/Textures/"

function GetYaw transform =
(
	return (transform as eulerangles).y
)

function GetPitch transform =
(
	return (transform as eulerangles).z
)

function GetRoll transform =
(
	return -1 * (transform as eulerangles).x
)

-- Max treballa en RH
function RHVectorToLH vector =
(
	return point3 vector.x vector.z vector.y
)

-- Max treballa amb la Y invertida
function ChangeUVCoords UV =
(
	local v = 1-Uv.y
	return point3 UV.x v UV.z
)

function ShowError _text =
(
	messageBox _text title:"Attention"
)

function ShowMessage _text =
(
	messageBox _text title:"Fine" beep:false
)