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
//#define NS_SPECULAR
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

#if defined( NS_SPECULAR )

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
  #if defined( NS_LIGHTING )
    float3 Normal : NORMAL;
  #endif
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
  #if defined( NS_LIGHTING )
    float3 WorldPosition : TEXCOORD2;
    float3 WorldNormal : TEXCOORD3;
    float4 PosLight      : TEXCOORD6;
  #endif
  #if defined( NS_NORMALMAP )
    float3 WorldTangent : TEXCOORD4;
    float3 WorldBinormal : TEXCOORD5;
  #endif
};



TNEW_PS NewVS(TNEW_VS _in) 
{
	TNEW_PS out_ = (TNEW_PS)0;
  
  #if defined( NS_CAL3D )
    float3 l_Position  = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
    float4 l_LocalPosition=float4(-l_Position.x,l_Position.y,l_Position.z, 1.0);
  #endif
	
  #if defined( NS_LIGHTING )
    #if defined( NS_CAL3D )
      float3 l_Normal    = CalcAnimtedPos(float4(IN.Normal.xyz,0.0), IN.Indices, IN.Weight);
      float3 l_Tangent   = CalcAnimtedPos(float4(IN.Tangent.xyz,0.0), IN.Indices, IN.Weight);
      float3 l_Bitangent = CalcAnimtedPos(float4(IN.Bitangent.xyz,0.0), IN.Indices, IN.Weight);
      
      out_.WorldPosition=mul(l_LocalPosition,g_WorldMatrix);  
      out_.WorldNormal = mul(float4(-l_Normal.x,l_Normal.y,l_Normal.z,0.0),g_WorldMatrix);
      out_.WorldTangent = mul(float4(l_Tangent.x,-l_Tangent.y,-l_Tangent.z,0.0),g_WorldMatrix);
      out_.WorldBinormal = mul(float4(l_Bitangent.x,-l_Bitangent.y,-l_Bitangent.z,0.0),g_WorldMatrix);

      float4 l_ViewPosition = mul(float4(l_LocalPosition,1.0),g_WorldViewMatrix);
    #else
      out_.WorldNormal   = mul(_in.Normal,(float3x3)g_WorldMatrix);
      out_.WorldPosition = mul(float4(_in.Position,1.0),g_WorldMatrix).xyz;
      float4 l_ViewPosition = mul(float4(_in.Position,1.0),g_WorldViewMatrix);
    #endif
    out_.PosLight = mul(l_ViewPosition, g_ViewToLightProjectionMatrix);
  #endif
  
  #if defined( NS_NORMALMAP )
    out_.WorldTangent  = mul(_in.Tangent,(float3x3)g_WorldMatrix);
    out_.WorldBinormal = mul(_in.Binormal,(float3x3)g_WorldMatrix);
  #endif

  #if defined( NS_TEX0 )
    out_.UV  = _in.UV.xy;
  #else
    out_.Color = _in.Color;
  #endif
  
  #if defined( NS_LIGHTMAP )
    out_.UV2 = _in.UV2.xy;
  #endif

  #if defined( NS_CAL3D )
    out_.HPosition = mul(l_LocalPosition, g_WorldViewProjectionMatrix );
  #else
    out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
  #endif
	
	return out_;
}

float4 NewPS(TNEW_PS _in) : COLOR {

  #if defined( NS_LIGHTMAP )
    bool l_DynamicObject = false;
  #else
    bool l_DynamicObject = true;
  #endif
  
  #if defined( NS_NORMALMAP )
    float4 l_Bump;
    float3 l_Normal = CalcNormalmap((float3)_in.WorldTangent, 
                                    (float3)_in.WorldBinormal, 
                                    (float3)_in.WorldNormal, 
                                    _in.UV, l_Bump);
  #else
    #if defined( NS_LIGHTING )
      float3 l_Normal = normalize(_in.WorldNormal);
    #endif
  #endif

  #if defined( NS_TEX0 )
    float4 l_DiffuseColor = tex2D(DiffuseTextureSampler,_in.UV);
  #else
    float4 l_DiffuseColor = _in.Color;
  #endif


  #if defined( NS_LIGHTING )
  
    #if defined( NS_LIGHTMAP )
      #if defined( NS_RADIOSITY_NORMALMAP )
        float4 l_AmbientColor = RadiosityNormalLightmapColor(l_Bump, _in.UV2);
      #else
        float4 l_AmbientColor = tex2D(LightmapTextureSampler,_in.UV2);
      #endif
    #else
      float4 l_AmbientColor = float4(g_AmbientLight,1.0);
    #endif
    
    #if defined( NS_SPECULAR )
      float  l_SpotlightFactor = 1.0;
      
      if(g_SpecularActive)
        l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
    #else
      float  l_SpotlightFactor = g_SpotlightFactor;
    #endif

    float3 l_EyeDirection = normalize(g_CameraPosition - _in.WorldPosition);
    l_DiffuseColor = float4(ComputeAllLightsNew( l_Normal, _in.WorldPosition, l_EyeDirection,
                                              l_DiffuseColor, l_AmbientColor, l_SpotlightFactor,
                                              _in.PosLight,l_DynamicObject)
                                             ,l_DiffuseColor.a);    
          
    #if defined ( NS_ENVIRONMENT )
      float3 l_ReflectionVector = normalize(reflect(-l_EyeDirection, l_Normal));
      float4 l_EnvColor = texCUBE(EnvironmentTextureSampler, l_ReflectionVector);
      l_DiffuseColor = l_EnvColor;
    #endif
                                             
  #endif
  
	return l_DiffuseColor;
}




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
