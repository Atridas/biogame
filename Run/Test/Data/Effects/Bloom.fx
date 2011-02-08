#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

// Optimització per separabilitat -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//float g_XIncrementTexture = 1/g_TextureWidth;
//float g_YIncrementTexture = 1/g_TextureHeight;

//Triangle de pascal / tartaglia nivell 7

//       1
//     1 1
//    1 2 1
//  1 3 3 1
//1 4 6 4 1   => 16
// 1   6  15  20   15  6   1       +++ => 64
//#define GLOW_KERNEL_HALF 3
//float g_GaussianKernel[GLOW_KERNEL_HALF] = {0.375 , 0.25, 0.0625};
//#define GLOW_KERNEL_HALF 4
//float g_GaussianKernel[GLOW_KERNEL_HALF] = {0.315, 0.234375 , 0.09375, 0.015625};

#define GLOW_KERNEL_HALF 7
float g_GaussianKernel[7] = { 0.3989422804,
                        0.2419707245,
                        0.0539909665,
                        0.0044318484,
                        0.2419707245,
                        0.0539909665,
                        0.0044318484};

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

float4 PostProcessBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  return tex2D(PrevFilterSampler, _UV );
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

technique PostProcessBloomTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 PostProcessBloomPS();
	}
}