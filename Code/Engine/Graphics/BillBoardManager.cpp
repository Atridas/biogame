#include "BillBoardManager.h"
#include "RenderManager.h"


CBillBoardManager::CBillBoardManager()
{
}


bool CBillBoardManager::Reload()
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

void CBillBoardManager::Release()
{
  vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                end = m_vBillboards.end();
  
  while(it != end)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_vBillboards.clear();
}

bool CBillBoardManager::Load(const string& _szFileName)
{


  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CBillBoardManager::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLBillboard;
  if(!l_XMLBillboard.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CBillBoardManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    SetOk(false);
    return false;
  }

	if(strcmp(l_XMLBillboard.GetName(),"Billboards") == 0)
  {
		    
		CXMLTreeNode l_treeInstanceBillboards = l_XMLBillboard["InstanceBillboard"];

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
 
			  
        Vect3f l_vVec3 = l_treeInstanceBillboard.GetVect3fProperty("position",Vect3f(0.0f),true);
			  l_pBillboard->SetPos(l_vVec3);
        l_pBillboard->SetId(l_treeInstanceBillboard.GetPszISOProperty("id" ,"",true));
       	float l_fSizeX = l_treeInstanceBillboard.GetFloatProperty("sizeX",1,true);
			  float l_fSizeY = l_treeInstanceBillboard.GetFloatProperty("sizeY",1,true);
        l_pBillboard->SetSizeX(l_fSizeX);
        l_pBillboard->SetSizeY(l_fSizeY);
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
                // el temps no fa falta si nomes te 1 animacio que es el que faig
                //float l_fTime = l_treeInstanceBillboardAnimated.GetFloatProperty("time",0,true);
			          float l_fTimeDiapo = l_treeInstanceBillboardAnimated.GetFloatProperty("TimeDiapo",1,true);
                l_pBillboard->SetTimeDiapo(l_fTimeDiapo);
			          // de moement nomes fare billboards amb 1 estat. es dir no cambia de textura en el temps
                //m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeInstanceBillboardAnimated.GetPszProperty("TexParticle","",true));
            			
			          int l_iTexNumFiles = l_treeInstanceBillboardAnimated.GetIntProperty("NumFiles",1,true);
                l_pBillboard->SetNumFiles(l_iTexNumFiles);
			          int l_iTexNumColumnes = l_treeInstanceBillboardAnimated.GetIntProperty("NumColumnes",1,true);
                l_pBillboard->SetColumnes(l_iTexNumColumnes);
              }
            }
			    }
        }
        m_vBillboards.push_back(l_pBillboard);
      }
    }
	
		SetOk(true);
	}
  return true;
}

CBillBoard* CBillBoardManager::GetParticleEmitter(const string& _szName)
{
  string l_szNameEmitter;
  vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                      end = m_vBillboards.end();

  while(it != end)
  {
    l_szNameEmitter=(*it)->GetId();
    if(_szName==l_szNameEmitter)
    {
      return (*it);
    }
    ++it;
  }
  LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager:: No existeix el emiter de tipus %s", _szName);
  return NULL;
}

void CBillBoardManager::Update(const float _fElapsedTime, CCamera* camera)
{
  vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                      end = m_vBillboards.end();


  while(it != end)
  {
    (*it)->Update(_fElapsedTime,camera);
    
    ++it;
  }
}

void CBillBoardManager::Render(CRenderManager* _pRM)
{
  Mat44f l_mat;
  l_mat.SetIdentity();

  _pRM->SetTransform(l_mat);
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

  _pRM->EnableAlphaBlend();
  l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  l_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  l_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  
  l_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  l_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  

  vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                      end = m_vBillboards.end();
  while(it != end)
  {
    (*it)->Render(_pRM);
    ++it;
  }
}

void CBillBoardManager::Init(CRenderManager* _pRM)
{
  vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                      end = m_vBillboards.end();
  while(it != end)
  {
    (*it)->Init(_pRM);
    ++it;
  }
}
