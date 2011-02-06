#include "Globals.fx"

float g_XIncrementTexture = 1/400.0; // amplada de la textura
float g_YIncrementTexture = 1/300.0; // alçada de la textura
float g_BloomThreshold = 0.90;
float g_KT[4] = {1.0, 0.8, 0.6, 0.4};
#define BLOOM_KERNEL_HALF 4


sampler FrameBufferSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler CaptureSampler : register(s1) = sampler_state
{
  MipFilter = POINT;
  MinFilter = POINT;  
  MagFilter = POINT;
  AddressU  = WRAP;
  AddressV  = WRAP;
};


float luma(float3 _color)
{
  return 0.3 * _color.r + 0.59 * _color.g + 0.11 * _color.b;
}

float4 BloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  //return float4(_UV, 0, 1);
  //return tex2D(DiffuseTextureSampler, _UV);
  float4 l_Total = 0;
  int l_Counter = 0;
  float3 l_BaseColor = tex2D(FrameBufferSampler, _UV);
  for(int i = - (BLOOM_KERNEL_HALF-1) ; i < (BLOOM_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_XIncrementTexture;
    for(int j = - (BLOOM_KERNEL_HALF-1) ; j < (BLOOM_KERNEL_HALF); j++)
    {
      float l_YTextureInc = j * g_YIncrementTexture;
      float l_KT = (g_KT[abs(i)] + g_KT[abs(j)]) * 0.5;
      float4 l_Color = tex2D(CaptureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
      if(luma(l_Color) > g_BloomThreshold )
      {
        l_Total += l_Color * l_KT;
        l_Color.a = 1.0;
      } else {
        l_Color.a = 0.0;
      }
      l_Counter += l_Color.a;
    }
  }
  l_Total = l_Total / max(1, l_Counter);
  float alpha = luma(l_Total.rgb);
  return float4(l_BaseColor * (1- alpha) + l_Total.xyz * alpha ,1);
}

technique BloomTechnique
{
  pass p0
  {
    CullMode = CCW;
    PixelShader = compile ps_3_0 BloomPS();
  }
}
