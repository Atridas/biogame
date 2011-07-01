#include "LightmapTextureDecorator.h"
#include "Core.h"
#include "TextureManager.h"

int  CLightmapTextureDecorator::GetMaterialType() const 
{
  if(m_eMode == RADIOSITY_NORMAL)
  {
    return RADIOSITY_NORMAL_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();
  }
  else
  {
    return LIGHTMAP_MATERIAL_MASK | m_pDecoratedMaterial->GetMaterialType();
  }
};

void CLightmapTextureDecorator::Activate() 
{
  switch(m_eMode)
  {
  case FLAT:
    m_pTexture->Activate(2);
    break;
  case RADIOSITY_NORMAL:
    m_pTextureR->Activate(7);
    m_pTextureG->Activate(8);
    m_pTextureB->Activate(9);
    break;
  case SHOW_R:
    m_pTextureR->Activate(2);
    break;
  case SHOW_G:
    m_pTextureG->Activate(2);
    break;
  case SHOW_B:
    m_pTextureB->Activate(2);
    break;
  }
  m_pDecoratedMaterial->Activate();
}

void CLightmapTextureDecorator::Activate(int _iTextureMask) 
{
  if(m_eMode == RADIOSITY_NORMAL)
  {
    if(_iTextureMask & RADIOSITY_NORMAL_MATERIAL_MASK)
      Activate();
    else
      m_pDecoratedMaterial->Activate();
  }
  else
  {
    if(_iTextureMask & LIGHTMAP_MATERIAL_MASK)
      Activate();
    else
      m_pDecoratedMaterial->Activate();
  }
}

void CLightmapTextureDecorator::ActivateRadiosityNormal(ELightmapMode _eMode)
{
  m_eMode = _eMode;
  if(!m_pTextureR && m_eMode != FLAT)
  {
    string l_szFilename = m_pTexture->GetFileName();

    int l_cPunt = l_szFilename.find_last_of('.');
  
    string l_szFilenamePrincipi = l_szFilename.substr(0, l_cPunt);
    string l_szFilenameFinal = l_szFilename.substr(l_cPunt, l_szFilename.size() - l_cPunt);
  
    string l_szR = l_szFilenamePrincipi + "_R" + l_szFilenameFinal;
    string l_szG = l_szFilenamePrincipi + "_G" + l_szFilenameFinal;
    string l_szB = l_szFilenamePrincipi + "_B" + l_szFilenameFinal;

    m_pTextureR = CORE->GetTextureManager()->GetResource(l_szR);
    m_pTextureG = CORE->GetTextureManager()->GetResource(l_szG);
    m_pTextureB = CORE->GetTextureManager()->GetResource(l_szB);
  }
}