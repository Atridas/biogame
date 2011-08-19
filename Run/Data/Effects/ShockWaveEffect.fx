#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 ShockWavePS(float2 _UV: TEXCOORD0) : COLOR
{
  //vec2 uv = gl_TexCoord[0].xy;
  float2 l_texCoord = _UV;
  float l_distance = distance(_UV, float2(200.0,200.0));
  if ( (l_distance <= (g_Time + 0.1)) &&
       (l_distance >= (g_Time - 0.1)) )
  {
    float l_diff = (l_distance - g_Time);
    float l_powDiff = 1.0 - pow(abs(l_diff*10.0),0.8);
    float l_diffTime = l_diff  * l_powDiff;
    float2 l_diffUV = normalize(_UV - float2(200.0,200.0));
    l_texCoord = _UV + (l_diffUV * l_diffTime);
  }
  return tex2D(PrevFilterSampler, l_texCoord);

}

technique ShockWaveTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 ShockWavePS();
	}
}