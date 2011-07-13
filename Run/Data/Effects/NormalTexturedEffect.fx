
#define NS_LIGHTING
#define NS_TEX0
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULARMAP

#include "NewShaders.fx"

technique NormalTexturedTechnique {
  TECHNIQUE_BODY
}

technique NormalTexturedAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}

/*
#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique NormalTexturedTechnique {
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
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}

technique NormalTexturedAlphaTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = false;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = true;
    SrcBlend = SrcAlpha;
    DestBlend = InvSrcAlpha;
    //Tipo de culling que queremos utilizar
    CullMode = CW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
	pass p1 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = false;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = true;
    SrcBlend = SrcAlpha;
    DestBlend = InvSrcAlpha;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}

*/