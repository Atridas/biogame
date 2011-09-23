/*
#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique WhiteTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 SimpleVS();
		PixelShader = compile ps_3_0 WhitePS();
	}
}

technique WhiteInstancedTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 SimpleInstancedVS();
		PixelShader = compile ps_3_0 WhitePS();
	}
}
*/


//#define NS_LIGHTING
//#define NS_TEX0
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULARMAP
//#define NS_ENVIRONMENT
#define NS_WHITE

#include "NewShaders.fx"

technique WhiteTechnique {
	TECHNIQUE_BODY
}

technique WhiteInstancedTechnique {
	ALPHA_TECHNIQUE_BODY
}