#include "BillBoardCore.h"
#include "RenderManager.h"
#include "Particle.h"
#include "Effect.h"
#include "EffectManager.h"

CBillBoardCore::CBillBoardCore()
{
	m_szFileName="";
	m_bReload=false;
	m_pTexParticle= NULL;
}

void CBillBoardCore::Release()
{
  map<string,CBillBoard*>::iterator it  = m_BillboardsCore.begin(),
                                    end = m_BillboardsCore.end();
  
  while(it != end)
  {
    CHECKED_DELETE(it->second);
    ++it;
  }
  m_BillboardsCore.clear();
}

bool CBillBoardCore::Load(const string& _szFileName)
{


  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CBillBoardCore::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLBillboard;
  if(!l_XMLBillboard.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CBillBoardCore:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    SetOk(false);
    return false;
  }

	if(strcmp(l_XMLBillboard.GetName(),"BillboardsCore") == 0)
  {
		    
		CXMLTreeNode l_treeInstanceBillboards = l_XMLBillboard["CoreBillboard"];

    if(l_treeInstanceBillboards.Exists())
    {
      int l_iNumChildrenInstances = l_treeInstanceBillboards.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CBillboardManager::Load Loading %d billboards.", l_iNumChildrenInstances);

      for(int i = 0; i < l_iNumChildrenInstances; i++)
      {
        CXMLTreeNode l_treeInstanceBillboard = l_treeInstanceBillboards(i);

        if(l_treeInstanceBillboard.IsComment())
				  continue;

        CBillBoard* l_pBillboard = new CBillBoard();
 
			  
        l_pBillboard->SetType(l_treeInstanceBillboard.GetPszISOProperty("type" ,"",true));
       	float l_fSizeX = l_treeInstanceBillboard.GetFloatProperty("sizeX",1,true);
			  float l_fSizeY = l_treeInstanceBillboard.GetFloatProperty("sizeY",1,true);
        l_pBillboard->SetSizeX(l_fSizeX);
        l_pBillboard->SetSizeY(l_fSizeY);
        bool l_bBucleInfinit = l_treeInstanceBillboard.GetBoolProperty("Bucle",true,false);
        l_pBillboard->SetBucleInfinit(l_bBucleInfinit);
        m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeInstanceBillboard.GetPszProperty("TexParticle","",true));
        l_pBillboard->SetTexture(m_pTexParticle);
       
        int Animations = l_treeInstanceBillboard.GetNumChildren();
      
		    for(int k=0;k<Animations;++k)
		    {
			    const char *l_Name=l_treeInstanceBillboard(k).GetName();
			    if(strcmp(l_Name,"Animateds")==0)
			    {
				    CXMLTreeNode l_treeInstanceBillboardAnimateds = l_treeInstanceBillboard["Animateds"];

            if(l_treeInstanceBillboardAnimateds.Exists())
            {
              int l_iNumChildrenAnimateds = l_treeInstanceBillboardAnimateds.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CBillboardManager::Load Loading %d BillboardAnimated.", l_iNumChildrenAnimateds);

              for(int j = 0; j < l_iNumChildrenAnimateds; j++)
              {
            
                CXMLTreeNode l_treeInstanceBillboardAnimated = l_treeInstanceBillboardAnimateds(j);
                if(l_treeInstanceBillboardAnimated.IsComment())
				        continue;

            
			          bool l_bAnimated = true;
                l_pBillboard->SetAnimated(l_bAnimated);
                float l_fTimeDiapo = l_treeInstanceBillboardAnimated.GetFloatProperty("TimeDiapo",1,true);
                l_pBillboard->SetTimeDiapo(l_fTimeDiapo);
			          int l_iTexNumFiles = l_treeInstanceBillboardAnimated.GetIntProperty("NumFiles",1,true);
                l_pBillboard->SetNumFiles(l_iTexNumFiles);
			          int l_iTexNumColumnes = l_treeInstanceBillboardAnimated.GetIntProperty("NumColumnes",1,true);
                l_pBillboard->SetColumnes(l_iTexNumColumnes);
                
              }
            }
			    }
        }
        m_BillboardsCore[l_pBillboard->GetType()] = l_pBillboard;
        
      }
    }
	
		SetOk(true);
	}
  return true;
}

CBillBoard* CBillBoardCore::GetBillBoard(const string& _szName)
{
  string l_szNameEmitter;
  map<string,CBillBoard*>::iterator it  = m_BillboardsCore.find(_szName);
  if(it != m_BillboardsCore.end())
  {
    return it->second;
  }

  LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager:: No existeix el emiter de tipus %s", _szName);
  return NULL;
}


