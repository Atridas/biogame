#include "Globals.fx"

sampler LuminanceTextureSampler : register(s0) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 LuminanceBlendPassPS() : COLOR
{  
  float4 l_Luminance = tex2D(LuminanceTextureSampler, float2(0.5, 0.5));
  
  //float l_fLuminance = exp(l_Luminance.x);
  //float l_fMaxLuminance = l_Luminance.y;
  
  //if(l_fLuminance > g_MaxGlobalLuminance)
  //{
  //  l_fLuminance = g_MaxGlobalLuminance;
  //}
  //if(l_fMaxLuminance > g_MaxGlobalLuminance)
  //{
  //  l_fMaxLuminance = g_MaxGlobalLuminance;
  //}
  
  return float4(l_Luminance.xy, 0.0, min(1.0,g_Time));
  //return float4(l_Luminance.xy, 0.0, 1.0);
}

technique LuminanceBlendPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaTestEnable = false;	
    AlphaBlendEnable = true;
    SrcBlend = SrcAlpha;
    DestBlend = InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 LuminanceBlendPassPS();
	}
}