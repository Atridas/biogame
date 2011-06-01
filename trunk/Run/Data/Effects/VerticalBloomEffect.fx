#include "Bloom.fx"

float4 VerticalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_YIncrementTexture = 1.0/(float)g_TextureHeight;
  
  float4 l_Total = float4(0, 0, 0, 0);
  for(int i = - (GLOW_KERNEL_HALF-1) ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_YTextureInc = i * l_YIncrementTexture;
    
    float l_KT = g_GaussianKernel[abs(i)];
    float4 l_Color = tex2D(PrevFilterSampler, _UV + float2(0,l_YTextureInc));
    
    if(l_Color.a > 0.0)
      l_Total += l_Color * l_KT;
  }
  return l_Total;
  //return l_Original + l_Total * l_Total.a;
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
