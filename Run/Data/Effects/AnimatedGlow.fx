#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique AnimatedGlowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 AnimatedGlow();
		PixelShader = compile ps_3_0 PixGlow();
	}
}


