#include "RenderableObject.h"

#include "RenderManager.h"

void CRenderableObject::Render(CRenderManager* _pRM)
{
  if(m_bVisible)
  {
    Mat44f r, r2, r3, t, total;
    
    t.SetIdentity();
    r.SetIdentity();
    r2.SetIdentity();
    r3.SetIdentity();

    t.Translate(m_vPosition);
    r.SetFromAngleY(-m_fYaw);
    r2.SetFromAngleZ(m_fPitch);
    r3.SetFromAngleX(m_fRoll);
    //s.Scale(1.5f,1.5f,1.5f);

    total = t*r*r2*r3;

    _pRM->SetTransform(total);

    RenderRenderableObject(_pRM);

    if(m_bRenderBoundingBox)
    {
      _pRM->RenderBoundingBox(GetBoundingBox());
    }

    if(m_bRenderBoundingSphere)
    {
      _pRM->RenderBoundingSphere(GetBoundingSphere());
    }
  }
}