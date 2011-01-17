/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

//------------------------------------------------------------------
// Samplers
//------------------------------------------------------------------
texture Tex1  <
	string ResourceName = "";//Optional default file name
	string UIName =  "Tex1 Texture";
	string ResourceType = "2D";
>;

sampler2D Tex1Sampler = sampler_state {
	Texture = <Tex1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};


texture Lightmap  <
	string ResourceName = "";//Optional default file name
	string UIName =  "Lightmap Texture";
	string ResourceType = "2D";
>;

sampler2D LightmapSampler = sampler_state {
	Texture = <Lightmap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};

//--------------------------------------------------------------------------------
// Structs

//Para declarer una estructura de vértice que utilizaremos como entrada para el Vertex Shader
struct TNORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
};
//Para declarar una estructura de vértice que utilizaremos como salida para el Vertex Shader y
//entrada para el Píxel Shader
struct TNORMAL_TEXTURED_VERTEX_PS {
	float4 Hposition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
};
/*
struct TNORMAL_TEXTURED2_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
};

struct TNORMAL_TEXTURED2_VERTEX_PS {
	float4 Hposition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
};
*/

//------------------------------------------------------------------------------
// Lights
float3 g_Lamp0Direction : DIRECTION
<
	string Object = "Directional Light 0";
	string UIName =  "Lamp 0 Direction";
	string Space = "World";
> = {-0.5f,2.0f,1.25f};

float3 g_Lamp_0_color : COLOR <
	string Object = "Directional Light 0";
	string UIName =  "Lamp 0 Color";
	string UIWidget = "Color";
> = {1.0f,1.0f,1.0f};

float3 g_LightAmbient : COLOR;
float g_SpecularReflection;

//Matrixes
float4x4 g_WorldViewProjMatrix : WorldViewProjection;
float4x4 g_WorldMatrix : World;
float4 g_CameraPosition: CameraPosition;

TNORMAL_TEXTURED_VERTEX_PS mainVSNormalTextured(TNORMAL_TEXTURED_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.UV = _in.UV.xy;
	out_.Hposition = mul(float4(_in.Position,1.0),g_WorldViewProjMatrix);
	out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}

/*
TNORMAL_TEXTURED2_VERTEX_PS mainVSNormalTextured2(TNORMAL_TEXTURED2_VERTEX_VS _in) {
	TNORMAL_TEXTURED_VERTEX_PS out_ = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	out_.WorldNormal = mul(_in.Normal,(float3x3)g_WorldMatrix);
	out_.UV = _in.UV.xy;
	out_.UV2 = _in.UV2.xy;
	out_.Hposition = mul(float4(_in.Position,1.0),g_WorldViewProjMatrix);
	out_.WorldPosition=mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
	
	return out_;
}
*/

struct SLIGHT_RESULTS {
	
};

SLIGHT_RESULTS computeLith(
							float3 _normal, 
							float3 _halfWayVector, 
							float3 _LampDirection,
							float3 _LampColor
							) {
	float l_lightIntensity = saturate(-dot(_LampDirection,_normal));
	float4 l_lightDiffuse = l_lightIntensity * float4(_LampColor,1.0);
	float4 l_lightSpecular = float4(_LampColor,1.0) * pow(dot(_normal,_halfWayVector),g_SpecularReflection);
	
	//float4 l_lightNonSpecular = saturate(float4(g_LightAmbient,1.0) + l_light0Diffuse);
	
}

float4 mainPSNormalTextured(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	/*float3 l_normal = normalize(_in.WorldNormal);
	float4 l_texColor = tex2D(Tex1Sampler, _in.UV);
	
	float3 l_halfWayVector = normalize(normalize(g_CameraPosition-_in.WorldPosition)-g_Lamp0Direction);
	
	//float4 l_lightColor = clamp(l_light0Color, float4(g_LightAmbient,1.0), 1.0);
	
	return l_texColor * l_light0NonSpecular + l_light0Specular;*/
	return float4(0.0,0.5,0.0,1.0);
}


/*
float4 mainPSNormalTextured2(TNORMAL_TEXTURED_VERTEX_PS _in) : COLOR {
	float4 l_ambient  = tex2D(LightmapSampler , _in.UV);
	float4 l_texColor = tex2D(Tex1Sampler, _in.UV);
	
	return l_texColor * l_ambient;
}
*/

technique NormalTexturedTechnique {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVSNormalTextured();
		PixelShader = compile ps_3_0 mainPSNormalTextured();
	}
}

technique NormalTextured2Technique {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVSNormalTextured();
		PixelShader = compile ps_3_0 mainPSNormalTextured();
	}
}

technique NormalDiffusedTechnique {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVSNormalTextured();
		PixelShader = compile ps_3_0 mainPSNormalTextured();
	}
}
