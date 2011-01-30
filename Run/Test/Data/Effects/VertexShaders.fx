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