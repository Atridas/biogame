#include "Globals.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler DepthPassTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler SkyTextureSampler : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 DeferredAmbientPassPS(float2 _UV: TEXCOORD0) : COLOR
{
	float4 l_DiffuseColor = tex2D(ColorTextureSampler, _UV);
	float l_Depth = tex2D(DepthPassTextureSampler, _UV).r;
  
  //float4 l_LightColor = tex2D(LightBufferTextureSampler, _UV);
    
  //float4 l_DeferredColor = l_DiffuseColor * (float4(g_AmbientLight.rgb,1) + l_LightColor) + tex2D(SpecularTextureSampler, _UV);
  
  
  if(l_Depth > 0.0)
    return l_DiffuseColor * float4(g_AmbientLight.rgb,1) ;
  else
    return tex2D(SkyTextureSampler, _UV);
  
}

technique DeferredAmbientPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 DeferredAmbientPassPS();
	}
}