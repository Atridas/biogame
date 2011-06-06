#include "VertexShaders.fx"
#include "PixelShaders.fx"

technique RenderGlowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
    AlphaBlendEnable = true;
    SrcBlend = SrcAlpha;
    DestBlend = InvSrcAlpha;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertGlow();
		PixelShader = compile ps_3_0 PixGlow();
	}
}

technique RenderGlowAlphaTechnique {
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
		VertexShader = compile vs_3_0 VertGlow();
		PixelShader = compile ps_3_0 PixGlow();
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
		VertexShader = compile vs_3_0 VertGlow();
		PixelShader = compile ps_3_0 PixGlow();
	}
}

/*
float g_XIncrementTexture = 1/512.0;
float g_YIncrementTexture = 1/512.0;
#define GLOW_KERNEL_HALF 4
float g_KT[GLOW_KERNEL_HALF] = {1.0, 0.8, 0.6, 0.4};

float4 PostProcessGlowPS(float2 _UV: TEXCOORD0) : COLOR
{
  float4 l_Total = 0;
  int l_Counter = 0;
  for(int i = - (GLOW_KERNEL_HALF-1) ; i < (GLOW_KERNEL_HALF); i++)
  {
    float l_XTextureInc = i * g_XIncrementTexture;
    for(int j = - (GLOW_KERNEL_HALF-1) ; j < (GLOW_KERNEL_HALF); j++)
    {
      float l_YTextureInc = j * g_YIncrementTexture;
      float l_KT = (g_KT[abs(i)] + g_KT[abs(j)]) * 0.5;
      float4 l_Color = tex2D(DiffuseTextureSampler, _UV + float2(l_XTextureInc,l_YTextureInc));
      
      if(l_Color.a > 0.0 )
      {
        l_Total += l_Color * l_KT;
        l_Color.a = 1.0;
      } else {
        l_Color.a = 0.0;
      }
      l_Counter += l_Color.a;
    }
  }
  return l_Total / max(1, l_Counter);
}

technique PostProcessGlowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 PostProcessGlowPS();
	}
}
*/

