#include "Globals.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 DeferredAmbientPassPS(float2 _UV: TEXCOORD0) : COLOR
{
	float3 l_DiffuseColor = tex2D(ColorTextureSampler, _UV).rgb;
  
  float3 l_LightResult = g_AmbientLight * l_DiffuseColor;
  
  float4 out_ = float4(l_LightResult.xyz,1.0);
	return out_;
}

technique DeferredAmbientPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = Zero;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 DeferredAmbientPassPS();
	}
}