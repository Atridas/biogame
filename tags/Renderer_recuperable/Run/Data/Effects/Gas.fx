#include "Globals.fx"

sampler GasSampler0 : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler GasSampler1 : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler GasSampler2 : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler GasSampler3 : register(s3) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler GasSampler4 : register(s4) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 GasPS(float2 _UV : TEXCOORD0, float4 _color : COLOR) : COLOR
{
  
  float2 l_weights = float2((sin(g_Time * 2.39 * 2) + 1)*0.5,
                            (sin(g_Time * 2.39    ) + 1)*0.5);
                              
  //l_weights = normalize(l_weights);
  
  float l_GasColor0 = tex2D(GasSampler0, _UV).x;
  float l_GasColor1 = tex2D(GasSampler1, _UV).x;
  float l_GasColor2 = tex2D(GasSampler2, _UV).x;
  float l_GasColor3 = tex2D(GasSampler3, _UV).x;
  float l_GasColor4 = tex2D(GasSampler4, _UV).x;
  
  float l_alpha = l_GasColor0 *      l_weights.x  +
                   l_GasColor1 * (1 - l_weights.x);
                   
                  /*l_GasColor2 *      l_weights.y  +
                  l_GasColor3 * (1 - l_weights.y) +
                  l_GasColor4 *      l_weights.z;*/
                  
  if(l_alpha < 0.5)
  {
    l_alpha = 0.0;
  } else {
    l_alpha = (l_alpha - 0.5) * 2;
  }
  
  return float4(1, 1, 1, l_alpha) * _color;
}
technique GasTechnique
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
		PixelShader = compile ps_3_0 GasPS();
  }
}