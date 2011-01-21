// Normal Diffused --------------------------------------------------------------------------------------------------------------

struct TNORMAL_DIFFUSED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct TNORMAL_DIFFUSED_VERTEX_PS {
	float4 HPosition : POSITION;
	float4 Color : COLOR;
	float3 WorldNormal : TEXCOORD0;
  float3 WorldPosition : TEXCOORD1;
};
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
  float3 WorldPosition : TEXCOORD2;
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
  float3 WorldPosition : TEXCOORD3;
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
  float3 WorldPosition : TEXCOORD2;
  float3 WorldTangent : TEXCOORD3;
  float3 WorldBinormal : TEXCOORD4;
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
  float3 WorldPosition : TEXCOORD3;
  float3 WorldTangent : TEXCOORD4;
  float3 WorldBinormal : TEXCOORD5;
};

// Cal3d --------------------------------------------------------------------------------------------------------------------------

struct CAL3D_HW_VERTEX_VS {
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float4 Normal : NORMAL;
    //float4 Tangent : TANGENT0;
	//float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct CAL3D_HW_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	//float3 WorldTangent : TEXCOORD3;
	//float3 WorldBinormal : TEXCOORD4;
};