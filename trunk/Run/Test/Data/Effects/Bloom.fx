#include "Globals.fx"
#include "Samplers.fx"

float g_IncrementTexture = 1/512.0;
float g_KT[4] = {1.0, 0.8, 0.6, 0.4};
#define BLOOM_KERNEL_HALF 4

float4 BloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Total = 0;
  int l_Counter = 0;
  for(int i = - (BLOOM_KERNEL_HALF-1) ; i < (BLOOM_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_IncrementTexture;
    for(int j = - (BLOOM_KERNEL_HALF-1) ; j < (BLOOM_KERNEL_HALF); j++)
    {
      float l_YTextureInc = j * g_IncrementTexture;
      float l_KT = (g_KT[abs(i)] + g_KT[abs(j)]) * 0.5;
      float4 l_Color = tex2D(DiffuseTextureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
      if(l_Color.a > 0 )
      {
        l_Total += l_Color;
        l_Color.a = 1.0;
      } else {
        l_Color.a = 0.0;
      }
      l_Counter += l_Color.a;
    }
  }
  return l_Total / max(1, l_Counter);
}

technique BloomTechnique
{
  pass p0
  {
    CullMode = CCW;
    PixelShader = compile ps_3_0 BloomPS();
  }
}
