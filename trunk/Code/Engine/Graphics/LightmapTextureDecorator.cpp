#include "LightmapTextureDecorator.h"
#include "Core.h"
#include "TextureManager.h"

int  CLightmapTextureDecorator::GetMaterialType() const 
{
  if(m_pTextureR)
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
  if(m_pTextureR)
  {
    m_pTextureR->Activate(7);
    m_pTextureG->Activate(8);
    m_pTextureB->Activate(9);
  }
  else
  {
    m_pTexture->Activate(2);
  }
  m_pDecoratedMaterial->Activate();
}

void CLightmapTextureDecorator::Activate(int _iTextureMask) 
{
  if(m_pTextureR)
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

void CLightmapTextureDecorator::ActivateRadiosityNormal()
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