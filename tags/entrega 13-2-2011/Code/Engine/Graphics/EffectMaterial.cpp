#include "EffectMaterial.h"

#include "Texture.h"
#include "TextureManager.h"
#include "EffectTechnique.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include "VertexsStructs.h"
#include <XML/XMLTreeNode.h>

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



bool CEffectMaterial::Init(const CXMLTreeNode& _xmlMaterial)
{
  m_bAnimated = _xmlMaterial.GetBoolProperty("animated", false);

  m_usVertexType = _xmlMaterial.GetIntProperty("vertex_type", 787);

  m_usTextureMask = 0;


  string l_szTexDiffuse(""), l_szTexNormal(""), l_szTexLightmap(""), l_szTexEnvironment(""), l_szTexGlow("");
  int l_iNumChildren = _xmlMaterial.GetNumChildren();
  for(int i = 0; i < l_iNumChildren; i++)
  {
    if(strcmp(_xmlMaterial(i).GetName(),"texture") == 0)
    {
      CXMLTreeNode l_xmlTexture = _xmlMaterial(i);
      string l_szTexType = l_xmlTexture.GetPszISOProperty("type","");
      string l_szTexFilename = l_xmlTexture.GetPszISOProperty("filename", "");
      if(l_szTexType == "diffusse")
      {
        if(m_usTextureMask & TEXTURE_TYPE_DIFFUSSE)
          LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init textura difusa repetida, sobreescrivint. Antiga: \"%s\" Nova: \"%s\"", l_szTexDiffuse.c_str(), l_szTexFilename);
        m_usTextureMask |= TEXTURE_TYPE_DIFFUSSE;
        l_szTexDiffuse = l_szTexFilename;
      } else if(l_szTexType == "normalmap")
      {
        if(m_usTextureMask & TEXTURE_TYPE_NORMALMAP)
          LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init textura normalmap repetida, sobreescrivint. Antiga: \"%s\" Nova: \"%s\"", l_szTexNormal.c_str(), l_szTexFilename);
        m_usTextureMask |= TEXTURE_TYPE_NORMALMAP;
        l_szTexNormal = l_szTexFilename;
      } else if(l_szTexType == "lightmap")
      {
        if(m_usTextureMask & TEXTURE_TYPE_LIGHTMAP)
          LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init textura lightmap repetida, sobreescrivint. Antiga: \"%s\" Nova: \"%s\"", l_szTexLightmap.c_str(), l_szTexFilename);
        m_usTextureMask |= TEXTURE_TYPE_LIGHTMAP;
        l_szTexLightmap = l_szTexFilename;
      } else if(l_szTexType == "environment")
      {
        if(m_usTextureMask & TEXTURE_TYPE_ENVIRONMENT)
          LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init textura environment repetida, sobreescrivint. Antiga: \"%s\" Nova: \"%s\"", l_szTexEnvironment.c_str(), l_szTexFilename);
        m_usTextureMask |= TEXTURE_TYPE_ENVIRONMENT;
        l_szTexEnvironment = l_szTexFilename;
      } else if(l_szTexType == "glow")
      {
        if(m_usTextureMask & TEXTURE_TYPE_GLOW)
          LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init textura glow repetida, sobreescrivint. Antiga: \"%s\" Nova: \"%s\"", l_szTexGlow.c_str(), l_szTexFilename);
        m_usTextureMask |= TEXTURE_TYPE_GLOW;
        l_szTexGlow = l_szTexFilename;
      } else if(l_szTexType == "")
      {
        LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat el tipus de textura.");
      } else 
      {
        LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init Tipus de textura no reconegut \"%s\".", l_szTexType.c_str());
      }
    } else if(!_xmlMaterial(i).IsComment())
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init trobat un element desconegut \"%s\"", _xmlMaterial(i).GetName());
    }
  }
  CTextureManager* l_pTextureManager = RENDER_MANAGER->GetTextureManager();
  if(m_usTextureMask & TEXTURE_TYPE_DIFFUSSE)
  {
    CTexture* l_Texture = l_pTextureManager->GetResource(l_szTexDiffuse);
    if(!l_Texture)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat la textura \"%s\".", l_szTexDiffuse.c_str());
      m_usTextureMask &= ~TEXTURE_TYPE_DIFFUSSE;
    } else {
      m_vTextures.push_back(l_Texture);
    }
  }
  if(m_usTextureMask & TEXTURE_TYPE_NORMALMAP)
  {
    CTexture* l_Texture = l_pTextureManager->GetResource(l_szTexNormal);
    if(!l_Texture)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat la textura \"%s\".", l_szTexNormal.c_str());
      m_usTextureMask &= ~TEXTURE_TYPE_NORMALMAP;
    } else {
      m_vTextures.push_back(l_Texture);
    }
  }
  if(m_usTextureMask & TEXTURE_TYPE_LIGHTMAP)
  {
    CTexture* l_Texture = l_pTextureManager->GetResource(l_szTexLightmap);
    if(!l_Texture)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat la textura \"%s\".", l_szTexLightmap.c_str());
      m_usTextureMask &= ~TEXTURE_TYPE_LIGHTMAP;
    } else {
      m_vTextures.push_back(l_Texture);
    }
  }
  if(m_usTextureMask & TEXTURE_TYPE_ENVIRONMENT)
  {
    CTexture* l_Texture = l_pTextureManager->GetResource(l_szTexEnvironment);
    if(!l_Texture)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat la textura \"%s\".", l_szTexEnvironment.c_str());
      m_usTextureMask &= ~TEXTURE_TYPE_ENVIRONMENT;
    } else {
      m_vTextures.push_back(l_Texture);
    }
  }
  if(m_usTextureMask & TEXTURE_TYPE_GLOW)
  {
    CTexture* l_Texture = l_pTextureManager->GetResource(l_szTexGlow);
    if(!l_Texture)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CEffectMaterial::Init No s'ha trobat la textura \"%s\".", l_szTexGlow.c_str());
      m_usTextureMask &= ~TEXTURE_TYPE_GLOW;
    } else {
      m_vTextures.push_back(l_Texture);
    }
  }
  SetOk(true);
  return true;
}

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
    if(m_usVertexType & (VERTEX_TYPE_BLENDIDX | VERTEX_TYPE_BLENDWGT))
    {
      m_bAnimated = true;
    }
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

void CEffectMaterial::ActivateTextures(const CRenderManager* _pRM) const
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

  //Activar les textures
  vector<CTexture*>::const_iterator l_it = m_vTextures.cbegin();
  
  if(m_usTextureMask & TEXTURE_TYPE_DIFFUSSE)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha menys textures de les necessàries.");
    (*l_it)->Activate(0);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(0,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateTextures error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_NORMALMAP)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha menys textures de les necessàries.");
    (*l_it)->Activate(1);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(1,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateTextures error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_LIGHTMAP)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha menys textures de les necessàries.");
    (*l_it)->Activate(2);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(2,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateTextures error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_ENVIRONMENT)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha menys textures de les necessàries.");
    (*l_it)->Activate(3);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(3,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateTextures error al directX");
  }

  if(m_usTextureMask & TEXTURE_TYPE_GLOW)
  {
    assert(l_it != m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha menys textures de les necessàries.");
    (*l_it)->Activate(4);
    ++l_it;
  } else {
    HRESULT l_Result = l_pDevice->SetTexture(4,0);
    assert(SUCCEEDED(l_Result) && "CEffectMaterial::ActivateTextures error al directX");
  }

  assert(l_it == m_vTextures.cend() && "CEffectMaterial::ActivateTextures hi ha més textures de les necessàries.");
}

CEffectTechnique* CEffectMaterial::GetEffectTechnique(const CRenderManager* _pRM) const
{
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
  if(mask & TEXTURE_TYPE_LIGHTMAP)
    l_count++;
  if(mask & TEXTURE_TYPE_ENVIRONMENT)
    l_count++;
  if(mask & TEXTURE_TYPE_GLOW)
    l_count++;
  return l_count;
}