#include "ExplosionsManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "RenderManager.h"


CExplosionsManager::CExplosionsManager()
{
  m_szFileName = "";
}


bool CExplosionsManager::Load(const string& _szFileName)
{
  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CExplosionsManager::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLExplosions;
  if(!l_XMLExplosions.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CExplosionsManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    SetOk(false);
    return false;
  }
  if(strcmp(l_XMLExplosions.GetName(),"Explosions") == 0)
  {
		CXMLTreeNode l_treeExplosionsCharacteristics = l_XMLExplosions["ExplosionsCharacteristics"];

    if(l_treeExplosionsCharacteristics.Exists())
    {
      int l_iNumChildren = l_treeExplosionsCharacteristics.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CExplosionsManager::Load Loading %d ExplosionsCharacteristics.", l_iNumChildren);

      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_treeExplosionCharacteristic = l_treeExplosionsCharacteristics(i);
        if(l_treeExplosionCharacteristic.IsComment())
				  continue;
      
        SExplosionInfo* l_pInfo= new SExplosionInfo;

        l_pInfo->m_szId = l_treeExplosionCharacteristic.GetPszISOProperty("id" ,"");
        l_pInfo->m_SizeX = l_treeExplosionCharacteristic.GetFloatProperty("SizeX");
        l_pInfo->m_SizeY = l_treeExplosionCharacteristic.GetFloatProperty("SizeY");
        l_pInfo->m_iTexNumFiles = l_treeExplosionCharacteristic.GetIntProperty("NumFiles");
        l_pInfo->m_iTexNumColumnes = l_treeExplosionCharacteristic.GetIntProperty("NumColumnes");
        l_pInfo->m_fTimeAnimationDiapo = l_treeExplosionCharacteristic.GetFloatProperty("TimeDiapo");
        l_pInfo->m_pTexExplosion = CORE->GetTextureManager()->GetResource(l_treeExplosionCharacteristic.GetPszProperty("TexExplosion",""));


        AddResource(l_pInfo->m_szId,l_pInfo);
      }
    }

    CXMLTreeNode l_treeInstanceExplosions = l_XMLExplosions["InstanceExplosions"];

    if(l_treeInstanceExplosions.Exists())
    {
      int l_iNumChildren = l_treeInstanceExplosions.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CExplosionsManager::Load Loading %d InstanceExplosions.", l_iNumChildren);

      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_treeInstanceExplosion = l_treeInstanceExplosions(i);
        if(l_treeInstanceExplosion.IsComment())
				  continue;
      
        CExplosion* l_pExplosion = new CExplosion();
			  
			  string l_szType = l_treeInstanceExplosion.GetPszProperty("type","");

        // s'ha de fer que la Id de la SInfo correspongui amb el type de la instancia
			  SExplosionInfo* l_pInfo = GetResource(l_szType);

        if(!l_pInfo)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CExplosionManager:: No existeix l'explosio de tipus %s", l_szType);
          continue;
        }

			  l_pExplosion->SetPosition(l_treeInstanceExplosion.GetVect3fProperty("Position",Vect3f(0.0f)));
        l_pExplosion->SetName(l_treeInstanceExplosion.GetPszISOProperty("id" ,""));
        l_pExplosion->SetAttributes(l_pInfo);

			 // m_vExplosions.push_back(l_pExplosion);
      }
    }
    SetOk(true);
  }
       
  return true;
}
  
void CExplosionsManager::Update(const float _fElapsedTime, CCamera* camera)
{

}
  
void CExplosionsManager::Render(CRenderManager* _pRM)
{

}
  
void CExplosionsManager::Init(CRenderManager* _pRM)
{

}
  
bool CExplosionsManager::Reload()
{
  Release();
	bool isOk = false;
  isOk = Load(m_szFileName);
  if (isOk)
  {
    Init(CORE->GetRenderManager());
  }
  return isOk;

}
  
void CExplosionsManager::Release()
{

}


