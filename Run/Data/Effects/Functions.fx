#ifndef __FUNCTIONS_FX__
#define __FUNCTIONS_FX__

#include "Globals.fx"
#include "Samplers.fx"

//shadow
int ShadowAmount(float4 _PosLight)
{
  float2 ShadowTexC = 0.5 * _PosLight.xy/_PosLight.w + float2( 0.5, 0.5 );
  ShadowTexC.y = 1.0f - ShadowTexC.y;

  bool l_bIsInShadow = tex2D( ShadowTextureSampler, ShadowTexC ).x + SHADOW_EPSILON < _PosLight.z/_PosLight.w;
  
  if(l_bIsInShadow)
    return 1;
  else
    return 0;
}

//Lights

//All params must be pre-normalized
float3 ComputeLight(float3 _Normal,          //Normal of the pixel
                    float3 _LightDirection,  //Direction TO THE Light
                    float3 _HalfPlaneVector, //Vector intermig camera-llum
                    float3 _LightColor,
                    float3 _MaterialColor,
                    float  _MaterialSpecularPow,
                    float  _MaterialSpotlightFactor)
{
  float3 _out;
  
  float ndotl = saturate(dot(_Normal, _LightDirection));
  float ndoth = dot(_Normal, _HalfPlaneVector);

  float3 l_AuxColor = _LightColor * _MaterialColor;
  _out = ndotl * l_AuxColor;
	  
  if(ndoth > 0.0)	
  {
    _out += pow(abs(ndoth), _MaterialSpecularPow) * l_AuxColor * _MaterialSpotlightFactor;
  }
  
  return _out;
}

// normal & eyeDirection han d'estar normalitzades
float3 ComputeAllLights(
                        float3 _Normal, float3 _WorldPosition, float3 _DiffuseColor, 
                        float3 _AmbientLight, float _SpotlightFactor, 
                        float4 _PosLight
                       )
{
  float3 out_ = _DiffuseColor * _AmbientLight;
  float3 l_EyeDirection = normalize(g_CameraPosition - _WorldPosition);
  for(int i = 0; i < MAXLIGHTS; i++)
  {
    int l_iShadowAmount = 0;

    if(g_ShadowEnabled[i])
    {
      l_iShadowAmount = ShadowAmount(_PosLight);
    }

    if(g_LightsEnabled[i] && l_iShadowAmount < 1)
    {

      float3 l_LightDirection;
      float  l_Attenuation = 1.0 - l_iShadowAmount;
      if(g_LightsType[i] == LIGHT_OMNI)
      {
        l_LightDirection = g_LightsPosition[i] - _WorldPosition;
        float l_DistSquared = dot(l_LightDirection,l_LightDirection);
        if(l_DistSquared < 0.0 || l_DistSquared > g_LightsEndRangeSQ[i])
        {
          l_Attenuation = 0.0;
        } else if(l_DistSquared > g_LightsStartRangeSQ[i]) {
          l_Attenuation *= (g_LightsEndRangeSQ[i] - l_DistSquared) / (g_LightsEndRangeSQ[i] - g_LightsStartRangeSQ[i]);
        }
        if(l_Attenuation > 0.0)
        {
          l_LightDirection = normalize(l_LightDirection);
        }
      } else if(g_LightsType[i] == LIGHT_DIRECTIONAL)
      {
        l_LightDirection = -g_LightsDirection[i];
      } else if(g_LightsType[i] == LIGHT_SPOT)
      {
        l_LightDirection = g_LightsPosition[i] - _WorldPosition;
        
        float l_DistSquared = dot(l_LightDirection,l_LightDirection);
        if(l_DistSquared < 0.0 || l_DistSquared > g_LightsEndRangeSQ[i])
        {
          l_Attenuation = 0.0;
        } else if(l_DistSquared > g_LightsStartRangeSQ[i]) {
          l_Attenuation *= (g_LightsEndRangeSQ[i] - l_DistSquared) / (g_LightsEndRangeSQ[i] - g_LightsStartRangeSQ[i]);
        }
        if(l_Attenuation > 0.0)
        {
          l_LightDirection = normalize(l_LightDirection);
          
          float l_cosAmbLightAngle = dot(l_LightDirection, -g_LightsDirection[i]);
		  
          if(l_cosAmbLightAngle < g_LightsFallOffCos[i])
          {
            l_Attenuation = 0.0;
          } else if(l_cosAmbLightAngle < g_LightsAngleCos[i])
          {
            l_Attenuation*=sin((3.1416/2.0)*(l_cosAmbLightAngle-g_LightsFallOffCos[i])/(g_LightsAngleCos[i]-g_LightsFallOffCos[i]));
            
          }
          
        }
      }
      if(l_Attenuation > 0.0)
      {
        float3 l_HalfWayVector = normalize(l_EyeDirection+l_LightDirection);
        out_ += ComputeLight( _Normal, 
                              l_LightDirection, 
                              l_HalfWayVector, 
                              g_LightsColor[i] * l_Attenuation, 
                              _DiffuseColor, 
                              g_SpecularPow,
                              _SpotlightFactor);
      }
    }
  }
  return out_;
}

float4 RadiosityNormalLightmapColor(float3 _Normal, float2 _UV)
{
  float4 l_LightR = tex2D(Radiosity_R_TextureSampler,_UV);
  float4 l_LightG = tex2D(Radiosity_G_TextureSampler,_UV);
  float4 l_LightB = tex2D(Radiosity_B_TextureSampler,_UV);
  
  return l_LightR * dot(_Normal, g_RadiosityNormalR)
       + l_LightG * dot(_Normal, g_RadiosityNormalG)
       + l_LightB * dot(_Normal, g_RadiosityNormalB);
}

float3 ComputeLightTangentSpace(float3 _Normal, float3 _Position, float3 _DiffuseColor, 
                                float3 _LightPosition, float3 _LightDirection, int lightID,
                                float3 _DirectionToEye)
{
  if(g_LightsEnabled[lightID])
  {
    float3 l_DirectionToLight;
    float  l_Attenuation = 1.0;
    if(g_LightsType[lightID] == LIGHT_OMNI)
    {
      l_DirectionToLight = _LightPosition - _Position;
      float l_DistSquared = dot(l_DirectionToLight,l_DirectionToLight);
      if(l_DistSquared < 0.0 || l_DistSquared > g_LightsEndRangeSQ[lightID])
      {
        l_Attenuation = 0.0;
      } else if(l_DistSquared > g_LightsStartRangeSQ[lightID]) {
        l_Attenuation *= (g_LightsEndRangeSQ[lightID] - l_DistSquared) / (g_LightsEndRangeSQ[lightID] - g_LightsStartRangeSQ[lightID]);
      }
      if(l_Attenuation > 0.0)
      {
        l_DirectionToLight = normalize(l_DirectionToLight);
      }
    } else if(g_LightsType[lightID] == LIGHT_DIRECTIONAL)
    {
      l_DirectionToLight = -_LightDirection;
    } else if(g_LightsType[lightID] == LIGHT_SPOT)
    {
      l_DirectionToLight = _LightPosition - _Position;
      
      float l_DistSquared = dot(l_DirectionToLight,l_DirectionToLight);
      if(l_DistSquared < 0.0 || l_DistSquared > g_LightsEndRangeSQ[lightID])
      {
        l_Attenuation = 0.0;
      } else if(l_DistSquared > g_LightsStartRangeSQ[lightID]) {
        l_Attenuation *= (g_LightsEndRangeSQ[lightID] - l_DistSquared) / (g_LightsEndRangeSQ[lightID] - g_LightsStartRangeSQ[lightID]);
      }
      if(l_Attenuation > 0.0)
      {
        l_DirectionToLight = normalize(l_DirectionToLight);
        
        float l_cosAmbLightAngle = dot(l_DirectionToLight, -_LightDirection);
    
        if(l_cosAmbLightAngle < g_LightsFallOffCos[lightID])
        {
          l_Attenuation = 0.0;
        } else if(l_cosAmbLightAngle < g_LightsAngleCos[lightID])
        {
          l_Attenuation*=sin((3.1416/2.0)*(l_cosAmbLightAngle-g_LightsFallOffCos[lightID])/(g_LightsAngleCos[lightID]-g_LightsFallOffCos[lightID]));
          
        }
        
      }
    }
    if(l_Attenuation > 0.0)
    {
      float3 l_HalfWayVector = normalize(_DirectionToEye+l_DirectionToLight);
      return ComputeLight(_Normal, 
                          l_DirectionToLight, 
                          l_HalfWayVector, 
                          g_LightsColor[lightID] * l_Attenuation, 
                          _DiffuseColor, 
                          g_SpecularPow,
                          g_SpotlightFactor);
    }
  }
  return float4(0,0,0,0);
}

//Normal Mapping / Parallax Mapping

float3 CalcNormalmap(float3 _Tangent, float3 _Binormal, float3 _Normal, float2 _UV)
{
  float3 l_Tangent  = normalize(_Tangent);
  float3 l_Binormal = normalize(_Binormal);
  float3 l_Normal   = normalize(_Normal);
  
  //La variable g_Bump es una constante que nos dará la profundidad, podemos utilizar un valor de
  float3 l_Bump=g_Bump*(tex2D(NormalTextureSampler,_UV).rgb - float3(0.5,0.5,0.5));

  return normalize(l_Bump.x*l_Tangent + l_Bump.y*l_Binormal + l_Bump.z*l_Normal);
}

float3 CalcParallaxMap(float3 _Position, float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV, out float2 OutUV)
{
	float2 l_UV = UV;
	float3 Vn = normalize(g_CameraPosition - _Position);
	// parallax code
	float3x3 tbnXf = float3x3(WorldTangent,WorldBinormal,WorldNormal);
	float4 l_NormalMapColor = tex2D(NormalTextureSampler,l_UV);
	float height = (1-l_NormalMapColor.w) * 2.0 * g_ParallaxHeight - g_ParallaxHeight;

	l_UV += height * mul(tbnXf,Vn).xy;
	// normal map
	float3 tNorm = l_NormalMapColor.xyz - float3(0.5,0.5,0.5);
	// transform tNorm to world space
	tNorm = normalize(tNorm.x*WorldTangent - tNorm.y*WorldBinormal + tNorm.z*WorldNormal);
	OutUV=l_UV;
	return tNorm;
}

// Cal3d functions

float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;

	return l_Position;
}

void CalcAnimatedNormalTangent(float3 Normal,
								float3 Tangent,
								float4 Indices,
								float4 Weight,
								out float3 OutNormal,
								out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	float3x3 m;
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.y);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.y);
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.z);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.z);
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.w);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.w);
	OutNormal = normalize(OutNormal);
	OutTangent = normalize(OutTangent);
}

// --------------------------------------------------------------------------------------------------------------------------------

float4x4 GetWorldMatrix(float4 WorldMatrix0, float4 WorldMatrix1, float4 WorldMatrix2, float4 WorldMatrix3)
{
  /*
  float4x4 aux = {
                    {WorldMatrix0.x, WorldMatrix1.x, WorldMatrix2.x, WorldMatrix3.x},
                    {WorldMatrix0.y, WorldMatrix1.y, WorldMatrix2.y, WorldMatrix3.y},
                    {WorldMatrix0.z, WorldMatrix1.z, WorldMatrix2.z, WorldMatrix3.z},
                    {WorldMatrix0.w, WorldMatrix1.w, WorldMatrix2.w, WorldMatrix3.w},
                 };  
                   */    
  float4x4 aux = {
                    {WorldMatrix0.x, WorldMatrix0.y, WorldMatrix0.z, WorldMatrix0.w},
                    {WorldMatrix1.x, WorldMatrix1.y, WorldMatrix1.z, WorldMatrix1.w},
                    {WorldMatrix2.x, WorldMatrix2.y, WorldMatrix2.z, WorldMatrix2.w},
                    {WorldMatrix3.x, WorldMatrix3.y, WorldMatrix3.z, WorldMatrix3.w},
                 }; 
               /*  
  float4x4 aux = {
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1},
                 };  
                 */
  //float4x4 aux = g_WorldMatrix;
  return aux;
}

#endif