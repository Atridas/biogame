#ifndef __VERTEX_SHADERS_FX__
#define __VERTEX_SHADERS_FX__

#include "VertexType.fx"
#include "Globals.fx"
#include "Functions.fx"

float4 SimpleVS(float4 _in : POSITION) : POSITION {
	
	return mul(_in,g_WorldViewProjectionMatrix);
}

float4 NoVS() : POSITION {
	
	return float4(0,0,0,0);
}

TNORMAL_DIFFUSED_VERTEX_PS NormalDiffusedVS(TNORMAL_DIFFUSED_VERTEX_VS _in) {
	TNORMAL_DIFFUSED_VERTEX_PS out_ = (TNORMAL_DIFFUSED_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.Color = _in.Color;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
  
	return out_;
}

TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVS(TNORMAL_TEXTURED_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = normalize(mul(_in.Normal,(float3x3)g_WorldMatrix));
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.UV = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
	
	return out_;
}

TTEXTURED_VERTEX_PS TexturedVS(TTEXTURED_VERTEX_VS _in) {
	TTEXTURED_VERTEX_PS out_ = (TTEXTURED_VERTEX_PS)0;
	
	out_.UV = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	
	return out_;
}

TNORMAL_TEXTURED2_VERTEX_PS NormalTextured2VS(TNORMAL_TEXTURED2_VERTEX_VS _in) {
	TNORMAL_TEXTURED2_VERTEX_PS out_ = (TNORMAL_TEXTURED2_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.UV2 = _in.UV2.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
	
	return out_;
}

TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS TangentBinormalNormalTexturedVS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS _in) {
	TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS out_ = (TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal   = normalize(mul(_in.Normal,(float3x3)g_WorldMatrix));
	out_.WorldPosition = mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	out_.WorldTangent  = normalize(mul(_in.Tangent,(float3x3)g_WorldMatrix));
	out_.WorldBinormal = normalize(cross(out_.WorldNormal, out_.WorldTangent));
	out_.UV  = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
	
	return out_;
}

TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS TangentBinormalNormalTextured2VS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_VS _in) {
	TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS out_ = (TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS)0;
	
	out_.WorldNormal   = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.WorldTangent  = mul(_in.Tangent,(float3x3)g_WorldMatrix);
	out_.WorldBinormal = mul(_in.Binormal,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.UV2 = _in.UV2.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
	
	return out_;
}

// Funcions en TANGENT SPACE ----------------------------------------------------------------------------------------------------------------------------------------------------------------------


/*
struct TTANGENT_SPACE_TEXTURED_PS {
  float4 HPosition : POSITION;
  float2 UV : TEXCOORD0;
  
  float3 TangentSpacePosition : TEXCOORD2;
  float3 TangentSpaceCamera : TEXCOORD3;
  
  float4 PosLight      : TEXCOORD6;
  
  float3 LightPosition0:  TEXCOORD7;
  float3 LightDirection0: TEXCOORD8;
  float3 LightPosition1:  TEXCOORD9;
  float3 LightDirection1: TEXCOORD10;
  float3 LightPosition2:  TEXCOORD11;
  float3 LightDirection2: TEXCOORD12;
  float3 LightPosition3:  TEXCOORD13;
  float3 LightDirection3: TEXCOORD14;
  
};*/

TTANGENT_SPACE_TEXTURED_PS TangentSpaceTexturedVS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS _in) {
	TTANGENT_SPACE_TEXTURED_PS out_ = (TTANGENT_SPACE_TEXTURED_PS)0;
	
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  float4 vPos = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);
	out_.UV  = _in.UV.xy;
  
	float3 l_WorldNormal    = normalize(mul(_in.Normal,(float3x3)g_WorldMatrix));
	float3 l_WorldPosition  = mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	float3 l_WorldTangent   = normalize(mul(_in.Tangent,(float3x3)g_WorldMatrix));
	float3 l_WorldBitangent = normalize(mul(_in.Binormal,(float3x3)g_WorldMatrix));
  
  float3x3 l_WorldToTangentMatrix = float3x3(l_WorldTangent,l_WorldBitangent,l_WorldNormal);
  
  out_.TangentSpacePosition = mul(l_WorldPosition, l_WorldToTangentMatrix);
  out_.TangentSpaceCamera = mul(g_CameraPosition, l_WorldToTangentMatrix);
  
  out_.LightPosition0 = mul(g_LightsPosition[0],l_WorldToTangentMatrix);
  out_.LightPosition1 = mul(g_LightsPosition[1],l_WorldToTangentMatrix);
  out_.LightPosition2 = mul(g_LightsPosition[2],l_WorldToTangentMatrix);
  out_.LightPosition3 = mul(g_LightsPosition[3],l_WorldToTangentMatrix);
  
  out_.LightDirection0 = mul(g_LightsDirection[0],l_WorldToTangentMatrix);
  out_.LightDirection1 = mul(g_LightsDirection[1],l_WorldToTangentMatrix);
  out_.LightDirection2 = mul(g_LightsDirection[2],l_WorldToTangentMatrix);
  out_.LightDirection3 = mul(g_LightsDirection[3],l_WorldToTangentMatrix);
	
	return out_;
}


// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Retornem un TNORMAL_TEXTURED_VERTEX_PS (en un futur TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS)
//enlloc de CAL3D_HW_VERTEX_PS
TNORMAL_TEXTURED_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	TNORMAL_TEXTURED_VERTEX_PS out_=(TNORMAL_TEXTURED_VERTEX_PS)0;
	//float3 l_Normal= 0;
	//float3 l_Tangent=0;
	//CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal,l_Tangent);
	float3 l_Position = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	float3 l_Normal   = CalcAnimtedPos(IN.Normal, IN.Indices, IN.Weight);
	float4 l_WorldPosition=float4(l_Position, 1.0);
	out_.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);  
	out_.WorldNormal = mul(l_Normal,(float3x3)g_WorldMatrix);
	
  
  
	//OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	//OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);
	out_.UV = IN.TexCoord.xy;
	out_.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
  float4 vPos = mul(float4(l_Position,1.0),g_WorldViewMatrix);
  out_.PosLight = mul(vPos, g_ViewToLightProjectionMatrix);

	return out_;
}

#endif