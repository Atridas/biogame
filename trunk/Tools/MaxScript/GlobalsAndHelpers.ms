global MATERIAL_INFO_VERTEXTYPE   = 0X0001
global MATERIAL_INFO_TEXTURETYPE = 0x0002

global VERTEX_TYPE_GEOMETRY        = 0x0001
global VERTEX_TYPE_NORMAL            = 0x0002
global VERTEX_TYPE_TANGENT          = 0x0004
global VERTEX_TYPE_BINORMAL         = 0x0008
global VERTEX_TYPE_TEXTURE1         = 0x0010
global VERTEX_TYPE_TEXTURE2         = 0x0020
global VERTEX_TYPE_DIFFUSE           = 0x0040
                                 
/*                                 
global TEXTURE_TYPE_DIFFUSSE        = 0X0001
global TEXTURE_TYPE_NORMALMAP     = 0X0002
global TEXTURE_TYPE_LIGHTMAP        = 0X0004
global TEXTURE_TYPE_ENVIRONMENT  = 0X0008
global TEXTURE_TYPE_GLOW             = 0X0010
*/

global DIFFUSE_MATERIAL_MASK         = 0x0002
global NORMALMAP_MATERIAL_MASK    = 0x0004
global LIGHTMAP_MATERIAL_MASK       = 0x0008
global SPECULARMAP_MATERIAL_MASK  = 0x0010
global GLOW_MATERIAL_MASK             = 0x0020
global PARALLAX_MATERIAL_MASK        = 0x0040
global SPECULAR_MATERIAL_MASK        = 0x0080

global PARALLAX_PROPERTY_TYPE        = 1
global BUMP_PROPERTY_TYPE              = 2
global SPECULAR_PROPERTY_TYPE        = 3

global DEFAULT_BUMP_VALUE              = 0.6
global DEFAULT_PARALLAX_VALUE        = 0.02

--global GAME_PATH = "Data/Assets/Textures/"

function GetYaw transform =
(
	return (transform as eulerangles).z
)

function GetPitch transform =
(
	return (transform as eulerangles).y
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

--Comprova si un element pertany a una array.
--Retorna true o false
function Member _array _element =
(
	for i = 1 to _array.count do
	(
		if _array[i] == _element then
		(
			return true
		)
	)
	return false
)

--Comprova si un element pertany a una array
--Retorna l'índex de l'element o false
function MemberPosition _array _element =
(
	for i = 1 to _array.count do
	(
		if _array[i] == _element then
		(
			return i
		)
	)
	return false
)

--Imprimeix un array recursivament.
function PrintArray _array =
(
	if classof _array == Array then
	(
		format "[\n"
		l_total = _array.count
		for i = 1 to l_total do
		(
			format "	% =>\n" i
			PrintArray _array[i]
		)
		format "]\n"
	)else
	(
		format "%\n" _array
	)
)