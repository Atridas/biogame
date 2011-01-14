#include "EffectManager.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include <XML/XMLTreeNode.h>


void CEffectManager::ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye)
{

}

/* format inventat de Effects.xml:
<shaders>
  <effects>
    <effect name=".../>
    <effect name=".../>
  </effects>
  <default_technique .../>
  <techniques>
    <technique name=".../>
    <technique name=".../>
    <technique name=".../>
  </techniques>
</shaders>
*/
bool CEffectManager::Load(const string& _szFileName)
{
  m_szFileName = _szFileName;
  return Load();
}

bool CEffectManager::Load()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Load Carregant el fitxer \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_treeShaders;
  if(!l_treeShaders.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CEffectManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  if(strcmp(l_treeShaders.GetName(),"shaders") == 0)
  {

    //--------Effects-------------
    CXMLTreeNode l_treeEffects = l_treeShaders["effects"];
    int l_iNumChildren = l_treeEffects.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeEffect = l_treeEffects(i);
      if(l_treeEffect.IsComment())
        continue;
      
      CEffect* l_pEffect = new CEffect();
      l_pEffect->Init(l_treeEffect);

      m_Effects.AddResource(l_pEffect->GetName(),l_pEffect);
    }
    //----------------------------

    //TODO: default techniques

    //--------Techniques-----------
    CXMLTreeNode l_treeTechniques = l_treeShaders["techniques"];
    l_iNumChildren = l_treeTechniques.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeTechnique = l_treeTechniques(i);
      if(l_treeTechnique.IsComment())
        continue;

      CEffectTechnique* l_pTechnique = new CEffectTechnique();
      l_pTechnique->Init(l_treeTechnique);

      AddResource(l_pTechnique->GetName(),l_pTechnique);
    }
    //-----------------------------
  }

  return true;
}

void CEffectManager::Reload()
{
  Release();
  Load();
}

void CEffectManager::Release()
{
  CMapManager::Release();

  //map
  m_DefaultTechniqueEffectMap.clear();
  //mapmanager
  m_Effects.Release();

  //pointers
  CHECKED_DELETE(m_pStaticMeshTechnique);
  CHECKED_DELETE(m_pAnimatedModelTechnique);
}