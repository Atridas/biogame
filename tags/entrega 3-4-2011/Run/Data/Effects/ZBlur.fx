#include "Globals.fx"
#include "Functions.fx"
#include "VertexType.fx"

// ZBlur
// vDofParams coefficients:
// x = near blur depth; y = focal plane depth; z = far blur depth
// w = blurriness cutoff constant for objects behind the focal plane
float4 vDofParams=float4(2.0,5.0,45.0, 0.75);


//Vertex Shader
void PreZBlurVS(float4 _Pos : POSITION,
                out float4 Pos_ : POSITION,
                out float Depth_ : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
  float4 l_ViewPos = mul( _Pos, g_WorldViewMatrix );
  Pos_ = mul( l_ViewPos, g_ProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth_ = l_ViewPos.z / l_ViewPos.w;
}


void AnimatedPreZBlurVS(CAL3D_HW_VERTEX_VS _in,
                out float4 Pos_ : POSITION,
                out float Depth_ : TEXCOORD0 )
{
  //
  // Compute the projected coordinates
  //
	float3 l_Position = CalcAnimtedPos(float4(_in.Position.xyz,1.0), _in.Indices, _in.Weight);
  float4 l_ViewPos = mul( float4(-l_Position.x,l_Position.y,l_Position.z,1.0), g_WorldViewMatrix );
  Pos_ = mul( l_ViewPos, g_ProjectionMatrix );
  //
  // Store z and w in our spare texcoord
  //
  Depth_ = Pos_.z;
}

float ComputeDepthBlur(float depth /* in view space */)
{
/*
  float f;
  if(depth < vDofParams.y)
  {
    f = (depth - vDofParams.y) / (vDofParams.y - vDofParams.x);
  } else {
    f = (depth - vDofParams.y) / (vDofParams.z - vDofParams.y);
  
    f = clamp(f, 0, vDofParams.w);
  }
  
  return f * 0.5 + 0.5;
  */
  
  float f;
  if(depth < vDofParams.x)
  {
    f = 1.0;
  } else if(depth <= vDofParams.y)
  {
    f = ((-depth)/(vDofParams.y-vDofParams.x)) + vDofParams.x/(vDofParams.y-vDofParams.x) + 1;
  } else if(depth <= vDofParams.z)
  {
    f = (depth * vDofParams.w / (vDofParams.z - vDofParams.y)) - (vDofParams.y*vDofParams.w/(vDofParams.z-vDofParams.y));
  } else 
  {
    f = vDofParams.w;
  }
  return f;
}


float4 PreZBlurPS(float4 _Pos : POSITION,  float _Depth : TEXCOORD0 ) : COLOR
{
  float l_Blur=ComputeDepthBlur(_Depth);
  return float4(l_Blur,l_Blur,l_Blur,1.0);
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

/*
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
*/


//float2 g_PoissonBlurKernel[POISON_BLUR_KERNEL_SIZE]

#define ZBLUR_WIDTH  1024.0
#define ZBLUR_HEIGHT 1024.0

#define MAX_RADI_BLUR 5.0

//-----------------------------------------------------------------------------
// Name: DepthOfFieldManySamples
// Type: Pixel shader
// Desc: This post process pixel shader uses an array of values to offset into the
// render-target texture to blur the image based on the value of alpha channel.
//-----------------------------------------------------------------------------
float4 DepthOfFieldManySamples(float2 OriginalUV : TEXCOORD0) : COLOR
{
  float2 l_FrameBubberDivisor = float2(g_TextureWidth, g_TextureHeight);
  float2 l_ZBlurDivisor = float2(ZBLUR_WIDTH, ZBLUR_HEIGHT);

  float3 OriginalRGB = tex2D(FrameBufferSampler, OriginalUV).xyz;
  float OriginalA = tex2D(ZBlurSampler, OriginalUV).x;
  //return float4(OriginalA,OriginalA,OriginalA,1.0);
  float l_AcumA = 1-OriginalA;
  float3 Blurred = OriginalRGB * (1-OriginalA);
  for(int i = 0; i < POISON_BLUR_KERNEL_SIZE; i++)
  {
    float2 l_displacement = g_PoissonBlurKernel[i] * OriginalA * MAX_RADI_BLUR;
    float2 l_UVFrameBuffer = OriginalUV + l_displacement / l_FrameBubberDivisor;
    float2 l_UVZBlur = OriginalUV + l_displacement / l_ZBlurDivisor;
    
    float3 CurrentRGB = tex2D(FrameBufferSampler, l_UVFrameBuffer).xyz;
    float  CurrentA   = tex2D(ZBlurSampler,       l_UVZBlur).x;
    // Lerp between original rgb and the jitter rgb based on the alpha value
    Blurred += CurrentRGB * (CurrentA);
    l_AcumA += CurrentA;
  }
  if(l_AcumA == 0)
  {
    Blurred = OriginalRGB;
    l_AcumA = 1;
  }
  return float4(Blurred / l_AcumA, 1.0f);
}

technique PostZBlurTechnique
{
  pass P0
  {
    PixelShader = compile ps_3_0 DepthOfFieldManySamples(/*TwelveKernel, 12*/);
  }
}

