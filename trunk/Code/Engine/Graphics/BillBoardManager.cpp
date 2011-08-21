#include "BillBoardManager.h"
#include "RenderManager.h"
#include "Particle.h"
#include "Effect.h"
#include "EffectManager.h"

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
  map<string,CBillBoard*>::iterator it  = m_Billboards.begin(),
                                    end = m_Billboards.end();
  
  while(it != end)
  {
    CHECKED_DELETE(it->second);
    ++it;
  }
  m_Billboards.clear();
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
        bool l_bBucleInfinit = l_treeInstanceBillboard.GetBoolProperty("Bucle",true,false);
        l_pBillboard->SetBucleInfinit(l_bBucleInfinit);
        /*if(!l_bBucleInfinit)
        {
          float l_fTimeLife = l_treeInstanceBillboard.GetFloatProperty("Life",1,false);
          l_pBillboard->SetTimeLife(l_fTimeLife);
        }*/
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
        m_Billboards[l_pBillboard->GetId()] = l_pBillboard;
      }
    }
	
		SetOk(true);
	}
  return true;
}

CBillBoard* CBillBoardManager::GetBillBoard(const string& _szName)
{
  string l_szNameEmitter;
  /*vector<CBillBoard*>::iterator it  = m_vBillboards.begin(),
                                      end = m_vBillboards.end();

  while(it != end)
  {
    l_szNameEmitter=(*it)->GetId();
    if(_szName==l_szNameEmitter)
    {
      return (*it);
    }
    ++it;
  }*/

  map<string,CBillBoard*>::iterator it  = m_Billboards.find(_szName);
  if(it != m_Billboards.end())
  {
    return it->second;
  }

  LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager:: No existeix el emiter de tipus %s", _szName);
  return NULL;
}


void CBillBoardManager::SetAllBillboardsActive(bool _bActive)
{
  map<string,CBillBoard*>::iterator it  = m_Billboards.begin(),
                                    end = m_Billboards.end();
  
  while(it != end)
  {
    it->second->SetActive(_bActive);
    ++it;
  }
}

void CBillBoardManager::Update(const float _fElapsedTime, CCamera* camera)
{
  map<string,CBillBoard*>::iterator it  = m_Billboards.begin(),
                                    end = m_Billboards.end();


  while(it != end)
  {
    it->second->Update(_fElapsedTime,camera);
    
    ++it;
  }
}

void CBillBoardManager::Render(CRenderManager* _pRM)
{
  Mat44f l_mat;
  l_mat.SetIdentity();

  _pRM->SetTransform(l_mat);
  
#ifdef __PARTICLE_VIA_SHADER__
  CEffectManager* l_pEM = CORE->GetEffectManager();

  assert(l_pEM && l_pEM->IsOk());
  CEffect* l_pEffect = l_pEM->GetEffect("Particle");
  CEffect* l_pPrevEffect = l_pEM->GetForcedStaticMeshEffect();
  l_pEM->SetForcedStaticMeshEffect(l_pEffect);

  //l_pd3dDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));
#else
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

  _pRM->EnableAlphaBlend();
  l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  l_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  l_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  
  l_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  l_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
#endif

  map<string,CBillBoard*>::iterator it  = m_Billboards.begin(),
                                    end = m_Billboards.end();
  while(it != end)
  {
    it->second->Render(_pRM);
    ++it;
  }
  
#ifdef __PARTICLE_VIA_SHADER__
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();
  l_pd3dDevice->SetStreamSourceFreq(0, 1);
  l_pd3dDevice->SetStreamSourceFreq(1, 1);
  l_pEM->SetForcedStaticMeshEffect(l_pPrevEffect);
#endif
}

void CBillBoardManager::Init(CRenderManager* _pRM)
{
  map<string,CBillBoard*>::iterator it  = m_Billboards.begin(),
                                    end = m_Billboards.end();
  while(it != end)
  {
    it->second->Init(_pRM);
    ++it;
  }
}
