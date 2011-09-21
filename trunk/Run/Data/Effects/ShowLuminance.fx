#include "Globals.fx"

sampler LuminanceTextureSampler : register(s0) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 ShowLuminancePS(float2 _UV: TEXCOORD0) : COLOR
{
	float4 l_Luminance = tex2D(LuminanceTextureSampler, _UV);
  
  float l_fLum = l_Luminance.x;
  
  return float4(l_fLum, l_fLum, l_fLum, 1.0);
}

float4 ShowMaxLuminancePS(float2 _UV: TEXCOORD0) : COLOR
{
	float4 l_Luminance = tex2D(LuminanceTextureSampler, _UV);
  
  return float4(l_Luminance.y, l_Luminance.y, l_Luminance.y, 1.0);
}

technique ShowLuminanceTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 ShowLuminancePS();
	}
}

technique ShowMaxLuminanceTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		PixelShader = compile ps_3_0 ShowMaxLuminancePS();
	}
}
