#include "TextureInputSampler.h"
#include "TextureManager.h"
#include "Core.h"

bool CTextureInputSampler::Init(int _iIndex, const string& _szName)
{
  CInputSampler::Init(_iIndex);
  m_pTexture = CORE->GetTextureManager()->GetResource(_szName);

  SetOk(true);
  return IsOk();
}

