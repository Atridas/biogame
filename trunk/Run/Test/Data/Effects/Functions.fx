

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
  float height = l_NormalMapColor.w * 0.06 - 0.03;
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