
#define MAXBONES 29
#define MAXLIGHTS 4

// Matrixes
float4x4 g_WorldMatrix : World; 
float4x4 g_ViewMatrix : View;
float4x4 g_ProjectionMatrix : Projection; 
float4x4 g_WorldViewMatrix : WorldView; 
float4x4 g_ViewProjectionMatrix : ViewProjection; 
float4x4 g_WorldViewProjectionMatrix : WorldViewProjection; 
float4x4 g_ViewToLightProjectionMatrix : ViewToLightProjection;
float3x4 g_Bones[MAXBONES] : Bones;

float g_Bump = 2.4;
float g_BumpMaxHeight =  0.7;
float g_BumpMinHeight = -0.7;
float g_BumpDiff = 0.6;

float g_ParallaxHeight = 0.035f;

float g_SpecularPow = 30;
float g_SpotlightFactor = 0.5;

//Lights
#define LIGHT_OMNI        0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT        2


float3  g_AmbientLight                 : AmbientLight;

bool    g_LightsEnabled[MAXLIGHTS]     : LightsEnabled;
int     g_LightsType[MAXLIGHTS]        : LightsType;
float3  g_LightsPosition[MAXLIGHTS]    : LightsPosition;
float3  g_LightsDirection[MAXLIGHTS]   : LightsDirection;
float4  g_LightsColor[MAXLIGHTS]       : LightsColor;

float   g_LightsAngle[MAXLIGHTS]       : LightsAngle;
float   g_LightsFallOff[MAXLIGHTS]     : LightsFallOff;

float   g_LightsStartRange[MAXLIGHTS]  : LightsStartRange;
float   g_LightsEndRange[MAXLIGHTS]    : LightsEndRange;

//D3DXHANDLE m_pLightsEnabledParameter, m_pLightsTypeParameter, m_pLightsPositionParameter, m_pLightsDirectionParameter, m_pLightsAngleParameter, m_pLightsColorParameter;
//D3DXHANDLE m_pLightsFallOffParameter, m_pLightsStartRangeAttenuationParameter, m_pLightsEndRangeAttenuationParameter;


//Other
float3 g_CameraPosition : CameraPosition;
float g_Time : Time;
//D3DXHANDLE m_pBonesParameter;