#include "Globals.fx"
#include "Functions.fx"
#include "VertexType.fx"

//Vertex Shader
void VertGlow(float4 _Pos : POSITION,
              float2 _UV  : TEXCOORD0,
              out float4 Pos_ : POSITION,
              out float2 UV_  : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
  Pos_ = mul( _Pos, g_WorldViewProjectionMatrix );
  // Pass texture coordinates
  UV_.xy = _UV.xy;
}

void AnimatedGlow(CAL3D_HW_VERTEX_VS _in,
                out float4 Pos_ : POSITION,
                out float2 UV_ : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
	float3 l_Position = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
  Pos_ = mul( float4(l_Position,1.0), g_WorldViewProjectionMatrix );
  // Pass texture coordinates
  UV_.xy = _in.TexCoord.xy;
}

//Pixel Shader
void PixGlow( float2 _UV : TEXCOORD0, out float4 Color_ : COLOR )
{
  if(g_GlowActive)
  {
    Color_ = tex2D(GlowTextureSampler,_UV);
  } else {
    Color_ = float4(0,0,0,0);
  }
}


technique RenderGlowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertGlow();
		PixelShader = compile ps_3_0 PixGlow();
	}
}

technique Cal3DGlowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 AnimatedGlow();
		PixelShader = compile ps_3_0 PixGlow();
	}
}

float g_XIncrementTexture = 1/512.0;
float g_YIncrementTexture = 1/512.0;
#define GLOW_KERNEL_HALF 4
float g_KT[GLOW_KERNEL_HALF] = {1.0, 0.8, 0.6, 0.4};

float4 PostProcessGlowPS(float2 _UV: TEXCOORD0) : COLOR
{
  //return float4(_UV, 0, 1);
  //return tex2D(DiffuseTextureSampler, _UV);
  float4 l_Total = 0;
  int l_Counter = 0;
  float4 l_Original = tex2D(NormalTextureSampler,_UV);
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
  //return l_Original + l_Total * l_Total.a;
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
