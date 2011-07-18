#include "Bloom.fx"

float4 VerticalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_YIncrementTexture = 1.0/(float)g_TextureHeight;
  
  float4 l_Total = tex2D(PrevFilterSampler, _UV) * g_GaussianKernel[0];
  for(int i = 1 ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_YTextureInc = i * l_YIncrementTexture;

    l_Total += tex2D(PrevFilterSampler, _UV + float2(0,l_YTextureInc)) * g_GaussianKernel[i];
    l_Total += tex2D(PrevFilterSampler, _UV - float2(0,l_YTextureInc)) * g_GaussianKernel[i];

  }
  
  return l_Total;
}

technique VerticalBloomTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 VerticalBloomPS();
	}
}
