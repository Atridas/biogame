#include "Globals.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

// Optimització per separabilitat -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//float g_XIncrementTexture = 1/g_TextureWidth;
//float g_YIncrementTexture = 1/g_TextureHeight;

//Triangle de pascal / tartaglia nivell 7

//       1
//     1 1
//    1 2 1
//  1 3 3 1
//1 4 6 4 1   => 16
// 1   6  15  20   15  6   1       +++ => 64
//#define GLOW_KERNEL_HALF 3
//float g_GaussianKernel[GLOW_KERNEL_HALF] = {0.375 , 0.25, 0.0625};
//#define GLOW_KERNEL_HALF 4
//float g_GaussianKernel[GLOW_KERNEL_HALF] = {0.315, 0.234375 , 0.09375, 0.015625};

#define GLOW_KERNEL_HALF 7
float g_GaussianKernel[7] = {
                        0.3989422804,
                        0.2419707245,
                        0.2419707245,
                        0.0539909665,
                        0.0539909665,
                        0.0044318484,
                        0.0044318484};