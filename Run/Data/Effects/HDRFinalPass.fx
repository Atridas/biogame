#include "Globals.fx"
#include "Functions.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler BloomTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler LuminanceTextureSampler : register(s2) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler GlowPassTextureSampler : register(s3) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 HDRFinalPassPS(float2 _UV: TEXCOORD0) : COLOR
{
	float4 l_DiffuseColor = tex2D(ColorTextureSampler, _UV);
	float4 l_BloomColor = tex2D(BloomTextureSampler, _UV);
  float4 l_Glow = tex2D(GlowPassTextureSampler, _UV);
	float4 l_LuminanceColor = tex2D(LuminanceTextureSampler, float2(0.5, 0.5) );
  
  
  //return l_BloomColor;
  
  float4 l_fFinalColor = l_DiffuseColor + l_BloomColor * g_BloomFinalScale + l_Glow * g_GlowFinalScale;
  
  //return l_fFinalColor;
  
  float l_fLuminance = l_LuminanceColor.r;
  float l_fMaxLuminance = l_LuminanceColor.g;
  
  l_fLuminance    = clamp(l_fLuminance   , g_SceneLuminanceLowerLimit, g_SceneLuminanceUpperLimit);
  l_fMaxLuminance = clamp(l_fMaxLuminance, g_MaxLuminanceLowerLimit  , g_MaxLuminanceUpperLimit);
  
  float Lp = (g_Exposure / l_fLuminance) * GetLuminance(l_fFinalColor); //max( l_fFinalColor.r, max( l_fFinalColor.g, l_fFinalColor.b ) );
  
  float LmSqr = (l_fMaxLuminance + g_GaussMultiplier * l_fMaxLuminance) * (l_fMaxLuminance + g_GaussMultiplier * l_fMaxLuminance);
  
  float toneScalar = ( Lp * ( 1.0f + ( Lp / ( LmSqr ) ) ) ) / ( 1.0f + Lp );
  
  //float Y = GetLuminance(l_DiffuseColor);
  //float toneScalar = g_Exposure * (g_Exposure / l_fMaxLuminance + 1.0) / (g_Exposure + 1.0);
  
  return clamp(l_fFinalColor * toneScalar, 0.0, 1.0);
}

technique HDRFinalPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 HDRFinalPassPS();
	}
}
