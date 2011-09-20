#ifndef __GLOBALS_FX__
#define __GLOBALS_FX__


#define MAXBONES 29
#define MAXLIGHTS 4
#define SQRT2     1.414213562373095

// Matrixes
shared float4x4 g_WorldMatrix : World; 
shared float4x4 g_ViewMatrix : View;
shared float4x4 g_ProjectionMatrix : Projection; 
shared float4x4 g_InvProjectionMatrix : InvProjection; 
shared float4x4 g_WorldViewMatrix : WorldView; 
shared float4x4 g_ViewProjectionMatrix : ViewProjection; 
shared float4x4 g_WorldViewProjectionMatrix : WorldViewProjection; 
shared float4x4 g_ViewToLightProjectionMatrix : ViewToLightProjection;
shared float3x4 g_Bones[MAXBONES] : Bones;

shared float g_Bump : BumpAmount; // = 0.6;
float g_BumpMaxHeight =  0.7;
float g_BumpMinHeight = -0.7;
float g_BumpDiff = 0.6;

float g_ParallaxHeight = 0.02f;

shared float g_SpecularPow     : Glossiness;    // = 100;
shared float g_SpotlightFactor : SpecularLevel; //= 0.1;
float g_SpotlightFactorMax = 10;
float g_SpecularPowMax = 256;

shared float g_EnvironmentIntensity : EnvironmentIntensity;

float g_GlowBlurIntensity = 1.15;
shared float g_GlowIntensity : GlowIntensity;
shared float2 g_SpriteSize : SpriteSize;

//Lights
#define LIGHT_OMNI        0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT        2

//     X/red                       
//     0.908248                 -> 232.512
//     0.5                           -> 128
//     0.788675                 -> 201.9
//                                     
//     Y/green                    
//     0.295876                 -> 75.744
//     0.853555                 -> 218.509
//     0.788675                 -> 201.9
//                                     
//     Z/blue                      
//     0.295876                 -> 75.744
//     0.146446                 -> 37.49
//     0.788675                 -> 201.9


float3 g_RadiosityNormalR = { sqrt(2/3) ,  0        , 1/sqrt(3) };
float3 g_RadiosityNormalG = { -1/sqrt(6),  1/sqrt(2), 1/sqrt(3) };
float3 g_RadiosityNormalB = { -1/sqrt(6), -1/sqrt(2), 1/sqrt(3) };



//Shadows
#define SHADOW_EPSILON 0.0005f


shared float3  g_AmbientLight                 : AmbientLight;

shared bool    g_LightsEnabled[MAXLIGHTS]     : LightsEnabled;
shared int     g_LightsType[MAXLIGHTS]        : LightsType;
shared float3  g_LightsPosition[MAXLIGHTS]    : LightsPosition;
shared float3  g_LightsDirection[MAXLIGHTS]   : LightsDirection;
shared float4  g_LightsColor[MAXLIGHTS]       : LightsColor;

shared float   g_LightsAngleCos[MAXLIGHTS]    : LightsAngleCos;
shared float   g_LightsFallOffCos[MAXLIGHTS]  : LightsFallOffCos;

shared float   g_LightsStartRangeSQ[MAXLIGHTS]: LightsStartRangeSQ;
shared float   g_LightsEndRangeSQ[MAXLIGHTS]  : LightsEndRangeSQ;

shared bool    g_ShadowEnabled[MAXLIGHTS]     : ShadowEnabled;
shared bool    g_DynamicObjectsOnly[MAXLIGHTS]: DynamicObjectsOnly;

//Deferred

shared bool    g_LightEnabled       : LightEnabled;
shared int     g_LightType          : LightType;
shared float3  g_LightPosition      : LightPosition;
shared float3  g_LightDirection     : LightDirection;
shared float4  g_LightColor         : LightColor;
shared float   g_LightAngleCos      : LightAngleCos;
shared float   g_LightFallOffCos    : LightFallOffCos;
shared float   g_LightStartRangeSQ  : LightStartRangeSQ;
shared float   g_LightEndRangeSQ    : LightEndRangeSQ;
shared bool    g_LightShadowEnabled : LightShadowEnabled;
shared bool    g_LightDynamicOnly   : LightDynamicObjectsOnly;

//Other
shared float3 g_CameraUp       : CameraUp;
shared float3 g_CameraRight    : CameraRight;
shared float3 g_CameraPosition : CameraPosition;
shared float g_Time : Time;
shared float g_GlowActive : GlowActive;
shared float g_SpecularActive : SpecularActive;

shared int g_TextureWidth  : TextureWidth;
shared int g_TextureHeight : TextureHeight;
shared float g_AlphaFactor : AlphaFactor;

#define POISON_BLUR_KERNEL_SIZE 16

shared float2 g_PoissonBlurKernel[POISON_BLUR_KERNEL_SIZE] : PoissonBlurKernel;

#endif