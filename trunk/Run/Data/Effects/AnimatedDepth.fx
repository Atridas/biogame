#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique Cal3DTechniqueShadow
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 AnimatedShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}