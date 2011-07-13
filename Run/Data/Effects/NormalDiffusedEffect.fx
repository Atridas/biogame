//#include "VertexShaders.fx"
//#include "PixelShaders.fx"

#define NS_LIGHTING

#include "NewShaders.fx"

technique NormalDiffusedTechnique {
	TECHNIQUE_BODY
  /*
	pass p1 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Tipus de fill
    FillMode = WIREFRAME;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalDiffusedVS();
		PixelShader = compile ps_3_0 WhitePS();
	}
  */
}


technique NormalDiffusedAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}