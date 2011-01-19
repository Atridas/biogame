
#define MAXBONES 29

float4x4 g_WorldMatrix : World; 
float4x4 g_ViewMatrix : View;
float4x4 g_ProjectionMatrix : Projection; 
float4x4 g_WorldViewMatrix : WorldView; 
float4x4 g_ViewProjectionMatrix : ViewProjection; 
float4x4 g_WorldViewProjectionMatrix : WorldViewProjection; 
float4x4 g_ViewToLightProjectionMatrix : ViewToLightProjection;
float3x4 g_Bones[MAXBONES];
  
//D3DXHANDLE m_pLightsEnabledParameter, m_pLightsTypeParameter, m_pLightsPositionParameter, m_pLightsDirectionParameter, m_pLightsAngleParameter, m_pLightsColorParameter;
//D3DXHANDLE m_pLightsFallOffParameter, m_pLightsStartRangeAttenuationParameter, m_pLightsEndRangeAttenuationParameter;

float3 g_CameraPosition : CameraPosition;
float g_Time : Time;
//D3DXHANDLE m_pBonesParameter;