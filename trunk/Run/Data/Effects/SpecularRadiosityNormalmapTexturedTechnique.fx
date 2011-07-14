#define NS_LIGHTING
#define NS_TEX0
#define NS_LIGHTMAP
#define NS_NORMALMAP
//#define NS_CAL3D
#define NS_RADIOSITY_NORMALMAP
#define NS_SPECULAR

#include "NewShaders.fx"

technique SpecularRadiosityNormalmapTexturedTechnique {
	TECHNIQUE_BODY
}

technique SpecularRadiosityNormalmapTexturedAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}

/*
#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique SpecularRadiosityNormalmapTexturedTechnique {
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
		VertexShader = compile vs_3_0 TangentBinormalNormalTextured2VS();
		PixelShader = compile ps_3_0 SpecularRadiosityNormalmapTexturedPS();
	}
}
*/