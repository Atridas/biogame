#ifndef __PIXEL_SHADERS_FX__
#define __PIXEL_SHADERS_FX__

#include "VertexType.fx"
#include "Functions.fx"
#include "Samplers.fx"

float4 LightmapPS(TNORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {
	float3 l_Normal = normalize(_in.WorldNormal);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightResult = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor, _in.PosLight),1.0);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);
	return l_LightResult*l_LightmapColor * 2;
}

float4 ShowNormalsPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float3 l_normal = normalize(_in.WorldNormal);
	//return float4(0.5*l_normal + 0.5,1.0);
	return float4(l_normal,1.0);
}

float4 ShowNormalmapPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float3 l_DiffuseColor = CalcNormalmap((float3)_in.WorldTangent, 
                                        (float3)_in.WorldBinormal, 
                                        (float3)_in.WorldNormal, 
                                        _in.UV);
	return float4(l_DiffuseColor, 1.0);
}

float4 ShowTangentPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	return float4(_in.WorldTangent.xyz*0.5+0.5, 1.0);
}

float4 ShowBitangentPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	return float4(_in.WorldBinormal.xyz*0.5+0.5, 1.0);
}

float4 ShowFlatNormalmapPS(TTEXTURED_VERTEX_VS _in) : COLOR {
  float4 l_DiffuseColor = tex2D(NormalTextureSampler, _in.UV);
	return l_DiffuseColor;
}

float4 ShowUVCoordsPS(TTEXTURED_VERTEX_VS _in) : COLOR {
	return float4(_in.UV.x, _in.UV.y, 0.0, 1.0);
}

float4 NormalTexturedPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  float4 out_ = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor,_in.PosLight),1.0);
  
  return out_;
}

float4 TexturedPS(TTEXTURED_VERTEX_PS _in) : COLOR {

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  
  return l_DiffuseColor;
}

float4 NormalDiffusedPS(TNORMAL_DIFFUSED_VERTEX_PS _in) : COLOR {
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = _in.Color;

  float4 out_ = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor,_in.PosLight),1.0);
  
  return out_;
}

float4 TangentBinormalNormalTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float2 l_OUT;

  float3 l_Normal = normalize(CalcParallaxMap(_in.WorldPosition, _in.WorldNormal, _in.WorldTangent, _in.WorldBinormal, _in.UV, l_OUT));
	
  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,l_OUT);
  
  float4 out_ = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor, _in.PosLight),1.0);

  return out_;
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

#endif