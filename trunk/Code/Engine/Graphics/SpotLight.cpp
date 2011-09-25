#include "SpotLight.h"
#include "Core.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include "RenderableVertexs.h"

void CSpotLight::Init(CXMLTreeNode& _XMLParams)
{
  CDirectionalLight::Init(_XMLParams);
  m_fAngle = _XMLParams.GetFloatProperty("angle",1.0f);
  m_fFallOff = _XMLParams.GetFloatProperty("fall_off",1.0f);
}

void CSpotLight::RenderDeferredLight(CRenderManager* _pRM, CEffect* _pGeometryEffect)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();
  Mat44f m, t, s;
  t.SetIdentity();
  t.Translate(m_vPosition);

  float h = m_fEndRangeAttenuation;
  float b = m_fEndRangeAttenuation * tan(m_fFallOff * .75f * FLOAT_PI_VALUE / 180.f);
  h *= 1.5f;
  b *= 1.5f;
  s.SetIdentity();
  s.Scale(b,h,b);
  
  Mat33f r = GetFastestRotationFromDirToDir(Vect3f(0,1,0), m_vDirection);

  m = t * r * s;
  l_pEM->SetWorldMatrix(m);

  l_pEM->LoadShaderData(_pGeometryEffect);

  _pRM->GetConeVertexs()->Render(_pRM, _pGeometryEffect);
}
