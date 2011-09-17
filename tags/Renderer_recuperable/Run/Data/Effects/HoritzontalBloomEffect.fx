#include "Bloom.fx"

float4 HoritzontalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0/(float)g_TextureWidth;
  
  float4 l_Total = tex2D(PrevFilterSampler, _UV) * g_GaussianKernel[0];
  
  for(int i = 1 ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * l_XIncrementTexture;

    l_Total += tex2D(PrevFilterSampler, _UV + float2(l_XTextureInc,0)) * g_GaussianKernel[i];
    l_Total += tex2D(PrevFilterSampler, _UV - float2(l_XTextureInc,0)) * g_GaussianKernel[i];

  }
  
  return l_Total;

}

technique HoritzontalBloomTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 HoritzontalBloomPS();
	}
}
