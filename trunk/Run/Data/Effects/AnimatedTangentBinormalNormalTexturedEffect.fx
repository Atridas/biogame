#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique Cal3DNormalMapTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderTangentBitangentCal3DHWVS();
		PixelShader = compile ps_3_0 TangentBinormalNormalTexturedNoParallaxPS();
	}
}
