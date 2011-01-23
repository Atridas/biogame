#include "Globals.fx"
#include "VertexType.fx"
#include "Functions.fx"

#include "PixelShaders.fx"

CAL3D_HW_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	//float3 l_Normal= 0;
	//float3 l_Tangent=0;
	//CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal,l_Tangent);
	float3 l_Position = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	float3 l_Normal   = CalcAnimtedPos(IN.Normal, IN.Indices, IN.Weight);
	float4 l_WorldPosition=float4(l_Position, 1.0);
	OUT.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);  
	OUT.WorldNormal = mul(l_Normal,(float3x3)g_WorldMatrix);
  
  
	//OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	//OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);
	OUT.UV = IN.TexCoord.xy;
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
	return OUT;
}

technique Cal3DTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 TexturedPS();
    //PixelShader = compile ps_3_0 RenderCal3DHWPS();
	}
}