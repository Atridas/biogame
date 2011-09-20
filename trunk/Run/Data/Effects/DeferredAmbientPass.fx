#include "Globals.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler LightBufferTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler SpecularTextureSampler : register(s2) = sampler_state
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
  //if( dot(l_DiffuseColor.xyz, 1.0) == 0 ) discard;
  
  //float4 l_LightColor = tex2D(LightBufferTextureSampler, _UV);
  
  //float4 l_AlphaColor = tex2D(AlphaBufferTextureSampler, _UV);
    
  //float4 l_DeferredColor = l_DiffuseColor * (float4(g_AmbientLight.rgb,1) + l_LightColor) + tex2D(SpecularTextureSampler, _UV);
  
  //return  l_AlphaColor * l_AlphaColor.w + l_DeferredColor * (1 - l_AlphaColor.w) ;
  
  return l_DiffuseColor * float4(g_AmbientLight.rgb,1) ;
  
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