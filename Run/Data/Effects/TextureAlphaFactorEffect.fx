#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 TextureAlphaFactorPS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Color = tex2D(PrevFilterSampler,_UV);
  float l_Alpha = l_Color.a*g_AlphaFactor;
  
  if(l_Alpha == 0) discard;
  
  return float4(l_Color.rgb, l_Alpha);
}

technique TextureAlphaFactorTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 TextureAlphaFactorPS();
	}
}