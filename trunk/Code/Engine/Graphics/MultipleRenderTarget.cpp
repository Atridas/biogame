#include "MultipleRenderTarget.h"
#include "TextureRenderTarget.h"
#include "XML/XMLTreeNode.h"
#include "RenderManager.h"
#include "Core.h"

bool CMultipleRenderTarget::Init(const string& _szName)
{
  SetName(_szName);
  SetOk(true);
  return IsOk();
}

bool CMultipleRenderTarget::AddTextureRenderTarget(CTextureRenderTarget* _pTextureRenderTarget, int _iIndex)
{
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTarget = m_vTextureRenderTargets.begin();
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTargetEnd = m_vTextureRenderTargets.end();

  bool l_bIndexCorrecte = true;
  bool l_bDimensionsCorrectes = true;

  if(_iIndex >= 0 && _iIndex <= 3)
  {
    while(l_itTextureRenderTarget != l_itTextureRenderTargetEnd)
    {
      int l_iIndex2 = (*l_itTextureRenderTarget).m_iIndex;

      if(_iIndex == l_iIndex2)
      {
        l_bIndexCorrecte = true;
      }

      if((*l_itTextureRenderTarget).m_pTextureRenderTarget->GetWidth() != _pTextureRenderTarget->GetWidth() ||
          (*l_itTextureRenderTarget).m_pTextureRenderTarget->GetHeight() != _pTextureRenderTarget->GetHeight())
      {
        l_bDimensionsCorrectes = false;
      }

      ++l_itTextureRenderTarget;
    }
  }else{
    l_bIndexCorrecte = false;
  }

  if(l_bIndexCorrecte)
  {
    if(l_bDimensionsCorrectes)
    {
      SIndexedTextureRenderTarget l_IndexedTRT;

      l_IndexedTRT.m_pTextureRenderTarget = _pTextureRenderTarget;
      l_IndexedTRT.m_iIndex = _iIndex;

      m_vTextureRenderTargets.push_back(l_IndexedTRT);

      if(m_vTextureRenderTargets.size() == 1)
      {
        CRenderTarget::Init(l_IndexedTRT.m_pTextureRenderTarget->GetDepthStencilSurface());
      }

      return true;

    }else{
      LOGGER->AddNewLog(ELL_ERROR,"CMultipleRenderTarget::AddTextureRenderTarget tots els render_targets han de tenir la mateixa dimensio");
    }
  }else{
    LOGGER->AddNewLog(ELL_ERROR,"CMultipleRenderTarget::AddTextureRenderTarget index incorrecte %d. Index valids: 0..3",_iIndex);
  }

  return false;
}

void CMultipleRenderTarget::Activate(CRenderManager* l_pRM)
{
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTarget = m_vTextureRenderTargets.begin();
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTargetEnd = m_vTextureRenderTargets.end();

  l_pRM->SetDepthStencilBuffer(m_pDepthStencilSurface);

  for(;l_itTextureRenderTarget != l_itTextureRenderTargetEnd; ++l_itTextureRenderTarget)
  {
    CTextureRenderTarget* l_pSingleRenderTarget = (*l_itTextureRenderTarget).m_pTextureRenderTarget;
    int l_iIndex = (*l_itTextureRenderTarget).m_iIndex;

    l_pSingleRenderTarget->Activate(l_pRM,l_iIndex);
  }

}

void CMultipleRenderTarget::Deactivate(CRenderManager* l_pRM)
{
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTarget = m_vTextureRenderTargets.begin();
  vector<SIndexedTextureRenderTarget>::iterator l_itTextureRenderTargetEnd = m_vTextureRenderTargets.end();

  for(;l_itTextureRenderTarget != l_itTextureRenderTargetEnd; ++l_itTextureRenderTarget)
  {
    CTextureRenderTarget* l_pSingleRenderTarget = (*l_itTextureRenderTarget).m_pTextureRenderTarget;
    int l_iIndex = (*l_itTextureRenderTarget).m_iIndex;

    if(l_iIndex)
      l_pRM->SetRenderTarget(l_iIndex,0);
  }
}

void CMultipleRenderTarget::Release()
{
  m_vTextureRenderTargets.clear();
}

int CMultipleRenderTarget::GetWidth ()
{
  if(m_vTextureRenderTargets.size() == 0)
    return RENDER_MANAGER->GetScreenWidth();
  else
    return m_vTextureRenderTargets[0].m_pTextureRenderTarget->GetWidth();
}

int CMultipleRenderTarget::GetHeight()
{
  if(m_vTextureRenderTargets.size() == 0)
    return RENDER_MANAGER->GetScreenHeight();
  else
    return m_vTextureRenderTargets[0].m_pTextureRenderTarget->GetHeight();
}
