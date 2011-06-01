#include "Bloom.fx"

float4 HoritzontalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0/(float)g_TextureWidth;
  
  float4 l_Total = float4(0, 0, 0, 0);
  for(int i = - (GLOW_KERNEL_HALF-1) ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * l_XIncrementTexture;
    
    float l_KT = g_GaussianKernel[abs(i)];
    float4 l_Color = tex2D(PrevFilterSampler, _UV + float2(l_XTextureInc,0));
      
    if(l_Color.a > 0.0)
    {
      l_Total += l_Color * l_KT;
    }
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
