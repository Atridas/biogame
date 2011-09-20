#include "Globals.fx"

sampler LuminanceTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 DownsampleLuminancePS(float2 _UV: TEXCOORD0) : COLOR
{
	float4 l_Luminance[9];
  float l_fXOffset = 1.0 / (3 * g_TextureWidth  - 1.0);
  float l_fYOffset = 1.0 / (3 * g_TextureHeight - 1.0);
  
  l_Luminance[0] = tex2D(LuminanceTextureSampler, _UV);
  l_Luminance[1] = tex2D(LuminanceTextureSampler, _UV + float2( l_fXOffset, l_fYOffset));
  l_Luminance[2] = tex2D(LuminanceTextureSampler, _UV + float2( l_fXOffset,-l_fYOffset));
  l_Luminance[3] = tex2D(LuminanceTextureSampler, _UV + float2(-l_fXOffset,-l_fYOffset));
  l_Luminance[4] = tex2D(LuminanceTextureSampler, _UV + float2(-l_fXOffset, l_fYOffset));
  l_Luminance[5] = tex2D(LuminanceTextureSampler, _UV + float2( 0         , l_fYOffset));
  l_Luminance[6] = tex2D(LuminanceTextureSampler, _UV + float2( 0         ,-l_fYOffset));
  l_Luminance[7] = tex2D(LuminanceTextureSampler, _UV + float2(-l_fXOffset, 0         ));
  l_Luminance[8] = tex2D(LuminanceTextureSampler, _UV + float2( l_fXOffset, 0         ));
  
  float l_fLuminance = 0.0f;
  float l_fMax = -1e20f;
  for(int i = 0; i < 9; i++)
  {
    l_fLuminance +=  l_Luminance[i].x ;
    l_fMax = max(l_fMax, l_Luminance[i].y );
  }
  return float4(l_fLuminance / 9.0f, l_fMax, 0.0, 1.0);
}

technique DownsampleLuminanceTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 DownsampleLuminancePS();
	}
}
