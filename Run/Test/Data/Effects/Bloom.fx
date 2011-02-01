#include "Globals.fx"
#include "Samplers.fx"

float g_XIncrementTexture = 1/562.0; // amplada de la textura
float g_YIncrementTexture = 1/784.0; // alçada de la textura
float g_BloomThreshold = 0.95;
float g_KT[4] = {1.0, 0.8, 0.6, 0.4};
#define BLOOM_KERNEL_HALF 4

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
  float3 l_BaseColor;
  for(int i = - (BLOOM_KERNEL_HALF-1) ; i < (BLOOM_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_XIncrementTexture;
    for(int j = - (BLOOM_KERNEL_HALF-1) ; j < (BLOOM_KERNEL_HALF); j++)
    {
      float l_YTextureInc = j * g_YIncrementTexture;
      float l_KT = (g_KT[abs(i)] + g_KT[abs(j)]) * 0.5;
      float4 l_Color = tex2D(DiffuseTextureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
      if(luma(l_Color) > g_BloomThreshold )
      {
        l_Total.xyz += l_Color.xyz * l_KT;
        l_Total.a += l_KT;
        l_Color.a = 1.0;
      } else {
        l_Color.a = 0.0;
      }
      l_Counter += l_Color.a;
      
      if(i == 0 && j == 0)
      {
        l_BaseColor = l_Color.xyz;
      }
    }
  }
  l_Total = l_Total / max(1, l_Counter);
  float f = l_Total.a;
  return float4(l_BaseColor + l_Total.xyz * f ,1);
}

technique BloomTechnique
{
  pass p0
  {
    CullMode = CCW;
    PixelShader = compile ps_3_0 BloomPS();
  }
}
