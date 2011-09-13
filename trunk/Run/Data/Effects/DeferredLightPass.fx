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

sampler PosXYTextureSampler : register(s2) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

sampler DepthTextureSampler : register(s3) = sampler_state
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

float4 DeferredLightPassPS(float2 _UV: TEXCOORD0) : COLOR
{
	float3 l_DiffuseColor = tex2D(ColorTextureSampler, _UV).rgb;
	//float3 l_vSpecularMaterial = tex2D(SceneMaterialSampler, i.vTex0).a;
	
	// normals are stored in texture space [0,1] -> convert them back to [-1,+1] range
	float3 l_vWorldNrm = (tex2D(NormalsTextureSampler, _UV) - 0.5) * 2;
  l_vWorldNrm.z = -sqrt(1 - (l_vWorldNrm.x * l_vWorldNrm.x) - (l_vWorldNrm.y * l_vWorldNrm.y));

  l_vWorldNrm = normalize(l_vWorldNrm);
  
	//l_vWorldPos.xy = tex2D(PosXYTextureSampler, _UV).xy;
	float z = tex2D(DepthTextureSampler, _UV).x;
	float3 l_vWorldPos = PositionFromZ(z,_UV);
  
  float3 l_EyeDirection = normalize(- l_vWorldPos);
  
	//float3 vLightDir = normalize(c_vLightPos - vWorldPos);
	//float3 vEyeVec = normalize(c_mViewInverse[3].xyz - vWorldPos);	
  
  float3 l_LightResult = float3(0.0,0.0,0.0);

  float3 l_LightDirection;
  float  l_Attenuation = 1.0;

  if(g_LightType == LIGHT_OMNI)
  {
    l_LightDirection = g_LightPosition - l_vWorldPos;
    float l_DistSquared = dot(l_LightDirection,l_LightDirection);
    if(l_DistSquared < 0.0 || l_DistSquared > g_LightEndRangeSQ)
    {
      l_Attenuation = 0.0;
    } else if(l_DistSquared > g_LightStartRangeSQ) {
      l_Attenuation *= (g_LightEndRangeSQ - l_DistSquared) / (g_LightEndRangeSQ - g_LightStartRangeSQ);
    }
    if(l_Attenuation > 0.0)
    {
      l_LightDirection = normalize(l_LightDirection);
    }
  } else if(g_LightType == LIGHT_DIRECTIONAL)
  {
    l_LightDirection = normalize(-g_LightDirection);
  } else if(g_LightType == LIGHT_SPOT)
  {
    l_LightDirection = g_LightPosition - l_vWorldPos;
    
    float l_DistSquared = dot(l_LightDirection,l_LightDirection);
    if(l_DistSquared < 0.0 || l_DistSquared > g_LightEndRangeSQ)
    {
      l_Attenuation = 0.0;
    } else if(l_DistSquared > g_LightStartRangeSQ) {
      l_Attenuation *= (g_LightEndRangeSQ - l_DistSquared) / (g_LightEndRangeSQ - g_LightStartRangeSQ);
    }
    if(l_Attenuation > 0.0)
    {
      l_LightDirection = normalize(l_LightDirection);
      
      float l_cosAmbLightAngle = dot(l_LightDirection, -g_LightDirection);
  
      if(l_cosAmbLightAngle < g_LightFallOffCos)
      {
        l_Attenuation = 0.0;
      } else if(l_cosAmbLightAngle < g_LightAngleCos)
      {
        l_Attenuation*=sin((3.1416/2.0)*(l_cosAmbLightAngle-g_LightFallOffCos)/(g_LightAngleCos-g_LightFallOffCos));
      }
      
    }
  }
  if(l_Attenuation > 0.0)
  {
    float3 l_HalfWayVector = normalize(l_EyeDirection+l_LightDirection);
    l_LightResult = ComputeLight( l_vWorldNrm, 
                          l_LightDirection, 
                          l_HalfWayVector, 
                          g_LightColor * l_Attenuation, 
                          l_DiffuseColor, 
                          15.0,
                          1.0); //_SpotlightFactor);      
  }
  
  if( dot(l_LightResult.xyz, 1.0) == 0 ) discard;
  
  float4 out_ = float4(l_LightResult.xyz,1.0);
	return out_;
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