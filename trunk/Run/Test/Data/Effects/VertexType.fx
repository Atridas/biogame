

// Normal Textured --------------------------------------------------------------------------------------------------------------

struct TNORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
};

struct TNORMAL_TEXTURED_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
};

// Normal Textured 2 --------------------------------------------------------------------------------------------------------------

struct TNORMAL_TEXTURED2_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
};

struct TNORMAL_TEXTURED2_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
};

// Normal Textured + Binormal / Tangent------------------------------------------------------------------------------------------

struct TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
  float3 Tangent : TANGENT;
  float3 Binormal : BINORMAL;
};

struct TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
  float3 WorldTangent : TEXCOORD2;
  float3 WorldBinormal : TEXCOORD3;
};

// Normal Textured2+ Binormal / Tangent------------------------------------------------------------------------------------------

struct TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
  float3 Tangent : TANGENT;
  float3 Binormal : BINORMAL;
};

struct TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
  float3 WorldTangent : TEXCOORD3;
  float3 WorldBinormal : TEXCOORD4;
};

// Cal3d --------------------------------------------------------------------------------------------------------------------------

struct appdata
{
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};
