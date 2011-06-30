#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique SpecularRadiosityNormalmapTexturedTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 TangentBinormalNormalTextured2VS();
		PixelShader = compile ps_3_0 SpecularRadiosityNormalmapTexturedPS();
	}
}