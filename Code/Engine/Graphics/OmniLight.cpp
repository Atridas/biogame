#include "OmniLight.h"

#include "RenderManager.h"

void COmniLight::Render(CRenderManager* _pRM) const
{
    Mat44f t;
    
    t.SetIdentity();

    t.Translate(m_vPosition);

    _pRM->SetTransform(t);

    _pRM->DrawAxis();
}
