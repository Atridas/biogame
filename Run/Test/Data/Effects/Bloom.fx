#include "Globals.fx"
#include "Samplers.fx"

// Optimització per separabilitat -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float g_XIncrementTexture = 1/512.0;
float g_YIncrementTexture = 1/512.0;
#define GLOW_KERNEL_HALF 4
//Triangle de pascal / tartaglia nivell 7
// 1   6  15  20   15  6   1       +++ => 64
float g_GaussianKernel[GLOW_KERNEL_HALF] = {0.315, 0.234375 , 0.09375, 0.015625};

float4 VerticalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Total = 0;
  for(int i = - (GLOW_KERNEL_HALF-1) ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_XTextureInc = 0;//i * g_XIncrementTexture;
    float l_YTextureInc = i * g_YIncrementTexture;
    
    float l_KT = g_GaussianKernel[abs(i)];
    float4 l_Color = tex2D(DiffuseTextureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
    l_Total += l_Color * l_KT;
  }
  return l_Total;
  //return l_Original + l_Total * l_Total.a;
}

float4 HoritzontalBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Total = 0;
  for(int i = - (GLOW_KERNEL_HALF-1) ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_XIncrementTexture;
    float l_YTextureInc = 0;//i * g_YIncrementTexture;
    
    float l_KT = g_GaussianKernel[abs(i)];
    float4 l_Color = tex2D(DiffuseTextureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
    l_Total += l_Color * l_KT;
  }
  return l_Total;
  //return l_Original + l_Total * l_Total.a;
}

float4 PostProcessBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  return tex2D(DiffuseTextureSampler, _UV );
}



technique HoritzontalBloomTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 HoritzontalBloomPS();
	}
}

technique VerticalBloomTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 VerticalBloomPS();
	}
}

technique PostProcessBloomTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 PostProcessBloomPS();
	}
}