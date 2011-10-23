#include "Globals.fx"
#include "Functions.fx"

sampler PrevFilterSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

sampler NormalsTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

sampler DepthTextureSampler : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = CLAMP;
  AddressV  = CLAMP;
};

float4 CellShadingPS(float2 _UV: TEXCOORD0) : COLOR
{
  //float l_XIncrementTexture = 1.0 / (float)(g_TextureWidth);
  
  float4 l_Color = tex2D(PrevFilterSampler, _UV);
 
  float l_fLuminance = GetLuminancePonderat(l_Color.xyz);
  
  float l_fLum0 = smoothstep( 0.1, 0.2, l_fLuminance);
  float l_fLum1 = smoothstep( 0.5, 0.6, l_fLuminance);
  float l_fLum2 = smoothstep( 0.8, 0.9, l_fLuminance);
  
  float l_fExpectedLuminance = l_fLum0 * 0.2 + l_fLum1 * 0.4 + l_fLum2 * 0.4;
  
  l_Color.xyz *= l_fExpectedLuminance / l_fLuminance;
  
  return l_Color;
}


const static float2 offsets[9] = {  
  float2( 0.0,  0.0), //Center       0  
   float2(-1.0, -1.0), //Top Left     1  
   float2( 0.0, -1.0), //Top          2  
   float2( 1.0, -1.0), //Top Right    3  
   float2( 1.0,  0.0), //Right        4  
   float2( 1.0,  1.0), //Bottom Right 5  
   float2( 0.0,  1.0), //Bottom       6  
   float2(-1.0,  1.0), //Bottom Left  7  
   float2(-1.0,  0.0)  //Left         8  
};

float3 DL_GetNormal(float2 _UV)
{
  float4 l_NormalTextureValue = tex2D(NormalsTextureSampler, _UV);
  
	float3 l_vWorldNrm = float3((l_NormalTextureValue.xy - 0.5) * 2,0);
  l_vWorldNrm.z = -sqrt(1 - (l_vWorldNrm.x * l_vWorldNrm.x) - (l_vWorldNrm.y * l_vWorldNrm.y));
  
  l_vWorldNrm = normalize(l_vWorldNrm);
  
  return l_vWorldNrm;
}

float DL_GetDepth(float2 _UV)
{
  return tex2D(DepthTextureSampler,_UV).r;
}


float DL_GetEdgeWeight(float2 screenPos, float2 PixelSize)  
{  
  float Depth[9];  
  float3 Normal[9];  
  
  //Retrieve normal and depth data for all neighbors.  
  //for (int i=0; i<9; ++i)  
  //{  
  //  float2 uv = screenPos + offsets[i] * PixelSize;  
  //  Depth[i] = DL_GetDepth(uv);  //Retrieves depth from MRTs  
  //  Normal[i]= DL_GetNormal(uv); //Retrieves normal from MRTs  
  //}  
  Depth [0] = DL_GetDepth (screenPos + offsets[0] * PixelSize);
  Normal[0] = DL_GetNormal(screenPos + offsets[0] * PixelSize);
  Depth [1] = DL_GetDepth (screenPos + offsets[1] * PixelSize);
  Normal[1] = DL_GetNormal(screenPos + offsets[1] * PixelSize);
  Depth [2] = DL_GetDepth (screenPos + offsets[2] * PixelSize);
  Normal[2] = DL_GetNormal(screenPos + offsets[2] * PixelSize);
  Depth [3] = DL_GetDepth (screenPos + offsets[3] * PixelSize);
  Normal[3] = DL_GetNormal(screenPos + offsets[3] * PixelSize);
  Depth [4] = DL_GetDepth (screenPos + offsets[4] * PixelSize);
  Normal[4] = DL_GetNormal(screenPos + offsets[4] * PixelSize);
  Depth [5] = DL_GetDepth (screenPos + offsets[5] * PixelSize);
  Normal[5] = DL_GetNormal(screenPos + offsets[5] * PixelSize);
  Depth [6] = DL_GetDepth (screenPos + offsets[6] * PixelSize);
  Normal[6] = DL_GetNormal(screenPos + offsets[6] * PixelSize);
  Depth [7] = DL_GetDepth (screenPos + offsets[7] * PixelSize);
  Normal[7] = DL_GetNormal(screenPos + offsets[7] * PixelSize);
  Depth [8] = DL_GetDepth (screenPos + offsets[8] * PixelSize);
  Normal[8] = DL_GetNormal(screenPos + offsets[8] * PixelSize);
  
  
  
  //Compute Deltas in Depth.  
  float4 Deltas1;  
  float4 Deltas2;  
  Deltas1.x = Depth[1];  
  Deltas1.y = Depth[2];  
  Deltas1.z = Depth[3];  
  Deltas1.w = Depth[4];  
  Deltas2.x = Depth[5];  
  Deltas2.y = Depth[6];  
  Deltas2.z = Depth[7];  
  Deltas2.w = Depth[8];  
  //Compute absolute gradients from center.  
  Deltas1 = abs(Deltas1 - Depth[0]);  
  Deltas2 = abs(Deltas2 - Depth[0]);  
  
  //Find min and max gradient, ensuring min != 0  
  float4 maxDeltas = max(Deltas1, Deltas2);  
  float4 minDeltas = max(min(Deltas1, Deltas2), 0.00001);  
  
  // Compare change in gradients, flagging ones that change  
   // significantly.  
   // How severe the change must be to get flagged is a function of the  
   // minimum gradient. It is not resolution dependent. The constant  
   // number here would change based on how the depth values are stored  
   // and how sensitive the edge detection should be.  
   float4 depthResults = step(minDeltas * 5000.0, maxDeltas);  

   
  //Compute change in the cosine of the angle between normals.  
  Deltas1.x = dot(Normal[1], Normal[0]);  
  Deltas1.y = dot(Normal[2], Normal[0]);  
  Deltas1.z = dot(Normal[3], Normal[0]);  
  Deltas1.w = dot(Normal[4], Normal[0]);  
  Deltas2.x = dot(Normal[5], Normal[0]);  
  Deltas2.y = dot(Normal[6], Normal[0]);  
  Deltas2.z = dot(Normal[7], Normal[0]);  
  Deltas2.w = dot(Normal[8], Normal[0]);  
  Deltas1 = abs(Deltas1 - Deltas2);  
  // Compare change in the cosine of the angles, flagging changes  
   // above some constant threshold. The cosine of the angle is not a  
   // linear function of the angle, so to have the flagging be  
   // independent of the angles involved, an arccos function would be  
   // required.  
  float4 normalResults = step(0.4, Deltas1);  
  normalResults = max(normalResults, depthResults);  
  return (normalResults.x + normalResults.y +  
          normalResults.z + normalResults.w) * 0.25;  
}  


const static float2 offsets2[5] = {  
  float2( 0.0,  0.0), //Center       0  
   float2(-1.5, -1.5), //Top Left     1  
   float2( 1.5, -1.5), //Top Right    2  
   float2( 1.5,  1.5), //Bottom Right 3  
   float2(-1.5,  1.5), //Bottom Left  4  
};

float4 AntialiasingPS(float2 _UV : TEXCOORD0) : COLOR
{
  //float d = DL_GetDepth(_UV);
  //return float4(d,d,d  , 1.0);
  //return tex2D(PrevFilterSampler, _UV);
  //return float4( DL_GetNormal(_UV) , 1.0);

  float l_XIncrementTexture = 1.0 / (float)(g_TextureWidth);
  float l_YIncrementTexture = 1.0 / (float)(g_TextureHeight);
  float2 PixelSize = float2(l_XIncrementTexture, l_YIncrementTexture);
  
  float l_fAntialiasAmount = DL_GetEdgeWeight(_UV, PixelSize);
  
  //return float4(l_fAntialiasAmount, l_fAntialiasAmount, l_fAntialiasAmount, 1);
  
  float4 l_fColorAcum = float4(0,0,0,0);
  for(int i = 0; i < 5; i++)
  {
    l_fColorAcum += tex2D(PrevFilterSampler, _UV + (offsets2[i] * PixelSize * l_fAntialiasAmount));
  }
  
  return l_fColorAcum * 0.2;
  
}


technique CellShadingTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 CellShadingPS();
	}
}

technique AntialiasingTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 AntialiasingPS();
	}
}



// ------------------------------------------------


float4 SimpleBlurPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0 / (float)(g_TextureWidth);
  float l_YIncrementTexture = 1.0 / (float)(g_TextureHeight);
  float2 l_PixelSize = float2(l_XIncrementTexture, l_YIncrementTexture);
  
  float4 l_FinalColor = float4(0,0,0,0);
  
  for(int i = 0; i < POISON_BLUR_KERNEL_SIZE; i++)
  {
    float2 l_Texel = _UV + l_PixelSize * g_PoissonBlurKernel[i] * g_BlurRadius;
    float4 l_TexelColor = tex2D(PrevFilterSampler, l_Texel);
    
    l_FinalColor = l_FinalColor + l_TexelColor / POISON_BLUR_KERNEL_SIZE;
  }
  
  return l_FinalColor;
}



float BlurRadius(float _fDepth)
{
  if(_fDepth < g_NearBlurDepth)
  {
    return g_BlurRadius;
  } else if(_fDepth > g_FarBlurDepth)
  {
    return g_BlurRadius;
  } else if(_fDepth > g_FocalPlaneDepth)
  {
    return g_BlurRadius * (_fDepth - g_FocalPlaneDepth) / (g_FarBlurDepth - g_FocalPlaneDepth);
  } else
  {
    return g_BlurRadius * (g_FocalPlaneDepth - _fDepth) / (g_FocalPlaneDepth - g_NearBlurDepth);
  }
}

float4 ZBlurPS(float2 _UV: TEXCOORD0) : COLOR
{
  float l_XIncrementTexture = 1.0 / (float)(g_TextureWidth);
  float l_YIncrementTexture = 1.0 / (float)(g_TextureHeight);
  float2 l_PixelSize = float2(l_XIncrementTexture, l_YIncrementTexture);
  
  float4 l_FinalColor = float4(0,0,0,0);
  
  float l_BlurRadius = BlurRadius(tex2D(DepthTextureSampler, _UV).x);
  
  for(int i = 0; i < POISON_BLUR_KERNEL_SIZE; i++)
  {
    float2 l_Texel = _UV + l_PixelSize * g_PoissonBlurKernel[i] * l_BlurRadius;
    float4 l_TexelColor = tex2D(PrevFilterSampler, l_Texel);
    
    l_FinalColor = l_FinalColor + l_TexelColor / POISON_BLUR_KERNEL_SIZE;
  }
  
  return l_FinalColor;
}


technique SimpleBlurTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 SimpleBlurPS();
	}
}

technique ZBlurTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}
