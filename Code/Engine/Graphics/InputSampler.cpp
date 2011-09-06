#include "InputSampler.h"
#include "Texture.h"

void CInputSampler::Activate()
{
  m_pTexture->Activate(m_iIndex);

}