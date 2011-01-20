

//Lights

//All params must be pre-normalized
float3 ComputeLight(float3 _Normal,          //Normal of the pixel
                    float3 _LightDirection,  //Direction TO THE Light
                    float3 _HalfPlaneVector, //Vector intermig camera-llum
                    float3 _LightColor,
                    float3 _MaterialColor,
                    float  _MaterialSpecular
                    )
{
  float3 _out;
  
  float ndotl = saturate(dot(_Normal, _LightDirection));
  float ndoth = saturate(dot(_Normal, _HalfPlaneVector));
  
  float3 l_AuxColor = _LightColor * _MaterialColor;
  _out = ndotl * l_AuxColor;
  
  if(ndoth > 0.0)
  {
    _out += pow(ndoth, _MaterialSpecular) * l_AuxColor;
  }
  
  return _out;
}

// normal & eyeDirection han d'estar normalitzades
float3 ComputeAllLights(float3 _Normal, float3 _WorltPosition, float3 _EyeDirection, float _DiffuseColor)
{
  float3 out_ = float3(0.0, 0.0, 0.0);
  for(int i = 0; i < MAXLIGHTS; i++)
  {
    if(g_LightsEnabled[i])
    {
      float3 l_LightDirection;
      float  l_Attenuation = 1.0;
      if(g_LightsType[i] == LIGHT_OMNI)
      {
        l_LightDirection = normalize(g_LightsPosition[i] - _WorltPosition);
        //TODO
      } else if(g_LightsType[i] == LIGHT_DIRECTIONAL)
      {
        l_LightDirection = -g_LightsDirection[i];
        //TODO
      } else if(g_LightsType[i] == LIGHT_SPOT)
      {
        l_LightDirection = -g_LightsDirection[i];
        //TODO
      }
      if(l_Attenuation > 0.0)
      {
        float3 l_HalfWayVector = normalize(_EyeDirection+l_LightDirection);
        out_ += ComputeLight( _Normal, 
                              l_LightDirection, 
                              l_HalfWayVector, 
                              g_LightsColor[i] * l_Attenuation, 
                              _DiffuseColor, 
                              g_SpecularFactor);
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
  // view directions
  float3 Vn = normalize(_Position); //TODO
  out_.UV = _UV;
  // parallax code
  float3x3 tbnXf = float3x3(_Tangent,_Binormal,_Normal);
  float4 l_NormalMapColor = tex2D(NormalTextureSampler,out_.UV);
  float height = l_NormalMapColor.w * g_BumpDiff + g_BumpMinHeight;
  //float height = reliefTex.w * g_Bump - g_Bump*0.5;
  out_.UV += height * mul(tbnXf,Vn).xy;
  // normal map
  out_.Normal = l_NormalMapColor.xyz - float3(0.5,0.5,0.5);
  // transform tNorm to world space
  out_.Normal = normalize(out_.Normal.x*_Tangent -
                          out_.Normal.y*_Binormal +
                          out_.Normal.z*_Normal);
  return out_;
}