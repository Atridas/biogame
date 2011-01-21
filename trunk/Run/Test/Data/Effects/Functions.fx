

//Lights

//All params must be pre-normalized
float3 ComputeLight(float3 _Normal,          //Normal of the pixel
                    float3 _LightDirection,  //Direction TO THE Light
                    float3 _HalfPlaneVector, //Vector intermig camera-llum
                    float3 _LightColor,
                    float3 _MaterialColor,
                    float  _MaterialSpecularPow,
                    float  _MaterialSpotlightFactor
                    )
{
  float3 _out;
  
  float ndotl = saturate(dot(_Normal, _LightDirection));
  float ndoth = dot(_Normal, _HalfPlaneVector);
  
  float3 l_AuxColor = _LightColor * _MaterialColor;
  _out = ndotl * l_AuxColor;
  
  if(ndoth > 0.0)
  {
    _out += pow(ndoth, _MaterialSpecularPow) * l_AuxColor * _MaterialSpotlightFactor;
  }
  
  return _out;
}

// normal & eyeDirection han d'estar normalitzades
float3 ComputeAllLights(float3 _Normal, float3 _WorltPosition, float3 _DiffuseColor)
{
  float3 out_ = _DiffuseColor * g_AmbientLight;
  float3 l_EyeDirection = normalize(g_CameraPosition - _WorltPosition);
  for(int i = 0; i < MAXLIGHTS; i++)
  {
    if(g_LightsEnabled[i])
    {
      float3 l_LightDirection;
      float  l_Attenuation = 1.0;
      if(g_LightsType[i] == LIGHT_OMNI)
      {
        l_LightDirection = g_LightsPosition[i] - _WorltPosition;
        l_LightDirection = mul(l_LightDirection,(float3x3)g_WorldMatrix);
        //TODO
      } else if(g_LightsType[i] == LIGHT_DIRECTIONAL)
      {
        l_LightDirection = mul(-g_LightsDirection[i],(float3x3)g_WorldMatrix);
        //TODO
      } else if(g_LightsType[i] == LIGHT_SPOT)
      {
        l_LightDirection = mul(-g_LightsDirection[i],(float3x3)g_WorldMatrix);
        //TODO
      }
      if(l_Attenuation > 0.0)
      {
        l_LightDirection = normalize(l_LightDirection);
        float3 l_HalfWayVector = normalize(l_EyeDirection+l_LightDirection);
        out_ += ComputeLight( _Normal, 
                              l_LightDirection, 
                              l_HalfWayVector, 
                              g_LightsColor[i] * l_Attenuation, 
                              _DiffuseColor, 
                              g_SpecularPow,
                              g_SpotlightFactor);
      }
    }
  }
  return out_;
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

struct NORMAL_UV {
  float3 Normal;
  float2 UV;
};

NORMAL_UV CalcParallaxMap(float3 _Position, float3 _Normal, float3 _Tangent, float3 _Binormal, float2 _UV)
{
  NORMAL_UV out_ = (NORMAL_UV)0;

  //calcul normal
  float4 l_NormalMapColor = tex2D(NormalTextureSampler,_UV);

  out_.Normal = l_NormalMapColor.xyz - float3(0.5,0.5,0.5);

  // transform tNorm to world space
  out_.Normal = normalize(out_.Normal.x*_Tangent -
                          out_.Normal.y*_Binormal +
                          out_.Normal.z*_Normal);

  //calcul mapa
  float3x3 tbnXf = float3x3(_Tangent,_Binormal,_Normal);
  float3 Vn = normalize(_Position);

  out_.UV = _UV;

  float height = l_NormalMapColor.w * (g_BumpMaxHeight - g_BumpMinHeight) + g_BumpMinHeight;

  out_.UV += g_ParallaxHeight * height *  mul(tbnXf,Vn).xy;

  return out_;
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