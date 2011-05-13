#ifndef __GLOBALS_FX__
#define __GLOBALS_FX__


#define MAXBONES 29
#define MAXLIGHTS 4

// Matrixes
shared float4x4 g_WorldMatrix : World; 
shared float4x4 g_ViewMatrix : View;
shared float4x4 g_ProjectionMatrix : Projection; 
shared float4x4 g_WorldViewMatrix : WorldView; 
shared float4x4 g_ViewProjectionMatrix : ViewProjection; 
shared float4x4 g_WorldViewProjectionMatrix : WorldViewProjection; 
shared float4x4 g_ViewToLightProjectionMatrix : ViewToLightProjection;
shared float3x4 g_Bones[MAXBONES] : Bones;

float g_Bump = 2.4;
float g_BumpMaxHeight =  0.7;
float g_BumpMinHeight = -0.7;
float g_BumpDiff = 0.6;

float g_ParallaxHeight = 0.02f;

float g_SpecularPow = 100;
float g_SpotlightFactor = 0.1;

//Lights
#define LIGHT_OMNI        0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT        2

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

//Other
shared float3 g_CameraPosition : CameraPosition;
shared float g_Time : Time;
shared float g_GlowActive : GlowActive;

shared int g_TextureWidth  : TextureWidth;
shared int g_TextureHeight : TextureHeight;

#define POISON_BLUR_KERNEL_SIZE 16

shared float2 g_PoissonBlurKernel[POISON_BLUR_KERNEL_SIZE] : PoissonBlurKernel;

#endif