#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "Functions.fx"

CAL3D_HW_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	//CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal,l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	float4 l_WorldPosition=float4(l_Position, 1.0);
	OUT.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);
	OUT.WorldNormal=normalize(mul(IN.Normal,g_WorldMatrix));
	//OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	//OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);
	OUT.UV = IN.TexCoord.xy;
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
	return OUT;
}

float4 RenderCal3DHWPS(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	//float3 Nn=CalcNormalmap(IN.WorldPosition, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
  //float3 Nn=CalcNormalmap(IN.WorldTangent, IN.WorldBinormal, IN.WorldNormal, IN.UV);
	//float4 l_SpecularColor = 1.0;
	float4 l_DiffuseColor=tex2D(DiffuseTextureSampler, IN.UV);
	//return CalcLighting (IN.WorldPosition, Nn, l_DiffuseColor, l_SpecularColor);
  return l_DiffuseColor;
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
		PixelShader = compile ps_3_0 RenderCal3DHWPS();
	}
}