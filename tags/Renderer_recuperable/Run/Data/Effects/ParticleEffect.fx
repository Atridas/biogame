#include "VertexShaders.fx"
#include "PixelShaders.fx"

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
		PixelShader = compile ps_3_0 DiffuseTexturedPS();
	}
}
