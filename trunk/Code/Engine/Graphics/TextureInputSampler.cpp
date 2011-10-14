#include "TextureInputSampler.h"
#include "TextureManager.h"
#include "Core.h"

bool CTextureInputSampler::Init(int _iIndex, const string& _szName, bool _bIsCube)
{
  CInputSampler::Init(_iIndex);
  if(_bIsCube)
    m_pTexture = CORE->GetTextureManager()->GetCubeTexture(_szName);
  else
    m_pTexture = CORE->GetTextureManager()->GetResource(_szName);

  SetOk(true);
  return IsOk();
}

