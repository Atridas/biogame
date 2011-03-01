#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include <XML/XMLTreeNode.h>


void CEffectManager::ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye)
{
  m_mProjectionMatrix=_mProjectionMatrix;
  m_mViewMatrix=_mViewMatrix;
  m_vCameraEye=_vCameraEye;
  
  m_bInverseProjectionUpdated = m_bInverseViewUpdated = m_bInverseWorldUpdated = m_bViewProjectionUpdated = m_bWorldViewUpdated = m_bWorldViewProjectionUpdated = false;
}

/* format inventat de Effects.xml:
<shaders>
  <effects>
    <effect name=".../>
    <effect name=".../>
  </effects>
  <default_techniques>
    <default_technique .../>
    <default_technique .../>
    <default_technique .../>
  </default_techniques>
  <techniques>
    <technique name=".../>
    <technique name=".../>
    <technique name=".../>
  </techniques>
</shaders>
*/

bool CEffectManager::Load(const SEffectManagerParams& _params)
{
  m_szFileName = _params.szFile;
  return Load(false);
}

bool CEffectManager::Load(bool _bReload)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Load Carregant el fitxer \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_treeShaders;
  if(!l_treeShaders.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CEffectManager::Load No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  if(strcmp(l_treeShaders.GetName(),"shaders") == 0)
  {

    //--------Effects-------------
    CXMLTreeNode l_treeEffects = l_treeShaders["effects"];
    int l_iNumChildren = l_treeEffects.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeEffect = l_treeEffects(i);
      if(l_treeEffect.IsComment())
        continue;
      

      CEffect* l_pEffect = 0;

      string l_szEffectName = l_treeEffect.GetPszISOProperty("name","");

      l_pEffect = m_Effects.GetResource(l_szEffectName);

      if(l_pEffect)
      {
        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Reloading effect \"%s\"", l_szEffectName.c_str());
        }

        l_pEffect->Reload();

      } else {

        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Found new effect during reload: \"%s\"", l_szEffectName.c_str());
        }

        l_pEffect = new CEffect();

        l_pEffect->Init(l_treeEffect);

        m_Effects.AddResource(l_pEffect->GetName(),l_pEffect);
      }

    }
    //----------------------------

    //--------Default techniques-------------
    CXMLTreeNode l_treeDefTechniques = l_treeShaders["default_techniques"];
    l_iNumChildren = l_treeDefTechniques.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d default_techniques.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeDefTechnique = l_treeDefTechniques(i);
      if(l_treeDefTechnique.IsComment())
        continue;
      
      int l_iVertexType = l_treeDefTechnique.GetIntProperty("vertex_type");
      string l_szTechniqueName = l_treeDefTechnique.GetPszISOProperty("technique","");

      if(l_iVertexType == 0)
        LOGGER->AddNewLog(ELL_WARNING,"CEffectManager::Load Default technique \"%s\" amb vertex_type = 0.", l_szTechniqueName.c_str());

      if(l_treeDefTechnique.GetBoolProperty("instanced", false))
      {
        m_DefaultInstancedTechniqueEffectMap[l_iVertexType] = l_szTechniqueName;
      } else {
        m_DefaultTechniqueEffectMap[l_iVertexType] = l_szTechniqueName;
      }
    }
    //---------------------------------------

    //--------Techniques-----------
    CXMLTreeNode l_treeTechniques = l_treeShaders["techniques"];
    l_iNumChildren = l_treeTechniques.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d techniques.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeTechnique = l_treeTechniques(i);
      if(l_treeTechnique.IsComment())
        continue;

      string l_szTechniqueName = l_treeTechnique.GetPszISOProperty("name","");

      CEffectTechnique* l_pTechnique = GetResource(l_szTechniqueName);

      if(l_pTechnique)
      {
        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Reloading technique \"%s\"", l_szTechniqueName.c_str());
        }

        l_pTechnique->Init(l_treeTechnique);

        l_pTechnique->Refresh();

      } else {
        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Found new technique during reload: \"%s\"", l_szTechniqueName.c_str());
        }

        l_pTechnique = new CEffectTechnique();

        l_pTechnique->Init(l_treeTechnique);

        AddResource(l_pTechnique->GetName(),l_pTechnique);
      }

    }
    //-----------------------------
  }

  return true;
}

void CEffectManager::Reload()
{
  Load(true);
}

void CEffectManager::Release()
{
  CMapManager::Release();

  //map
  m_DefaultTechniqueEffectMap.clear();
  //mapmanager
  m_Effects.Release();

  //pointers
  //CHECKED_DELETE(m_pStaticMeshTechnique);
  //CHECKED_DELETE(m_pAnimatedModelTechnique);
}

string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short _sVertexType) const
{
  TDefaultTechniqueEffectMap::const_iterator l_it = m_DefaultTechniqueEffectMap.find((int)_sVertexType);
  if(l_it != m_DefaultTechniqueEffectMap.cend())
    return l_it->second;

  return "";
}

string CEffectManager::GetInstancedTechniqueEffectNameByVertexDefault(unsigned short _sVertexType) const
{
  TDefaultTechniqueEffectMap::const_iterator l_it = m_DefaultInstancedTechniqueEffectMap.find((int)_sVertexType);
  if(l_it != m_DefaultInstancedTechniqueEffectMap.cend())
    return l_it->second;

  return "";
}


const Mat44f& CEffectManager::GetInverseProjectionMatrix()
{
  if(!m_bInverseProjectionUpdated)
  {
    m_mInverseProjectionMatrix = m_mProjectionMatrix.GetInverted();
    m_bInverseProjectionUpdated = true;
  }
  return m_mInverseProjectionMatrix;
}

const Mat44f& CEffectManager::GetInverseViewMatrix()
{
  if(!m_bInverseViewUpdated)
  {
    m_mInverseViewMatrix = m_mViewMatrix.GetInverted();
    m_bInverseViewUpdated = true;
  }
  return m_mInverseViewMatrix;
}

const Mat44f& CEffectManager::GetInverseWorldMatrix()
{
  if(!m_bInverseWorldUpdated)
  {
    m_mInverseWorldMatrix = m_mWorldMatrix.GetInverted();
    m_bInverseWorldUpdated = true;
  }
  return m_mInverseWorldMatrix;
}

const Mat44f& CEffectManager::GetViewProjectionMatrix()
{
  if(!m_bViewProjectionUpdated)
  {
    m_mViewProjectionMatrix = m_mProjectionMatrix * m_mViewMatrix;
    m_bViewProjectionUpdated = true;
  }
  return m_mViewProjectionMatrix;
}

const Mat44f& CEffectManager::GetWorldViewMatrix()
{
  if(!m_bWorldViewUpdated)
  {
    m_mWorldViewMatrix = m_mViewMatrix * m_mWorldMatrix;
    m_bWorldViewUpdated = true;
  }
  return m_mWorldViewMatrix;
}

const Mat44f& CEffectManager::GetWorldViewProjectionMatrix()
{
  if(!m_bInverseProjectionUpdated)
  {
    m_mWorldViewProjectionMatrix = m_mProjectionMatrix * m_mViewMatrix * m_mWorldMatrix;
    m_bWorldViewProjectionUpdated = true;
  }
  return m_mWorldViewProjectionMatrix;
}

