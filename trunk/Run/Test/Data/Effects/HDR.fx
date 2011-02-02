#include "Globals.fx"

sampler FrameBufferSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR1 : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR2 : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR4 : register(s3) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR8 : register(s4) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR16 : register(s5) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR32 : register(s6) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR64 : register(s7) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR128 : register(s8) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR256 : register(s9) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR512 : register(s10) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};
sampler HDR1024 : register(s11) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float g_XIncrementTexture = 1/600.0; // amplada de la textura
float g_YIncrementTexture = 1/800.0; // alçada de la textura
float g_BloomThreshold = 0.95;
float g_KT[4] = {1.0, 0.8, 0.6, 0.4};
#define BLOOM_KERNEL_HALF 4

float luma(float3 _color)
{
  return 0.3 * _color.r + 0.59 * _color.g + 0.11 * _color.b;
}

float4 HDRPS(float2 _UV: TEXCOORD0) : COLOR
{
  //return float4(_UV, 0, 1);
  //return tex2D(FrameBufferSampler, _UV);
  float4 l_Total = 0;
  int l_Counter = 0;
  for(int i = - (BLOOM_KERNEL_HALF-1) ; i < (BLOOM_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_XIncrementTexture;
    for(int j = - (BLOOM_KERNEL_HALF-1) ; j < (BLOOM_KERNEL_HALF); j++)
    {
      float l_YTextureInc = j * g_YIncrementTexture;
      float l_KT = (g_KT[abs(i)] + g_KT[abs(j)]) * 0.5;
      float4 l_Color = tex2D(FrameBufferSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
      if(luma(l_Color) > g_BloomThreshold )
      {
        l_Total.xyz += l_Color.xyz * l_KT;
        l_Color.a = 1.0;
      } else {
        l_Color.a = 0.0;
      }
      l_Counter += l_Color.a;
    }
  }
  l_Total = l_Total / max(1, l_Counter);
  //l_Total += tex2D(HDR1024, _UV);
  //l_Total += tex2D(HDR512, _UV);
  float4 downsampleColor = float4(0,0,0,0);
  downsampleColor += tex2D(HDR1024, _UV);
  downsampleColor += tex2D(HDR512, _UV);
  downsampleColor += tex2D(HDR256, _UV);
  downsampleColor += tex2D(HDR128, _UV);
  downsampleColor += tex2D(HDR64, _UV);
  downsampleColor += tex2D(HDR32, _UV);
  downsampleColor += tex2D(HDR16, _UV);
  downsampleColor += tex2D(HDR8, _UV);
  downsampleColor += tex2D(HDR4, _UV);
  downsampleColor += tex2D(HDR2, _UV);
  downsampleColor += tex2D(HDR1, _UV);
  return l_Total + downsampleColor / 9;
}

technique HDRTechnique
{
  pass p0
  {
    CullMode = CCW;
    PixelShader = compile ps_3_0 HDRPS();
  }
}
