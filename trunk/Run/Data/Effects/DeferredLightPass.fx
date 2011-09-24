#include "Globals.fx"
#include "Functions.fx"

sampler ColorTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler NormalsTextureSampler : register(s1) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler DepthTextureSampler : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};


struct T_DEF_LIGHTPASS_PS {
  float4 HPosition      : POSITION;
  float3 LightPosition  : TEXCOORD0;
  float3 LightDirection : TEXCOORD1;
};

//T_DEF_LIGHTPASS_PS DeferredLightPassVS(float4 _Position : POSITION)
//{
//  T_DEF_LIGHTPASS_PS Out_;
//  Out_.HPosition = _Position;
//  Out_.LightPosition = (mul(float4(g_LightPosition,1.0), g_ViewMatrix)).xyz;
//  Out_.LightDirection = mul(g_LightDirection, (float3x3)g_ViewMatrix);
//  
//  return Out_;
//}

//struct PS_OUTPUT {
//  float4 Light      : COLOR0;
//  float4 Specular   : COLOR1;
//};

struct VS_IN {
  float4 Position : POSITION;
};

struct VS_OUT {
  float2 uv: TEXCOORD0;
  float4 HPosition : POSITION;
};

float4 GeometryLightVS(float3 _Position : POSITION) : POSITION
{
  float4 HPosition_ = mul(float4(_Position,1.0), g_WorldViewProjectionMatrix );
  
  // fem que la llum mai "desaparegui" per el final del frustum
  HPosition_.z = min(HPosition_.z, HPosition_.w);
  
  return HPosition_;
}


float4 DeferredLightPassPS(float2 _vpos: VPOS) : COLOR
{

  float2 _UV = _vpos;
  _UV.x /= g_TextureWidth  - 1.0;
  _UV.y /= g_TextureHeight - 1.0;
  
  float z = tex2D(DepthTextureSampler, _UV).x;
  if(z == 0) discard;
  
	float3 l_DiffuseColor = tex2D(ColorTextureSampler, _UV).xyz;
  
  float4 l_NormalTextureValue = tex2D(NormalsTextureSampler, _UV);
  
	float3 l_vWorldNrm = float3((l_NormalTextureValue.xy - 0.5) * 2,0);
  l_vWorldNrm.z = -sqrt(1 - (l_vWorldNrm.x * l_vWorldNrm.x) - (l_vWorldNrm.y * l_vWorldNrm.y));
  
  l_vWorldNrm = normalize(l_vWorldNrm);
  
	float3 l_vWorldPos = PositionFromZ(z,_UV);
  
  float l_SpecularFactor = l_NormalTextureValue.z * g_SpotlightFactorMax;
  float l_SpecularPow = l_NormalTextureValue.a * g_SpecularPowMax;
  
  float3 l_EyeDirection = normalize(- l_vWorldPos);

  float3 l_LightDirection;
  float  l_Attenuation = 1.0;

  if(g_LightType == LIGHT_OMNI)
  {
    l_LightDirection = g_LightPosition - l_vWorldPos;
    float l_DistSquared = dot(l_LightDirection,l_LightDirection);
    if(l_DistSquared < 0.0 || l_DistSquared > g_LightEndRangeSQ)
    {
      discard;
    }
    
    if(l_DistSquared > g_LightStartRangeSQ) {
      l_Attenuation *= (g_LightEndRangeSQ - l_DistSquared) / (g_LightEndRangeSQ - g_LightStartRangeSQ);
    }
    
    l_LightDirection = normalize(l_LightDirection);
    
  } else if(g_LightType == LIGHT_DIRECTIONAL)
  {
    l_LightDirection = normalize(-g_LightDirection);
  } else if(g_LightType == LIGHT_SPOT)
  {
    l_LightDirection = g_LightPosition - l_vWorldPos;
    
    float l_DistSquared = dot(l_LightDirection,l_LightDirection);
    if(l_DistSquared < 0.0 || l_DistSquared > g_LightEndRangeSQ)
    {
      discard;
    }
    
    if(l_DistSquared > g_LightStartRangeSQ) {
      l_Attenuation *= (g_LightEndRangeSQ - l_DistSquared) / (g_LightEndRangeSQ - g_LightStartRangeSQ);
    }
    
    l_LightDirection = normalize(l_LightDirection);
    
    float l_cosAmbLightAngle = dot(l_LightDirection, -g_LightDirection);

    if(l_cosAmbLightAngle < g_LightFallOffCos)
    {
      discard;
    }
    
    if(l_cosAmbLightAngle < g_LightAngleCos)
    {
      l_Attenuation*=sin((3.1416/2.0)*(l_cosAmbLightAngle-g_LightFallOffCos)/(g_LightAngleCos-g_LightFallOffCos));
    }
  }
  if(l_Attenuation == 0.0)
  {
    discard;
  }
  
  float3 l_HalfWayVector = normalize(l_EyeDirection+l_LightDirection);
  float3 l_LightResult = ComputeLight( l_vWorldNrm, 
                        l_LightDirection, 
                        l_HalfWayVector, 
                        (g_LightColor * l_Attenuation).xyz, 
                        l_DiffuseColor, 
                        l_SpecularPow,
                        l_SpecularFactor); //_SpotlightFactor); 
  //PS_OUTPUT out_ = (PS_OUTPUT)0;
  //
  //float ndotl = saturate(dot(l_vWorldNrm, l_LightDirection));
  //float ndoth = dot(l_vWorldNrm, l_HalfWayVector);
  //
  //float3 l_LightResult = ndotl * g_LightColor * l_Attenuation;
  //
  //if( dot(l_LightResult, 1.0) == 0 ) discard;
  //
  //out_.Light = float4(l_LightResult.xyz,1.0);
  //out_.Specular = pow(abs(ndoth), l_SpecularPow) * g_LightColor * l_Attenuation * l_SpecularFactor;
  
  if( dot(l_LightResult, 1.0) == 0 ) discard;

	return float4(l_LightResult,1.0);
}

float4 white(float2 _vpos: VPOS) : COLOR
{

  float2 _UV = _vpos;
  _UV.x /= g_TextureWidth;
  _UV.y /= g_TextureHeight;
	float3 l_DiffuseColor = tex2D(ColorTextureSampler, _UV);
  //return float4(_UV.xy, 0, 1);
  //return float4(l_DiffuseColor,1.0);
  return float4(1, 1, 1, 1);
}

technique DeferredLightPassTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		AlphaTestEnable = false;	
		CullMode = CCW;
		//VertexShader = compile ps_3_0 DeferredLightPassVS();
		PixelShader  = compile ps_3_0 DeferredLightPassPS();
	}
}
/*
technique DeferredGeometryLightPassTechnique
{
	pass p0
	{
		ZEnable = true;
    ZFunc = Greater;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		AlphaTestEnable = false;	
		CullMode = CW;
    
		VertexShader = compile vs_3_0 GeometryLightVS();
		//PixelShader  = compile ps_3_0 DeferredLightPassPS();
    
		PixelShader  = compile ps_3_0 white();
	}
}
*/
technique DeferredGeometryLightPassTechnique
{
	pass p0
	{
		ZEnable = true;
    ZFunc   = LESS;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = None;
    
    StencilEnable = true;
    TwoSidedStencilMode = false;
    StencilRef = 0;
    //StencilMask			= 0x0000000F;
    //StencilWriteMask	= 0x0000000F;
    // stencil settings for front facing triangles
    StencilFunc			= Always;
    StencilZFail		= Invert;
    StencilPass			= Keep;
    StencilFail			= Keep;
    // stencil settings for back facing triangles 
    Ccw_StencilFunc		= Always;
    Ccw_StencilZFail	= Invert;
    Ccw_StencilPass		= Keep;
    Ccw_StencilFail		= Keep;
    
    ColorWriteEnable = 0x00000000;
    
		VertexShader = compile vs_3_0 GeometryLightVS();
		PixelShader  = null;
	}
  
	pass p1
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		AlphaTestEnable = false;	
		CullMode = CW;
    
    StencilEnable = true;
    TwoSidedStencilMode = true;
    // stencil settings for front facing triangles
    StencilFunc			= Less;
    StencilZFail		= Keep;
    StencilPass			= Keep;
    StencilFail			= Keep;
    // stencil settings for back facing triangles 
    Ccw_StencilFunc		= Less;
    Ccw_StencilZFail	= Keep;
    Ccw_StencilPass		= Keep;
    Ccw_StencilFail		= Keep;
    StencilRef = 0;
    //StencilMask			= 0x0000000F;
    //StencilWriteMask	= 0x0000000F;
    
    ColorWriteEnable = 0x0000000f;
    
		VertexShader = compile vs_3_0 GeometryLightVS();
		PixelShader  = compile ps_3_0 DeferredLightPassPS();
    
		//PixelShader  = compile ps_3_0 white();
	}
}
/*
technique DeferredGeometryLightPassTechnique
{
	pass p0
	{
		ZEnable = true;
    ZFunc   = LESSEQUAL;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		AlphaTestEnable = false;	
		CullMode = CCW;
		VertexShader = compile vs_3_0 GeometryLightVS();
		PixelShader  = compile ps_3_0 DeferredLightPassPS();
		//PixelShader  = compile ps_3_0 white();
	}
}
*/
technique DeferredGeometryInsideLightPassTechnique
{
	pass p0
	{
		ZEnable = true;
    ZFunc   = GREATEREQUAL;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = One;
		DestBlend = One;
		AlphaTestEnable = false;	
		CullMode = CW;
		VertexShader = compile vs_3_0 GeometryLightVS();
		//PixelShader  = compile ps_3_0 DeferredLightPassPS();
		PixelShader  = compile ps_3_0 white();
	}
}

/*
//Deferred.fx
technique StencilConvexLight
{	
  pass Pass0_DoubleSidedStencil	
  {		
    VertexShader		= compile vs_2_0 pos_vs_main();        
    PixelShader			= null;                
    ColorWriteEnable	= 0x0;        
    CullMode			= none;                
    // Disable writing to the frame buffer        
    AlphaBlendEnable	= true;        
    SrcBlend			= Zero;
    DestBlend			= One;
    // Disable writing to depth buffer
    ZWriteEnable		= false;
    ZEnable				= true;
    ZFunc				= Less;
    // Setup stencil states
    StencilEnable		= true;
    TwoSidedStencilMode = true;
    StencilRef			= 1;
    StencilMask			= 0xFFFFFFFF;
    StencilWriteMask	= 0xFFFFFFFF;
    // stencil settings for front facing triangles
    StencilFunc			= Always;
    StencilZFail		= Incr;
    StencilPass			= Keep;
    // stencil settings for back facing triangles 
    Ccw_StencilFunc		= Always;
    Ccw_StencilZFail	= Decr;
    Ccw_StencilPass		= Keep;
  }
  pass Pass1_PointLightDiffuse
	{
    VertexShader	= compile vs_3_0 pos_vs_main();
		PixelShader		= compile ps_3_0 Pass4_diffuse_point_ps_main();
    ZEnable			= false;
		ZWriteEnable	= false;
    AlphaBlendEnable = true;
    SrcBlend		= One;
    DestBlend		= One;
    CullMode		= CW;
    ColorWriteEnable = 0xFFFFFFFF;
    StencilEnable	= true;
    TwoSidedStencilMode = false;
    StencilFunc		= Equal;
		StencilFail		= Keep;
		StencilZFail	= Keep;
		StencilPass		= Keep;
		StencilRef		= 0;
		StencilMask		= 0xFFFFFFFF;
    StencilWriteMask = 0xFFFFFFFF;
  }
  pass Pass2_ShowStencilResult
	{
    VertexShader	= compile vs_3_0 pos_vs_main();
		PixelShader		= compile ps_3_0 stencil_bright_light_ps_main();
    ZEnable			= false;
		ZWriteEnable	= false;
    AlphaBlendEnable = true;
    SrcBlend		= One;
    DestBlend		= One;
    CullMode		= CW;	
    ColorWriteEnable = 0xFFFFFFFF;  
		StencilEnable	= true;  	
    TwoSidedStencilMode = false;   
    StencilFunc		= Equal;	
    StencilFail		= Keep;		
    StencilZFail	= Keep;	
    StencilPass		= Keep;	
    StencilRef		= 0;		
    StencilMask		= 0xFFFFFFFF;    
    StencilWriteMask = 0xFFFFFFFF;	
  }
}
*/
