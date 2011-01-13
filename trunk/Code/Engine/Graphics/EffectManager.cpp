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
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Load Carregant el fitxer \"%s\"", _szFileName.c_str());

  CXMLTreeNode l_treeEffects;
  if(l_treeEffects.LoadFile(_szFileName.c_str()))
  {



  }
 /*   if(strcmp(l_treeEffects.GetName(),"animatedModel") == 0)
    {

    int l_iNumChildren = l_treeEffects.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeAnimatedModel = l_treeAnimatedModels(i);
      if(strcmp(l_treeAnimatedModel.GetName(),"animatedModel") == 0)
      {
        const char* l_pcPath = l_treeAnimatedModel.GetPszProperty("path",0);
        const char* l_pcName = l_treeAnimatedModel.GetPszProperty("name",0);

        if(l_pcPath == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat la propietat \"path\" a una animatedModel.");
        } else if(l_pcName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat la propietat \"name\" a una animatedModel.");
        } else {
          GetCore(l_pcName, l_pcPath);
        }
      } else if(!l_treeAnimatedModel.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load S'ha trobat un element desconegut \"%s\"", l_treeAnimatedModel.GetName());
      }
    }
  } else {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedModelManager::Load No s'ha trobat el fitxer");
  }
  SetOk(true);*/

  return IsOk();
}

void CEffectManager::Reload()
{

}

CEffect* CEffectManager::GetEffect(const string& _szName)
{
  return 0;
}

CEffectTechnique* CEffectManager::GetEffectTechnique(const string& _szName)
{
  return 0;
}

void CEffectManager::Release()
{
  //map
  m_DefaultTechniqueEffectMap.clear();
  //mapmanager
  m_Effects.Release();

  //pointers
  CHECKED_DELETE(m_pStaticMeshTechnique);
  CHECKED_DELETE(m_pAnimatedModelTechnique);
}