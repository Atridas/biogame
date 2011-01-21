

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "Functions.fx"



float4 SimpleVS(float4 _in : POSITION) : POSITION {
	
	return mul(_in,g_WorldViewProjectionMatrix);
}

float4 NoVS() : POSITION {
	
	return float4(0,0,0,0);
}

TNORMAL_DIFFUSED_VERTEX_PS NormalDiffusedVS(TNORMAL_DIFFUSED_VERTEX_VS _in) {
	TNORMAL_DIFFUSED_VERTEX_PS out_ = (TNORMAL_DIFFUSED_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.Color = _in.Color;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVS(TNORMAL_TEXTURED_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = normalize(mul(_in.Normal,(float3x3)g_WorldMatrix));
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.UV = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

TNORMAL_TEXTURED2_VERTEX_PS NormalTextured2VS(TNORMAL_TEXTURED2_VERTEX_VS _in) {
	TNORMAL_TEXTURED2_VERTEX_PS out_ = (TNORMAL_TEXTURED2_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.UV2 = _in.UV2.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS TangentBinormalNormalTexturedVS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS _in) {
	TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS out_ = (TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal   = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.WorldTangent  = mul(_in.Tangent,(float3x3)g_WorldMatrix);
	out_.WorldBinormal = mul(_in.Binormal,(float3x3)g_WorldMatrix);
	out_.UV  = _in.UV.xy;
	out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
	//out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS TangentBinormalNormalTextured2VS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_VS _in) {
	TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS out_ = (TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS)0;
	
	out_.WorldNormal   = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.WorldPosition = mul(_in.Position,(float3x3)g_WorldMatrix);
	out_.WorldTangent  = mul(_in.Tangent,(float3x3)g_WorldMatrix);
	out_.WorldBinormal = mul(_in.Binormal,(float3x3)g_WorldMatrix);
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
	//return float4(0.5*l_normal + 0.5,1.0);
	return float4(l_normal,1.0);
}

float4 ShowNormalmapPS(TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS _in) : COLOR {
  float3 l_DiffuseColor = CalcNormalmap((float3)_in.WorldTangent, 
                                        (float3)_in.WorldBinormal, 
                                        (float3)_in.WorldNormal, 
                                        _in.UV);
	return float4(l_DiffuseColor, 1.0);
	//return float4(l_normal,1.0);
}

float4 NormalTexturedPS(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  float4 out_ = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor),1.0);
  
  return out_;
}

float4 NormalDiffusedPS(TNORMAL_DIFFUSED_VERTEX_PS _in) : COLOR {
  float3 l_Normal = normalize(_in.WorldNormal);

  float4 l_DiffuseColor = _in.Color;
  
  float4 out_ = float4(ComputeAllLights(l_Normal, _in.WorldPosition, l_DiffuseColor),1.0);
  
  return out_;
}

float4 TangentBinormalNormalTexturedPS(TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
  NORMAL_UV l_aux = CalcParallaxMap(_in.WorldPosition, 
                                    _in.WorldNormal, 
                                    _in.WorldTangent, 
                                    _in.WorldBinormal, 
                                    _in.UV);

	//float3 l_Normal = CalcNormalmap(_in.WorldTangent, _in.WorldBinormal, _in.WorldNormal, _in.UV);
  
	
  float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,l_aux.UV);
  
  float4 out_ = float4(ComputeAllLights(l_aux.Normal, _in.WorldPosition, l_DiffuseColor),1.0);
  
	return out_;
}

float4 WhitePS() : COLOR {
  return float4(1.0,1.0,1.0,1.0);
}
float4 NoPS() : COLOR {
  return float4(0.0,0.0,0.0,0.0);
}

technique NormalDiffusedTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalDiffusedVS();
		PixelShader = compile ps_3_0 NormalDiffusedPS();
	}
}
technique NormalTexturedTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NormalTexturedVS();
		PixelShader = compile ps_3_0 NormalTexturedPS();
	}
}
technique TangentBinormalNormalTexturedTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 TangentBinormalNormalTexturedVS();
		PixelShader = compile ps_3_0 TangentBinormalNormalTexturedPS();
	}
}
technique ShowNormalsTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 TangentBinormalNormalTextured2VS();
		PixelShader = compile ps_3_0 ShowNormalmapPS();
	}
}

technique LightmapTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
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

technique WhiteTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    //Deshabilitamos el alphablend
    AlphaBlendEnable = false;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 SimpleVS();
		PixelShader = compile ps_3_0 WhitePS();
	}
}

technique NoTechnique {
	pass p0 {
		//Activamos el Zbuffer, el Zwrite y la funci�n de Z�s que queremos utilizar
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    AlphaBlendEnable = true;
    //Tipo de culling que queremos utilizar
    CullMode = CCW;
    //Vertex / Pixel shader
		VertexShader = compile vs_3_0 NoVS();
		PixelShader = compile ps_3_0 NoPS();
	}
}