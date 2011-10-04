#include "Globals.fx"
#include "Functions.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
/*
sampler GlowPassTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
*/
sampler GlowDepthTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

struct PS_OUTPUT
{
	float4	Bloom     : COLOR0;
  float4	Luminance : COLOR1;
};

PS_OUTPUT HDRFirstPassPS(float2 _UV: TEXCOORD0)
{
  PS_OUTPUT out_;

	float4 l_DiffuseColor = tex2D(ColorTextureSampler, _UV);
  
  
  float l_fLuminance = GetLuminance(l_DiffuseColor);
  
  if(l_fLuminance >= g_BrightPassThreshold)
  {
    out_.Bloom = l_DiffuseColor;
  }
  else
  {
    out_.Bloom = float4(0.0, 0.0, 0.0, 1.0);
  }
  
  float4 l_Glow = l_DiffuseColor * tex2D(GlowDepthTextureSampler, _UV).y;
  
  out_.Bloom += max(0.0, 4 * l_Glow / g_GaussMultiplier);
  
  float l_fGlowLuminance = max(0.0, GetLuminance(l_Glow * 0.5));
  
  l_fLuminance += clamp(l_fGlowLuminance, 0.0, g_MaxGlowLuminance);
  
  out_.Luminance = float4(l_fLuminance, l_fLuminance, 0.0, 1.0);
  
  return out_;
}

technique HDRFirstPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 HDRFirstPassPS();
	}
}
