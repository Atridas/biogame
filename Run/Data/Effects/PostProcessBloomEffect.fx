#include "Bloom.fx"

float4 PostProcessBloomPS(float2 _UV: TEXCOORD0) : COLOR
{
  return tex2D(PrevFilterSampler, _UV );
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