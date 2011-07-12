
#define NS_LIGHTING
#define NS_TEX0
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULAR

#include "NewShaders.fx"

technique NormalTexturedTechnique {
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
		VertexShader = compile vs_3_0 NewVS();
		PixelShader  = compile ps_3_0 NewPS();
	}
}

technique NormalTexturedAlphaTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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
		VertexShader = compile vs_3_0 NewVS();
		PixelShader  = compile ps_3_0 NewPS();
	}
	pass p1 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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
		VertexShader = compile vs_3_0 NewVS();
		PixelShader  = compile ps_3_0 NewPS();
	}
}

/*
#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique NormalTexturedTechnique {
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
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}

technique NormalTexturedAlphaTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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