#include "OmniLight.h"

#include "RenderManager.h"

void COmniLight::Init(CXMLTreeNode& _XMLParams)
{
  CLight::Init(_XMLParams);


}

void COmniLight::Render(CRenderManager* _pRM) const
{
    Mat44f t;
    
    t.SetIdentity();

    t.Translate(m_vPosition);

    _pRM->SetTransform(t);

    _pRM->DrawAxis();
}

void COmniLight::RenderDeferredLight(CRenderManager* _pRM, CEffect* _pGeometryEffect)
{
  _pRM->DrawShadedSphere(m_vPosition, m_fEndRangeAttenuation* 1.5f, _pGeometryEffect);
}
