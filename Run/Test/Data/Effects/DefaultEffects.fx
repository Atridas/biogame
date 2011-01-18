

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"



TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVS(TNORMAL_TEXTURED_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.UV = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

float4 ShowNormalsPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float3 l_normal = normalize(_in.WorldNormal);
	//return float4(0.5*l_normal + 0.5,1.0);
	return float4(l_normal,1.0);
}


technique ShowNormalsTechnique {
	pass p0 {
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 ShowNormalsPS();
	}
}