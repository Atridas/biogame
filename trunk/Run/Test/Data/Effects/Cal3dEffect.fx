#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

TNORMAL_TEXTURED_VERTEX_PS mainVsSkeletalAnimation(appdata _IN,
													uniform float4x4 _WorldViewProj,
													uniform float4 _Diffuse,
													uniform float3x4 _Bones[MAXBONES])
{
	TNORMAL_TEXTURED_VERTEX_PS OUT_ = (TNORMAL_TEXTURED_VERTEX_PS)0;

	float4 l_f4TempPos;
	l_f4TempPos.xyz = _IN.Position.xyz;
	l_f4TempPos.w = 1.0;

	float3 l_f3Position= 0;
	float3 l_f3Normal= 0;
	float4 l_f4Index;

	l_f4Index = _IN.Indices;

	l_f3Position = mul(_Bones[l_f4Index.x], l_f4TempPos) * _IN.Weight.x;
	l_f3Position += mul(_Bones[l_f4Index.y], l_f4TempPos) * _IN.Weight.y;
	l_f3Position += mul(_Bones[l_f4Index.z], l_f4TempPos) * _IN.Weight.z;
	l_f3Position += mul(_Bones[l_f4Index.w], l_f4TempPos) * _IN.Weight.w;

	float3x3 l_f3x3BoneMatrix;

	l_f3x3BoneMatrix[0].xyz = _Bones[l_f4Index.x][0].xyz;
	l_f3x3BoneMatrix[1].xyz = _Bones[l_f4Index.x][1].xyz;
	l_f3x3BoneMatrix[2].xyz = _Bones[l_f4Index.x][2].xyz;

	l_f3Normal += mul(l_f3x3BoneMatrix, _IN.Normal)* _IN.Weight.x;

	l_f3x3BoneMatrix[0].xyz = _Bones[l_f4Index.y][0].xyz;
	l_f3x3BoneMatrix[1].xyz = _Bones[l_f4Index.y][1].xyz;
	l_f3x3BoneMatrix[2].xyz = _Bones[l_f4Index.y][2].xyz;

	l_f3Normal += normalize(mul(l_f3x3BoneMatrix, _IN.Normal)* _IN.Weight.y);
	l_f3Normal = normalize(l_f3Normal);

	OUT_.UV.xy = _IN.TexCoord.xy;
	OUT_.WorldNormal = normalize(mul(l_f3Normal,g_WorldMatrix));
	OUT_.HPosition = mul(_WorldViewProj,float4(l_f3Position.xyz,1));

	return OUT_;
}

technique cal3dTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 mainVsSkeletalAnimation(g_WorldViewProjMatrix,g_Diffuse,g_Bones);
		PixelShader = NULL;
	}
}