#if !defined( __NEW_SHADERS_FX__ )
#define __NEW_SHADERS_FX__



#include "Globals.fx"
#include "Functions.fx"

//#define NS_LIGHTING
//#define NS_TEX0
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULARMAP
//#define NS_ENVIRONMENT


//Tangent, radiosity i Specular necessiten tenir les primeres coordenades de textura i la normal (iluminació) per funcionar.

#if defined( NS_RADIOSITY_NORMALMAP )

  #if !defined( NS_NORMALMAP )
    #define NS_NORMALMAP
  #endif

#endif

#if defined( NS_NORMALMAP )

  #if !defined( NS_TEX0 )
    #define NS_TEX0
  #endif

  #if !defined( NS_LIGHTING )
    #define NS_LIGHTING
  #endif

#endif

#if defined( NS_SPECULARMAP )

  #if !defined( NS_TEX0 )
    #define NS_TEX0
  #endif

  #if !defined( NS_LIGHTING )
    #define NS_LIGHTING
  #endif

#endif



struct TNEW_VS
{
	float3 Position : POSITION;
  float3 Normal : NORMAL;
  #if defined( NS_TEX0 )
    float4 UV : TEXCOORD0;
  #else
    float4 Color : COLOR;
  #endif
  #if defined( NS_LIGHTMAP )
    float4 UV2 : TEXCOORD1;
  #endif
  #if defined( NS_NORMALMAP )
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
  #endif
  #if defined( NS_CAL3D )
    float4 Weight : BLENDWEIGHT;
    float4 Indices : BLENDINDICES;
  #endif
};

struct TNEW_PS {
  float4 HPosition : POSITION;
  #if defined( NS_TEX0 )
    float2 UV : TEXCOORD0;
  #else
    float4 Color : COLOR;
  #endif
  #if defined( NS_LIGHTMAP )
    float2 UV2 : TEXCOORD1;
  #endif
  #if defined( NS_NORMALMAP )
    float3 WorldNormal : TEXCOORD3;
    float3 WorldTangent : TEXCOORD4;
    float3 WorldBinormal : TEXCOORD5;
  #else
    float3 ViewNormal : TEXCOORD3;
  #endif
  float4 ViewPosition : TEXCOORD6;
};

struct PS_OUTPUT
{
	float4	Color		: COLOR0;
	float4	Normals : COLOR1;
  float4	Depth   : COLOR2;
};

TNEW_PS NewVS(TNEW_VS _in) 
{
	TNEW_PS out_ = (TNEW_PS)0;
  
  #if defined( NS_CAL3D )
    float3 l_Position  = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
    float3 l_Normal    = CalcAnimtedPos(float4(_in.Normal.xyz,0.0), _in.Indices, _in.Weight);
    float4 l_LocalPosition=float4(-l_Position.x,l_Position.y,l_Position.z, 1.0);
    
    #if defined( NS_NORMALMAP )
      float3 l_Tangent   = CalcAnimtedPos(float4(_in.Tangent.xyz,0.0), _in.Indices, _in.Weight);
      float3 l_Binormal = CalcAnimtedPos(float4(_in.Binormal.xyz,0.0), _in.Indices, _in.Weight);
      
      l_Tangent = float3(l_Tangent.x,-l_Tangent.y,-l_Tangent.z);
      l_Binormal = float3(l_Binormal.x,-l_Binormal.y,-l_Binormal.z);
    #endif
    l_Normal = float3(-l_Normal.x,l_Normal.y,l_Normal.z);
  #else
    float4 l_LocalPosition = float4(_in.Position,1.0);
    float3 l_Normal    = _in.Normal.xyz;
    
    #if defined( NS_NORMALMAP )
      float3 l_Tangent   = _in.Tangent.xyz;
      float3 l_Binormal  = _in.Binormal.xyz;
    #endif
  #endif
  
  #if defined( NS_NORMALMAP )
    out_.WorldNormal   = mul(l_Normal  , (float3x3)g_WorldMatrix);
    out_.WorldTangent  = mul(l_Tangent , (float3x3)g_WorldMatrix);
    out_.WorldBinormal = mul(l_Binormal, (float3x3)g_WorldMatrix);
  #else
    out_.ViewNormal    = mul(l_Normal, (float3x3)g_WorldViewMatrix);
  #endif

  #if defined( NS_TEX0 )
    out_.UV  = _in.UV.xy;
  #else
    out_.Color = _in.Color;
  #endif
  
  #if defined( NS_LIGHTMAP )
    out_.UV2 = _in.UV2.xy;
  #endif

	out_.HPosition = mul(l_LocalPosition, g_WorldViewProjectionMatrix );
  out_.ViewPosition=mul(l_LocalPosition,g_WorldViewMatrix);
  
	return out_;
}






PS_OUTPUT NewPS(TNEW_PS _in)
{
  PS_OUTPUT l_Output;
  
  #if defined( NS_LIGHTMAP )
    bool l_DynamicObject = false;
  #else
    bool l_DynamicObject = true;
  #endif
  
  #if defined( NS_NORMALMAP )
    float4 l_Bump;
    float3 l_WorldNormal = CalcNormalmap((float3)_in.WorldTangent, 
                                         (float3)_in.WorldBinormal, 
                                         (float3)_in.WorldNormal, 
                                         _in.UV, l_Bump);
                                         
    float3 l_ViewNormal = mul(l_WorldNormal, (float3x3)g_ViewMatrix);
  #else
    float3 l_ViewNormal = normalize(_in.ViewNormal);
  #endif

  #if defined( NS_TEX0 )
    float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  #else
    float4 l_DiffuseColor = _in.Color;
  #endif

  
  l_Output.Color = l_DiffuseColor;
  l_Output.Normals = float4(l_ViewNormal.xyz * 0.5 + 0.5, 0.0);
  //l_Output.PosXY = float4(_in.ViewPosition.xy, 0, 0);
	l_Output.Depth = float4(_in.ViewPosition.z, 0, 0, 0);
  
  //#if defined( NS_TEX0 )
  //  if(g_GlowActive)
  //  {
  //    l_Output.Glow = tex2D(GlowTextureSampler,_in.UV) * g_GlowIntensity;
  //    //l_Output.Glow.a = 1.0;
  //  } else {
  //    l_Output.Glow = float4(0, 0, 0, 0);
  //  }
  //#else
  //  l_Output.Glow = float4(0, 0, 0, 0);
  //#endif
  
  
	return l_Output;
}



#if defined( NS_CAL3D )
  #define TECHNIQUE_BODY \
    pass p0 {                                            \
      /*Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar*/ \
      ZEnable = true;                                    \
      ZWriteEnable = true;                               \
      ZFunc = LessEqual;                                 \
      /*Deshabilitamos el alphablend*/                              \
      AlphaBlendEnable = false;                          \
      /*Tipo de culling que queremos utilizar*/                         \
      CullMode = CW;                                    \
      /*Vertex / Pixel shader*/                                  \
      VertexShader = compile vs_3_0 NewVS();             \
      PixelShader  = compile ps_3_0 NewPS();             \
    }
#else
  #define TECHNIQUE_BODY \
    pass p0 {                                            \
      /*Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar*/ \
      ZEnable = true;                                    \
      ZWriteEnable = true;                               \
      ZFunc = LessEqual;                                 \
      /*Deshabilitamos el alphablend*/                              \
      AlphaBlendEnable = false;                          \
      /*Tipo de culling que queremos utilizar*/                         \
      CullMode = CCW;                                    \
      /*Vertex / Pixel shader*/                                  \
      VertexShader = compile vs_3_0 NewVS();             \
      PixelShader  = compile ps_3_0 NewPS();             \
    }
#endif
    
#define ALPHA_TECHNIQUE_BODY \
  pass p0 {                                               \
		/*Activamos el Zbuffer, el Zwrite y la funciÃ³n de Zâ€™s que queremos utilizar*/ \
    ZEnable = true;                                       \
    ZWriteEnable = false;                                 \
    ZFunc = LessEqual;                                    \
    /*Deshabilitamos el alphablend*/                                 \
    AlphaBlendEnable = true;                              \
    SrcBlend = SrcAlpha;                                  \
    DestBlend = InvSrcAlpha;                              \
    /*Tipo de culling que queremos utilizar*/                           \
    CullMode = CW;                                        \
    /*Vertex / Pixel shader*/                                     \
		VertexShader = compile vs_3_0 NewVS();                \
		PixelShader  = compile ps_3_0 NewPS();                \
	}                                                       \
	pass p1 {                                               \
		/*Activamos el Zbuffer, el Zwrite y la funciÃ³n de Zâ€™s que queremos utilizar*/ \
    ZEnable = true;                                       \
    ZWriteEnable = false;                                 \
    ZFunc = LessEqual;                                    \
    /*Deshabilitamos el alphablend*/                                 \
    AlphaBlendEnable = true;                              \
    SrcBlend = SrcAlpha;                                  \
    DestBlend = InvSrcAlpha;                              \
    /*Tipo de culling que queremos utilizar*/                           \
    CullMode = CCW;                                       \
    /*Vertex / Pixel shader*/                                     \
		VertexShader = compile vs_3_0 NewVS();                \
		PixelShader  = compile ps_3_0 NewPS();                \
	}
  
#endif
