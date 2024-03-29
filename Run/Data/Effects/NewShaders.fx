#if !defined( __NEW_SHADERS_FX__ )
#define __NEW_SHADERS_FX__



#include "Globals.fx"
#include "Functions.fx"

//#define NS_LIGHTING
//#define NS_TEX0
//#define NS_COLOR
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULARMAP
//#define NS_ENVIRONMENT
//#define NS_WHITE


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
  #if defined( NS_LIGHTING )
    float3 Normal : NORMAL;
  #endif
  #if defined( NS_TEX0 )
    float4 UV : TEXCOORD0;
  #endif
  #if defined( NS_COLOR )
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
  #endif
  #if defined( NS_COLOR )
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
  //float4 ViewPosition : TEXCOORD7;
};

struct PS_OUTPUT
{
	float4	Color		  : COLOR0;
	float4	Glow    	: COLOR1;
};

TNEW_PS NewVS(TNEW_VS _in) 
{
	TNEW_PS out_ = (TNEW_PS)0;
  
  #if defined( NS_CAL3D )
    float3 l_Position  = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
    float3 l_Normal    = CalcAnimtedPos(float4(_in.Normal.xyz,0.0), _in.Indices, _in.Weight);
    float4 l_LocalPosition=float4(-l_Position.x,l_Position.y,l_Position.z, 1.0);
  #endif
	
  #if defined( NS_LIGHTING )
    #if defined( NS_CAL3D )
      #if defined( NS_NORMALMAP )
        float3 l_Tangent   = CalcAnimtedPos(float4(_in.Tangent.xyz,0.0), _in.Indices, _in.Weight);
        float3 l_Binormal = CalcAnimtedPos(float4(_in.Binormal.xyz,0.0), _in.Indices, _in.Weight);
        out_.WorldTangent = mul(float4(l_Tangent.x,-l_Tangent.y,-l_Tangent.z,0.0),g_WorldMatrix);
        out_.WorldBinormal = mul(float4(l_Binormal.x,-l_Binormal.y,-l_Binormal.z,0.0),g_WorldMatrix);
      #endif
      
      out_.WorldPosition=mul(l_LocalPosition,g_WorldMatrix);  
      out_.WorldNormal = mul(float4(-l_Normal.x,l_Normal.y,l_Normal.z,0.0),g_WorldMatrix);
      
      float4 l_ViewPosition = mul(float4(l_LocalPosition.xyz,1.0),g_WorldViewMatrix);
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
  #endif
  #if defined( NS_COLOR )
    out_.Color = _in.Color;
  #endif
  
  #if defined( NS_LIGHTMAP )
    out_.UV2 = _in.UV2.xy;
  #endif

  #if defined( NS_CAL3D )
    out_.HPosition = mul(l_LocalPosition, g_WorldViewProjectionMatrix );
    //out_.ViewPosition = mul(l_LocalPosition, g_WorldViewMatrix );
  #else
    out_.HPosition = mul(float4(_in.Position,1.0),g_WorldViewProjectionMatrix);
    //out_.ViewPosition = mul(float4(_in.Position,1.0), g_WorldViewMatrix );
  #endif
	
	return out_;
}

PS_OUTPUT NewPS(TNEW_PS _in, float _fFace : VFACE)
{
  PS_OUTPUT l_Output = (PS_OUTPUT)0;
  
  
  #if defined( NS_WHITE )
      l_Output.Color = float4(1, 1, 1, 1);
      //l_Output.DepthGlow.x = _in.ViewPosition.z;
  #else
  
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
      //l_Output.Color = float4(l_DiffuseColor.a, l_DiffuseColor.a, l_DiffuseColor.a, 1.0);
      //l_Output.Glow = float4(0,0,0,0);
      //return l_Output;
      #if defined( NS_COLOR )
        l_DiffuseColor *= _in.Color;
      #endif
    #else
      #if defined( NS_COLOR )
        float4 l_DiffuseColor = _in.Color;
      #else
        float4 l_DiffuseColor = float4(0, 0, 0, 0);
      #endif
    #endif
    
    l_Output.Color.a = l_DiffuseColor.a;

    #if defined( NS_LIGHTING )
    
      #if defined( NS_LIGHTMAP )
        #if defined( NS_RADIOSITY_NORMALMAP )
          float4 l_AmbientColor = RadiosityNormalLightmapColor(l_Bump, _in.UV2) * 2.0;
        #else
          float4 l_AmbientColor = tex2D(LightmapTextureSampler,_in.UV2) * 2.0;
        #endif
      #else
        float4 l_AmbientColor = float4(g_AmbientLight,1.0);
      #endif
      
      
      
      #if defined( NS_SPECULARMAP )
        float  l_SpotlightFactor = 1.0;
        
        #if defined ( NS_ENVIRONMENT )
          float l_SpecularTextureValue = tex2D(SpecularTextureSampler,_in.UV).x;
          if(g_SpecularActive)
            l_SpotlightFactor;
        #else
          if(g_SpecularActive)
            l_SpotlightFactor = tex2D(SpecularTextureSampler,_in.UV).x;
        #endif
      #else
        #if defined ( NS_ENVIRONMENT )
          float l_SpecularTextureValue = 1.0;
        #endif 
        float  l_SpotlightFactor = 1.0;
      #endif

      float3 l_EyeDirection = normalize(g_CameraPosition - _in.WorldPosition);
      l_DiffuseColor = float4(ComputeAllLightsNew( l_Normal, _in.WorldPosition, l_EyeDirection,
                                                l_DiffuseColor, l_AmbientColor, l_SpotlightFactor * g_SpotlightFactor,
                                                _in.PosLight,l_DynamicObject)
                                               ,l_DiffuseColor.a);    
            
      #if defined ( NS_ENVIRONMENT )
        float3 l_ReflectionVector = normalize(reflect(-l_EyeDirection, l_Normal));
        float4 l_EnvColor = texCUBE(EnvironmentTextureSampler, l_ReflectionVector);
        //l_DiffuseColor += l_SpotlightFactor * l_EnvColor;
        //l_DiffuseColor = float4(l_SpotlightFactor, l_SpotlightFactor, l_SpotlightFactor, 1);
        l_DiffuseColor.rgb += g_EnvironmentIntensity * l_SpecularTextureValue * l_EnvColor.rgb;
      #endif
                                               
    #endif
    
    l_Output.Color.rgb = l_DiffuseColor.rgb;
    //l_Output.DepthGlow.x = _in.ViewPosition.z;
    //l_Output.DepthGlow.w = 1;
    //
    //#if defined( NS_TEX0 )
    //  if(g_GlowActive)
    //  {
    //    //l_Output.Glow  = tex2D(GlowTextureSampler,_in.UV);
    //    //l_Output.Glow *= g_GlowIntensity * l_Output.Glow.w;
    //    //l_Output.Glow = max(0.0, l_Output.Glow);
    //    
    //    l_Output.DepthGlow.y = tex2D(GlowTextureSampler,_in.UV).r * g_GlowIntensity;
    //    l_Output.DepthGlow.y = max(0.0, l_Output.DepthGlow.y);
    //    
    //  }
    //  else
    //  {
    //    l_Output.DepthGlow.y = 0.0;
    //  }
    //#else
    //  l_Output.DepthGlow.y = 0.0;
    //#endif
    
    #if defined( NS_TEX0 )
      if(g_GlowActive && _fFace > 0)
      {
        float l_fGlowIntensity = tex2D(GlowTextureSampler,_in.UV).r * g_GlowIntensity;
        l_Output.Glow = l_DiffuseColor * l_fGlowIntensity;
        #if defined( NS_COLOR )
          l_Output.Glow *= _in.Color;
        #endif
        l_Output.Glow = max(0.0, l_Output.Glow);
        l_Output.Glow.a = l_DiffuseColor.a;
      } else {
        l_Output.Glow = float4(0, 0, 0, 0);
      }
    #else
      l_Output.Glow = float4(0, 0, 0, 0);
    #endif
  #endif
  
  l_Output.Color.rgb = saturate(l_Output.Color.rgb);
  //l_Output.Color.a = 0.5;
  
	return l_Output;
}

float4 caca() : COLOR0
{
  return float4(1,0,1,0);
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
    StencilEnable = false;                                \
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
		/*PixelShader  = compile ps_3_0 caca();*/                \
	}                                                       \
	pass p1 {                                               \
		/*Activamos el Zbuffer, el Zwrite y la funciÃ³n de Zâ€™s que queremos utilizar*/ \
    StencilEnable = false;                                \
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
		/*PixelShader  = compile ps_3_0 caca();*/                \
	}
  
#endif
