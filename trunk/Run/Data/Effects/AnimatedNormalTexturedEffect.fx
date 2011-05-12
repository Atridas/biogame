#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique Cal3DTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}