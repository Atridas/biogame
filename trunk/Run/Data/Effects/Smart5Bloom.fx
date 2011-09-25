#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 Smart5BloomVerticalPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_YIncrementTexture = 1.0 / (float)(g_TextureHeight);
  
  float2 l_SamplePositions[]= { {0, 0},
                                {0, -l_YIncrementTexture * 1.2},
                                {0, +l_YIncrementTexture * 1.2}
                              };
  float l_SampleScale[]= { g_GaussMultiplier * 6.0 / 16.0, g_GaussMultiplier * 5.0 / 16.0, g_GaussMultiplier * 5.0 / 16.0 };
  
  float4 l_Total = float4(0, 0, 0, 0);
  for(int i = 0 ; i < 3; i++)
  {
    l_Total += tex2D(PrevFilterSampler, _UV + l_SamplePositions[i]) * l_SampleScale[i];
  }
  return l_Total;
  //return l_Original + l_Total * l_Total.a;
}

float4 Smart5BloomHoritzontalPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0 / (float)(g_TextureWidth);
  
  float2 l_SamplePositions[]= { {0, 0},
                                {-l_XIncrementTexture * 1.2, 0.0},
                                {+l_XIncrementTexture * 1.2, 0.0}
                              };
  float l_SampleScale[]= { 6.0 / 16.0, 5.0 / 16.0, 5.0 / 16.0 };
  
  float4 l_Total = float4(0, 0, 0, 0);
  for(int i = 0 ; i < 3; i++)
  {
    l_Total += tex2D(PrevFilterSampler, _UV + l_SamplePositions[i]) * l_SampleScale[i];
  }
  return l_Total;
  //return l_Original + l_Total * l_Total.a;
}

technique Smart5BloomVerticalTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 Smart5BloomVerticalPS();
	}
}

technique Smart5BloomHoritzontalTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 Smart5BloomHoritzontalPS();
	}
}
