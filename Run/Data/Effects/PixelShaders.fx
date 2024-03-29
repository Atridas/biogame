#ifndef __PIXEL_SHADERS_FX__
#define __PIXEL_SHADERS_FX__

#include "VertexType.fx"
#include "Functions.fx"
#include "Samplers.fx"

float4 LightmapPS(TNORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
	float3 l_Normal = normalize(_in.WorldNormal);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);
	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
	return l_LightResult;
}

float4 LightmapNormalmapPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
  float4 l_Bump;
	float3 l_Normal = CalcNormalmap((float3)_in.WorldTangent, 
                                  (float3)_in.WorldBinormal, 
                                  (float3)_in.WorldNormal, 
                                  _in.UV, l_Bump);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);
	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
	return l_LightResult;
}

float4 RadiosityNormalmapPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
  float4 l_Bump;
	float3 l_Normal = CalcNormalmap((float3)_in.WorldTangent, 
                                  (float3)_in.WorldBinormal, 
                                  (float3)_in.WorldNormal, 
                                  _in.UV, l_Bump);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = RadiosityNormalLightmapColor(l_Bump, _in.UV2);
	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
	return l_LightResult;
}

float4 ShowNormalsPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float3 l_normal = normalize(_in.WorldNormal);
	//return float4(0.5*l_normal + 0.5,1.0);
	return float4(l_normal,1.0);
}

float4 ShowNormalmapPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float4 l_Bump;
  float3 l_DiffuseColor = CalcNormalmap((float3)_in.WorldTangent, 
                                        (float3)_in.WorldBinormal, 
                                        (float3)_in.WorldNormal, 
                                        _in.UV, l_Bump);
	return float4(l_DiffuseColor, 1.0);
}

float4 ShowTangentPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	return float4(_in.WorldTangent.xyz, 1.0);
}

float4 ShowBitangentPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	return float4(_in.WorldBinormal.xyz, 1.0);
}

float4 ShowFlatNormalmapPS(TTEXTURED_VERTEX_VS _in) : COLOR {
  float4 l_DiffuseColor = tex2D(NormalTextureSampler, _in.UV);
	return l_DiffuseColor;
}

float4 ShowUVCoordsPS(TTEXTURED_VERTEX_VS _in) : COLOR {
	return float4(_in.UV.x, _in.UV.y, 0.0, 1.0);
}

float4 NormalTexturedPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = true;
  
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
  
  return out_;
}

float4 DiffuseTexturedPS(TTEXTURED_DIFFUSSED_VERTEX_PS _in) : COLOR {

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  
  return l_DiffuseColor * _in.Color;
}

float4 TexturedPS(TTEXTURED_VERTEX_PS _in) : COLOR {

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  
  return l_DiffuseColor;
}

float4 NormalDiffusedPS(TNORMAL_DIFFUSED_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = true;
  
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = _in.Color;

  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
  
  return out_;
}

float4 TangentBinormalNormalTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float2 l_OUT;
  
  bool l_DynamicObject = true;

  float3 l_Normal = normalize(CalcParallaxMap(_in.WorldPosition, _in.WorldNormal, _in.WorldTangent, _in.WorldBinormal, _in.UV, l_OUT));
	
  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,l_OUT);
  
  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);

  return out_;
}

float4 TangentBinormalNormalTexturedNoParallaxPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = true;
  
  float4 l_Bump;
  float3 l_Normal = normalize(CalcNormalmap(_in.WorldTangent, _in.WorldBinormal, _in.WorldNormal, _in.UV, l_Bump));
	
  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  
  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, g_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);

  return out_;
}

float4 SpecularTexturedPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  
  bool l_DynamicObject = true;
  
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);

  float  l_SpotlightFactor = 1.0;
	
  if(g_SpecularActive)
  {
	l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
  }

  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, l_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
  
  return out_;
}

float4 SpecularNormalmapTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = true;
  
  float4 l_Bump;
  float3 l_Normal = normalize(CalcNormalmap(_in.WorldTangent, _in.WorldBinormal, _in.WorldNormal, _in.UV, l_Bump));

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  
  float  l_SpotlightFactor = 1.0;
	
  if(g_SpecularActive)
  {
	l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
  }

  float4 out_ = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  g_AmbientLight, l_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
  
  return out_;
}

float4 SpecularLightmapTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
	float3 l_Normal = normalize(_in.WorldNormal);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);

	float  l_SpotlightFactor = 1.0;
	
	if(g_SpecularActive)
	{
		l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
	}

	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, l_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
	return l_LightResult;
}

float4 SpecularLightmapNormalmapTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
  float4 l_Bump;
	float3 l_Normal = CalcNormalmap((float3)_in.WorldTangent, 
                                  (float3)_in.WorldBinormal, 
                                  (float3)_in.WorldNormal, 
                                  _in.UV, l_Bump);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);

	float  l_SpotlightFactor = 1.0;
	
	if(g_SpecularActive)
	{
		l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
	}

	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, l_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject),l_DiffuseColor.a);
	return l_LightResult;
}

float4 SpecularRadiosityNormalmapTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {

  bool l_DynamicObject = false;
  
  float4 l_Bump;
	float3 l_Normal = CalcNormalmap((float3)_in.WorldTangent, 
                                  (float3)_in.WorldBinormal, 
                                  (float3)_in.WorldNormal, 
                                  _in.UV, l_Bump);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = RadiosityNormalLightmapColor(l_Bump, _in.UV2);
	
	float  l_SpotlightFactor = 1.0;
	
	if(g_SpecularActive)
	{
		l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
	}

	float4 l_LightResult = float4(ComputeAllLights( l_Normal, _in.WorldPosition, l_DiffuseColor, 
                                                  l_LightmapColor, l_SpotlightFactor,
                                                  _in.PosLight,l_DynamicObject)
                                ,l_DiffuseColor.a);
	return l_LightResult;
}

float4 WhitePS() : COLOR {
  return float4(1.0,1.0,1.0,1.0);
}

float4 ColorPS(float4 _color : COLOR) : COLOR {
  return _color;
}

float4 NoPS() : COLOR {
  return float4(0.0,0.0,0.0,0.0);
}

//Pixel Shader
void PixShadow( float2 _Depth : TEXCOORD0, out float4 Color_ : COLOR )
{
  //
  // Depth is z / w
  //
  Color_ = _Depth.x / _Depth.y;
}

//Pixel Shader
void PixGlow( float2 _UV : TEXCOORD0, out float4 Color_ : COLOR )
{
  if(g_GlowActive)
  {
    Color_ = tex2D(GlowTextureSampler,_UV) * g_GlowIntensity;
  } else {
    Color_ = float4(0,0,0,0);
  }
}

#endif