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
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}

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

technique Cal3dShowNormalsTechnique {
	pass p0 {
	//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 ShowNormalsPS();
	}
}