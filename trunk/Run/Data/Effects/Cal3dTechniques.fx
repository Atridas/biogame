#include "Globals.fx"
#include "VertexType.fx"
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
		CullMode = CCW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		//PixelShader = compile ps_3_0 RenderCal3DHWPS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}