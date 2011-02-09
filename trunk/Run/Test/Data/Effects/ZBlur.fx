#include "Globals.fx"
#include "Functions.fx"
#include "VertexType.fx"

// ZBlur
// vDofParams coefficients:
// x = near blur depth; y = focal plane depth; z = far blur depth
// w = blurriness cutoff constant for objects behind the focal plane
float4 vDofParams=float4(1.0,3.0,20.0, 0.75);


//Vertex Shader
void PreZBlurVS(float4 _Pos : POSITION,
                out float4 Pos_ : POSITION,
                out float Depth_ : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
  Pos_ = mul( _Pos, g_WorldViewProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth_ = Pos_.z;
}


void AnimatedPreZBlurVS(CAL3D_HW_VERTEX_VS _in,
                out float4 Pos_ : POSITION,
                out float Depth_ : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
	float3 l_Position = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
  Pos_ = mul( float4(l_Position,1.0), g_WorldViewProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth_ = Pos_.z;
}

float ComputeDepthBlur(float depth /* in view space */)
{
  float f;
  if(depth < vDofParams.y) //y ==> Planol focal
  {
    //scale depth value between near blur distance and focal distance to [-1,0] range
    f=(depth-vDofParams.y)/(vDofParams.y-vDofParams.x);
  }
  else
  {
    //scale depth value between foca distance and far blur distance to [0,1] range
    f=(depth-vDofParams.y)/(vDofParams.z-vDofParams.y);
    f=clamp(f,0,vDofParams.w);
  }
  //scale and bias into [0, 1] range
  return f*0.5f + 0.5f;
}


float4 PreZBlurPS(float4 _Pos : POSITION,  float2 _Depth : TEXCOORD0 ) : COLOR
{
  float l_Depth=ComputeDepthBlur(_Depth.x / _Depth.y);
  return float4(l_Depth,l_Depth,l_Depth,1.0);
}

technique PreZBlurTechnique
{
  pass p0
  {
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    AlphaBlendEnable = false;
    CullMode = CCW;
    VertexShader = compile vs_3_0 PreZBlurVS();
    PixelShader = compile ps_3_0 PreZBlurPS();
  }
}

technique AnimatedPreZBlurTechnique
{
  pass p0
  {
    ZEnable = true;
    ZWriteEnable = true;
    ZFunc = LessEqual;
    AlphaBlendEnable = false;
    CullMode = CCW;
    VertexShader = compile vs_3_0 AnimatedPreZBlurVS();
    PixelShader = compile ps_3_0 PreZBlurPS();
  }
}

// Post Process --------------------------------------------------------------------------------------------------------------------------------------------------------------


sampler FrameBufferSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};
sampler ZBlurSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

#define NUM_SAMPLES 12

float2 TwelveKernelBase[] = {
                              { 1,  0},
                              {-1,  0},
                              { 0,  1},
                              { 0, -1},
                              { 0.8660254,  0.5},
                              { 0.8660254, -0.5},
                              {-0.8660254,  0.5},
                              {-0.8660254, -0.5},
                              { 0.5,  0.8660254},
                              { 0.5, -0.8660254},
                              {-0.5,  0.8660254},
                              {-0.5, -0.8660254},
                            };


#define ZBLUR_WIDTH  1024
#define ZBLUR_HEIGHT 1024

//-----------------------------------------------------------------------------
// Name: DepthOfFieldManySamples
// Type: Pixel shader
// Desc: This post process pixel shader uses an array of values to offset into the
// render-target texture to blur the image based on the value of alpha channel.
//-----------------------------------------------------------------------------
float4 DepthOfFieldManySamples(float2 OriginalUV : TEXCOORD0) : COLOR
{
  float2 l_FrameBubberMultiplier = 5*float2(1 / g_TextureWidth, 1 / g_TextureHeight);
  float2 l_ZBlurMultiplier = 5*float2(1 / ZBLUR_WIDTH, 1 / ZBLUR_HEIGHT);

  float3 OriginalRGB = tex2D(FrameBufferSampler, OriginalUV).xyz;
  float OriginalA = tex2D(ZBlurSampler, OriginalUV).x;
  //return float4(OriginalA,OriginalA,OriginalA,1.0);
  float3 Blurred = 0;
  for(int i = 0; i < NUM_SAMPLES; i++)
  {
    // Lookup into the rendertarget based by offsetting the
    // original UV by KernelArray[i]. See the TwelveKernelBase[] above
    // and UpdateTechniqueSpecificVariables() for how this array is created
    float3 CurrentRGB = tex2D(FrameBufferSampler, OriginalUV + TwelveKernelBase[i] * l_FrameBubberMultiplier).xyz;
    float  CurrentA   = tex2D(ZBlurSampler,       OriginalUV + TwelveKernelBase[i] * l_ZBlurMultiplier).x;
    // Lerp between original rgb and the jitter rgb based on the alpha value
    Blurred += lerp(OriginalRGB, CurrentRGB, saturate(OriginalA*CurrentA));
  }
  return float4(Blurred / NUM_SAMPLES, 1.0f);
}

technique PostZBlurTechnique
{
  pass P0
  {
    PixelShader = compile ps_3_0 DepthOfFieldManySamples(/*TwelveKernel, 12*/);
  }
}

