#include "Bloom.fx"

float4 PostProcessBloom2PS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Color = tex2D(PrevFilterSampler, _UV );
  
  if(l_Color.a == 0) discard;
  
  return l_Color;
}

technique PostProcessBloomTechnique2
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 PostProcessBloom2PS();
	}
}