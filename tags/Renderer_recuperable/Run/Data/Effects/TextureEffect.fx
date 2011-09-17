#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 TexturePS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Color = tex2D(PrevFilterSampler,_UV);
  
  return float4(l_Color.rgb,1.0);
}

technique TextureTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 TexturePS();
	}
}