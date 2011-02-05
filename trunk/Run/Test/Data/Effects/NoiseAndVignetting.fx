#include "Globals.fx"

sampler VignettingSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler NoiseSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 RenderNoiseAndVignettingPS(float2 UV : TEXCOORD0) : COLOR
{
  float2 l_Offset=float2(g_Time*0.39, g_Time*0.79);
  //float2 l_Offset=0;
  float2 l_UV=UV+l_Offset;
  float4 l_VignettingColor = tex2D(VignettingSampler, UV);
  float4 l_NoiseColor=tex2D(NoiseSampler, l_UV);
  return float4(l_NoiseColor.xyz*l_VignettingColor.xyz, l_NoiseColor.a+l_VignettingColor.a);
}
technique RenderNoiseAndVignettingTechnique
{
  pass p0
  {
    CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		//VertexShader = compile vs_3_0 RenderNoiseAndVignettingVS();
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
  }
}