

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

TNORMAL_TEXTURED2_VERTEX_PS NormalTextured2VS(TNORMAL_TEXTURED2_VERTEX_VS _in) {
	TNORMAL_TEXTURED2_VERTEX_PS out_ = (TNORMAL_TEXTURED2_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.UV2 = _in.UV2.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

float4 LightmapPS(TNORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler,_in.UV2);
	return l_DiffuseColor*l_LightmapColor * 2;
}

float4 ShowNormalsPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float3 l_normal = normalize(_in.WorldNormal);
	return float4(0.5*l_normal + 0.5,1.0);
	//return float4(l_normal,1.0);
}

float4 ShowNormalmapPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float4 l_DiffuseColor = tex2D(NormalTextureSampler,_in.UV);
	return l_DiffuseColor;
	//return float4(l_normal,1.0);
}


technique ShowNormalsTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 ShowNormalsPS();
	}
}

technique ShowNormalmapTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 ShowNormalmapPS();
	}
}

technique LightmapTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTextured2VS();
		PixelShader = compile ps_3_0 LightmapPS();
	}
}