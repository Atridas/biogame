#include "BillBoardManager.h"
#include "RenderManager.h"
#include "Particle.h"
#include "Effect.h"
#include "EffectManager.h"
#include "BillBoardCore.h"

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
  
  //això és una puta merda
  //while(it != end)
  //{
  //  CHECKED_DELETE(it->second);
  //  ++it;
  //}
  m_Billboards.clear();

  CHECKED_DELETE(m_pBillboardCore);
}

bool CBillBoardManager::Load(const string& _szFileName)
{

  m_pBillboardCore = new CBillBoardCore();
  m_pBillboardCore->Load("./Data/XML/BillboardCore.xml");
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

        
 
		    string l_szType = l_treeInstanceBillboard.GetPszISOProperty("type" ,"",true);
		    
        CBillBoard* l_pBillboard = m_pBillboardCore->GetBillBoard(l_szType);
        if(!l_pBillboard)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CBillboardManagerManager:: No existeix de tipus Billboard seleccionat %s", l_szType.c_str());
          continue;
        }
        Vect3f l_vVec3 = l_treeInstanceBillboard.GetVect3fProperty("position",Vect3f(0.0f),true);
			  l_pBillboard->SetPos(l_vVec3);
        l_pBillboard->SetId(l_treeInstanceBillboard.GetPszISOProperty("id" ,"",true));
       	
        
        m_Billboards[l_pBillboard->GetId()] = l_pBillboard;
      }
    }
	  
		SetOk(true);
  }
  
  return true;
}

CBillBoard* CBillBoardManager::CreateBillBorad(const string& _szName, const string& _szId, const Vect3f& _vPosition)
{
 
  map<string,CBillBoard*>::iterator it  = m_Billboards.find(_szId);
  if(it != m_Billboards.end())
  {
    CBillBoard* l_pBillboard = it->second;
	  l_pBillboard->SetPos(_vPosition);
	  l_pBillboard->SetId(_szName);
    //m_Billboards[l_pBillboard->GetId()] = l_pBillboard;
    return l_pBillboard;
	
  }else
  {
    LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager:: No existeix el emiter de tipus %s", _szName.c_str());
    return NULL;
  }
}

CBillBoard* CBillBoardManager::GetBillBoard(const string& _szName)
{
  string l_szNameEmitter;
 
  map<string,CBillBoard*>::iterator it  = m_Billboards.find(_szName);
  if(it != m_Billboards.end())
  {
    return it->second;
  }

  LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager:: No existeix el emiter de tipus %s", _szName.c_str());
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
