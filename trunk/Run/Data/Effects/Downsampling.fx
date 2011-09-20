#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 DownSample4xPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0 / (float)(2 * g_TextureWidth  - 1.0);
  float l_YIncrementTexture = 1.0 / (float)(2 * g_TextureHeight - 1.0);
  
  float2 l_SamplePositions[]= { {-l_XIncrementTexture, -l_YIncrementTexture},
                                {+l_XIncrementTexture, -l_YIncrementTexture},
                                {-l_XIncrementTexture, +l_YIncrementTexture},
                                {+l_XIncrementTexture, +l_YIncrementTexture}
                              };
  
  float4 l_Total = float4(0, 0, 0, 0);
  for(int i = 0 ; i < 4; i++)
  {
    l_Total += tex2D(PrevFilterSampler, _UV + l_SamplePositions[i]);
  }
  return l_Total * 0.25;
  //return l_Original + l_Total * l_Total.a;
}

technique DownSample4xTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 DownSample4xPS();
	}
}