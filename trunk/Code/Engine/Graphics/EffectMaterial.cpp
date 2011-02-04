#include "EffectMaterial.h"

#include "Texture.h"
#include "TextureManager.h"
#include "EffectTechnique.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include "VertexsStructs.h"

#include <fstream>

void CEffectMaterial::Release(void)
{
}


/*
bool CEffectMaterial::Init(bool _bAnimated, int _iTextureMask, int _iVertexType, const vector<CTexture*>& _vTextures)
{
  m_bAnimated = _bAnimated;
  m_iTextureMask = _iTextureMask;
  m_iVertexType  = _iVertexType;
  m_vTextures.clear();

  if(NumTextures(_iTextureMask) != _vTextures.size())
  {
    LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No hi ha el nombre de textures que hi hauria d'haver. Actual: %d, Esperat: %d", _vTextures.size(), NumTextures(_iTextureMask));
    assert(0 && "CEffectMaterial::Init No hi ha el nombre de textures que hi hauria d'haver.");
    SetOk(false);
    return false;
  }

  vector<CTexture*>::const_iterator l_end = _vTextures.cend();

  for(vector<CTexture*>::const_iterator l_it = _vTextures.cbegin(); l_it != l_end; ++l_it)
  {
    m_vTextures.push_back( *l_it );
  }

  SetOk(true);
  return true;
}
*/

bool CEffectMaterial::Init(std::fstream& _File)
{
  m_bAnimated = false;

  CTextureManager* l_pTextureManager = RENDER_MANAGER->GetTextureManager();
  //MaterialInfo
  _File.read((char*)&(m_usMaterialInfo), sizeof(uint16));

  //Vertex Type
  if(m_usMaterialInfo & MATERIAL_INFO_VERTEXTYPE)
  {
    _File.read((char*)&(m_usVertexType), sizeof(uint16));
  }
  //Textures
  uint16 l_usTextureNum = 0; 
  if(m_usMaterialInfo & MATERIAL_INFO_TEXTURETYPE)
  {
    _File.read((char*)&m_usTextureMask, sizeof(uint16));
    l_usTextureNum = NumTextures(m_usTextureMask);
  } else {
    l_usTextureNum = GetTextureNum(m_usVertexType);
  }

  for(int i = 0; i < l_usTextureNum; i++)
  {
    uint16 l_usTextLen = 0;
    _File.read((char*)&l_usTextLen, sizeof(uint16));
    char* l_pcTexture = new char[++l_usTextLen];
    
    _File.read(l_pcTexture, sizeof(char) * l_usTextLen);

    m_vTextures.push_back(l_pTextureManager->GetResource(l_pcTexture));
    if(m_vTextures[i] == 0)
    {
      delete l_pcTexture;
      //l_File.close();
      return false;
    }
    delete l_pcTexture;
  }

  SetOk(true);
  return true;
}

CEffectTechnique* CEffectMaterial::ActivateMaterial(const CRenderManager* _pRM) const
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  //Activar les textures
  vector<CTexture*>::const_iterator l_it = m_vTextures.cbegin();
  
  if(m_usTextureMask & TEXTURE_TYPE_DIFFUSSE)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha menys textures de les necessàries.");
    (*l_it)->Activate(0);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(0,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateMaterial error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_NORMALMAP)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha menys textures de les necessàries.");
    (*l_it)->Activate(1);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(1,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateMaterial error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_LIGHTMAN)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha menys textures de les necessàries.");
    (*l_it)->Activate(2);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(2,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateMaterial error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_ENVIRONMENT)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha menys textures de les necessàries.");
    (*l_it)->Activate(3);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(3,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateMaterial error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_GLOW)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha menys textures de les necessàries.");
    (*l_it)->Activate(4);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(4,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateMaterial error al directX");
  }

  assert(l_it == m_vTextures.cend() && "CEffectMaterial::ActivateMaterial hi ha més textures de les necessàries.");

  CEffectManager*   l_pEM = _pRM->GetEffectManager();
  CEffectTechnique* l_pForcedTechnique;
  if(m_bAnimated)
    l_pForcedTechnique = l_pEM->GetAnimatedModelTechnique();
  else
    l_pForcedTechnique = l_pEM->GetStaticMeshTechnique();

  if(l_pForcedTechnique)
    return l_pForcedTechnique;

  if(m_pTechnique)
    return m_pTechnique;

  string l_szName = l_pEM->GetTechniqueEffectNameByVertexDefault(m_usVertexType);
  return l_pEM->GetEffectTechnique(l_szName);
}

int CEffectMaterial::NumTextures(int mask)
{
  int l_count = 0;
  if(mask & TEXTURE_TYPE_DIFFUSSE)
    l_count++;
  if(mask & TEXTURE_TYPE_NORMALMAP)
    l_count++;
  if(mask & TEXTURE_TYPE_LIGHTMAN)
    l_count++;
  if(mask & TEXTURE_TYPE_ENVIRONMENT)
    l_count++;
  if(mask & TEXTURE_TYPE_GLOW)
    l_count++;
  return l_count;
}