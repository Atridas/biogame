#include "VertexShaders.fx"
//#include "PixelShaders.fx"



//#define NS_LIGHTING
#define NS_TEX0
#define NS_COLOR
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULARMAP
//#define NS_ENVIRONMENT
//#define NS_WHITE

#include "NewShaders.fx"



technique ParticleTechnique {
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
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 ParticleVS();
		PixelShader = compile ps_3_0 NewPS();
		//PixelShader = compile ps_3_0 DiffuseTexturedPS();
	}
}
