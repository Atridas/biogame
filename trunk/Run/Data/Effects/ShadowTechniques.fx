#include "Globals.fx"
#include "Functions.fx"
#include "VertexType.fx"

//Vertex Shader
void VertShadow(float4 Pos : POSITION,
                out float4 oPos : POSITION,
                out float2 Depth : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
  oPos = mul( Pos, g_WorldViewProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth.xy = oPos.zw;
}

void AnimatedShadow(CAL3D_HW_VERTEX_VS _in,
                out float4 oPos : POSITION,
                out float2 Depth : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
	float3 l_Position = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
  oPos = mul( float4(l_Position,1.0), g_WorldViewProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth.xy = oPos.zw;
}

//Pixel Shader
void PixShadow( float2 _Depth : TEXCOORD0, out float4 Color_ : COLOR )
{
  //
  // Depth is z / w
  //
  Color_ = _Depth.x / _Depth.y;
}


technique RenderShadowTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}

technique Cal3DTechniqueShadow
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 AnimatedShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}
