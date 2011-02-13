#include "Globals.fx"

sampler PostFXSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 PostProcessBlurPS(float2 _UV : TEXCOORD0, float4 _Color: COLOR) : COLOR
{
  float4 l_PostFXColor = tex2D(PostFXSampler, _UV);
  return l_PostFXColor * _Color;
}
technique PostProcessBlurTechnique
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
		PixelShader = compile ps_3_0 PostProcessBlurPS();
  }
}