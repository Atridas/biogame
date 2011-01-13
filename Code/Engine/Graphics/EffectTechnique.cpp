#include "EffectTechnique.h"
#include "Effect.h"

bool CEffectTechnique::BeginRender()
{

  return false;
}

bool CEffectTechnique::Refresh()
{

  return false;
}

D3DXHANDLE CEffectTechnique::GetD3DTechnique()
{

  return 0;
}

void CEffectTechnique::Release()
{
  //self
  CHECKED_DELETE(m_pEffect);
  //d3d9
  CHECKED_DELETE(m_pD3DTechnique);
}