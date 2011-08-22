#ifndef __VERTEX_TYPE_FX__
#define __VERTEX_TYPE_FX__

// minimalista -------------------------------------------------------------------------------------------------------------------

struct TSIMPLE_VS 
{
	float3 Position : POSITION;
};

struct TSIMPLE_INSTANCED_VS 
{
	float3 Position : POSITION;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

// Normal Diffused --------------------------------------------------------------------------------------------------------------

struct TNORMAL_DIFFUSED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct TNORMAL_DIFFUSED_VERTEX_INSTANCED_VS {
	float3 Position : POSITION0;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
  
  float4 WorldMatrix0 : POSITION1;
  float4 WorldMatrix1 : POSITION2;
  float4 WorldMatrix2 : POSITION3;
  float4 WorldMatrix3 : POSITION4;
};

struct TNORMAL_DIFFUSED_VERTEX_PS {
	float4 HPosition : POSITION;
	float4 Color : COLOR;
  float3 WorldPosition : TEXCOORD2;
	float3 WorldNormal : TEXCOORD3;
  float4 PosLight      : TEXCOORD4;
};


// Textured --------------------------------------------------------------------------------------------------------------

struct TTEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float4 UV : TEXCOORD0;
};

struct TTEXTURED_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float4 UV : TEXCOORD0;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TTEXTURED_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
};
// Textured Difussed--------------------------------------------------------------------------------------------------------------

struct TTEXTURED_DIFFUSSED_VERTEX_VS {
	float3 Position : POSITION;
	float4 Color : COLOR;
	float4 UV : TEXCOORD0;
};

struct TTEXTURED_DIFFUSSED_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float4 Color : COLOR;
	float4 UV : TEXCOORD0;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TTEXTURED_DIFFUSSED_VERTEX_PS {
	float4 HPosition : POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD0;
};
// Normal Textured --------------------------------------------------------------------------------------------------------------

struct TNORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
};

struct TNORMAL_TEXTURED_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TNORMAL_TEXTURED_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
  float3 WorldPosition : TEXCOORD2;
	float3 WorldNormal : TEXCOORD3;
  float4 PosLight      : TEXCOORD4;
};

// Normal Textured 2 --------------------------------------------------------------------------------------------------------------

struct TNORMAL_TEXTURED2_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
};

struct TNORMAL_TEXTURED2_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TNORMAL_TEXTURED2_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
  float3 WorldPosition : TEXCOORD2;
	float3 WorldNormal : TEXCOORD3;
  float4 PosLight      : TEXCOORD4;
};

// Normal Textured + Binormal / Tangent------------------------------------------------------------------------------------------

struct TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
  float3 Tangent : TANGENT;
  float3 Binormal : BINORMAL;
};

struct TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
  float3 Tangent : TANGENT;
  float3 Binormal : BINORMAL;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TTANGENT_BINORMAL_NORMAL_TEXTURED_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
  float3 WorldPosition : TEXCOORD2;
	float3 WorldNormal : TEXCOORD3;
  float3 WorldTangent : TEXCOORD4;
  float3 WorldBinormal : TEXCOORD5;
  float4 PosLight      : TEXCOORD6;
};

struct TTANGENT_SPACE_TEXTURED_PS {
  float4 HPosition : POSITION;
  float2 UV : TEXCOORD0;
  
  float4 PosLight      : TEXCOORD6;
  
  float3 TangentSpacePosition : TEXCOORD7;
  float3 TangentSpaceCamera : TEXCOORD8;
  
  float3 LightPosition0:  TEXCOORD9;
  float3 LightDirection0: TEXCOORD10;
  float3 LightPosition1:  TEXCOORD11;
  float3 LightDirection1: TEXCOORD12;
  float3 LightPosition2:  TEXCOORD13;
  float3 LightDirection2: TEXCOORD14;
  float3 LightPosition3:  TEXCOORD15;
  float3 LightDirection3: TEXCOORD16;
  
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

struct TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_INSTANCED_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
  float3 Tangent : TANGENT;
  float3 Binormal : BINORMAL;
  
  float4 WorldMatrix0 : TEXCOORD2;
  float4 WorldMatrix1 : TEXCOORD3;
  float4 WorldMatrix2 : TEXCOORD4;
  float4 WorldMatrix3 : TEXCOORD5;
};

struct TTANGENT_BINORMAL_NORMAL_TEXTURED2_VERTEX_PS {
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
  float3 WorldPosition : TEXCOORD2;
	float3 WorldNormal : TEXCOORD3;
  float3 WorldTangent : TEXCOORD4;
  float3 WorldBinormal : TEXCOORD5;
  float4 PosLight      : TEXCOORD6;
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

struct CAL3D_TANGENT_BITANGENT_HW_VERTEX_VS {
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float4 Normal : NORMAL;
  float4 Tangent : TANGENT0;
	float4 Bitangent : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

// Partícules --------------------------------------------------------------------------------------------------------------------------

struct PARTICLE_VS {
	float3 Position : POSITION;
	float4 Parameters1 : TEXCOORD0; // x, y, z, diapo
	float4 Parameters2 : TEXCOORD1; // angleSin, angleCos, sizeX, sizeY
	float4 Color : COLOR;
};

#endif