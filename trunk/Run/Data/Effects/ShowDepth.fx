#include "Globals.fx"


sampler PosXYTextureSampler : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler DepthTextureSampler : register(s3) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float LinearizeDepth(float2 uv)
{
  //float n = 1; // camera z near
  //float f = 100; // camera z far
  //float z = tex2D(PrevFilterSampler, uv).x;
  //return 1.0 - (2.0 * n) / (f + n - z * (f - n));
  float z = tex2D(DepthTextureSampler, uv).x;
  //return frac(z);
  return z / 100;
}

float4 ShowDepthPS(float2 _UV: TEXCOORD0) : COLOR
{
  float d;
  d = LinearizeDepth(_UV);
  
  return float4(d,d,d,1.0);
  
	//float3 l_vWorldPos;
	//l_vWorldPos.xy = tex2D(PosXYTextureSampler, _UV).xy;
	//l_vWorldPos.z = tex2D(DepthTextureSampler, _UV).x;
  
  //return float4(frac(l_vWorldPos),1.0);
}

technique ShowDepthTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 ShowDepthPS();
	}
}