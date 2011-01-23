#ifndef __VERTEX_SHADERS_FX__
#define __VERTEX_SHADERS_FX__

#include "VertexType.fx"
#include "Globals.fx"

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
	
	return out_;
}

TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVS(TNORMAL_TEXTURED_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = normalize(mul(_in.Normal,(float3x3)g_WorldMatrix));
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
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
	
	return out_;
}

TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS TangentBinormalNormalTexturedVS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS _in) {
	TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS out_ = (TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal   = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.WorldTangent  = mul(_in.Tangent,(float3x3)g_WorldMatrix);
	out_.WorldBinormal = mul(_in.Binormal,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	
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
	
	return out_;
}

#endif