#include "RenderableObject.h"
#include "RenderManager.h"

void CRenderableObject::Render(CRenderManager* _pRM, const vector<CEffect*>& _vEffects)
{
  //Mat44f l_mScaleMatrix;
  //l_mScaleMatrix.SetIdentity().Scale(m_vScale.x,m_vScale.y,m_vScale.z);

  //_pRM->SetTransform(GetMat44()*l_mScaleMatrix);

  RenderRenderableObject(_pRM,_vEffects);
}