#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique NoTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    AlphaBlendEnable = true;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NoVS();
		PixelShader = compile ps_3_0 NoPS();
	}
}