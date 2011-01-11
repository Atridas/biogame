#include "DirectionalLight.h"

#include "RenderManager.h"


void CDirectionalLight::Render(CRenderManager *_pRM) const
{
    Mat44f t, r, r2, r3, total;
    
    t.SetIdentity();
    //r.SetIdentity();
    //r2.SetIdentity();
    //r3.SetIdentity();

    t.Translate(m_vPosition);
    //r.SetFromAngleY(-m_fYaw);
    //r2.SetFromAngleZ(m_fPitch);
    //r3.SetFromAngleX(m_fRoll);

    //total = t*r*r2*r3;

    _pRM->SetTransform(t);

    _pRM->DrawAxis();
    _pRM->DrawLine(v3fZERO,m_vDirection*2,colWHITE);
}


void CDirectionalLight::SetDirection(const Vect3f& _vDirection) 
{
  m_vDirection = _vDirection.GetNormalized();
  m_vDirection.GetAngles(m_fRoll, m_fYaw, m_fPitch);
};
