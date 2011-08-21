#include "ParticleManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "IndexedVertexs.h"
#include "VertexsStructs.h"
#include "EffectManager.h"
#include "ParticleCore.h"

CParticleManager::CParticleManager():
m_pParticleVertex(0)
{
  m_iNumDirections=1;
  m_szFileName = "";
  m_bReload = false;
}

bool CParticleManager::Reload()
{
  Release();
	bool isOk = false;
  isOk = Load(m_szFileName);
  if (isOk)
  {
    Init(CORE->GetRenderManager());
  }
  SetAllEmittersActive(true);
  return isOk;
}

void CParticleManager::Release()
{
  //Cridar el release del pare
  CMapManager::Release();

  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();
  
  while(it != end)
  {
              CHECKED_DELETE(*it);
    ++it;
  }
  
  m_vEmitterParticle.clear();
  m_vDirection.clear();
  m_vTimeDirection.clear();
  m_vTimeDirectionInterpolation.clear();

  CHECKED_DELETE(m_pParticleVertex);
}


bool CParticleManager::Load(const string& _szFileName)
{


  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CParticleManager::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLParticles;
  if(!l_XMLParticles.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CParticleManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    SetOk(false);
    return false;
  }

	if(strcmp(l_XMLParticles.GetName(),"Particles") == 0)
  {
		    
		CXMLTreeNode l_treeInstanceParticles = l_XMLParticles["InstanceParticles"];

    if(l_treeInstanceParticles.Exists())
    {
      int l_iNumChildrenInstances = l_treeInstanceParticles.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d particules.", l_iNumChildrenInstances);

      for(int i = 0; i < l_iNumChildrenInstances; i++)
      {
        CXMLTreeNode l_treeInstanceParticle = l_treeInstanceParticles(i);

        if(l_treeInstanceParticle.IsComment())
				  continue;

        CParticleEmitter* l_pParticleEmitter = new CParticleEmitter();
 
			  string l_szType = l_treeInstanceParticle.GetPszProperty("type","",true);
        Vect3f l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("position",Vect3f(0.0f),true);
			  l_pParticleEmitter->SetPosition(l_vVec3);
        l_pParticleEmitter->SetName(l_treeInstanceParticle.GetPszISOProperty("id" ,"",true));
        Vect3f l_vSpawnDir = l_treeInstanceParticle.GetVect3fProperty("Direction",Vect3f(0,1,0),true);
		    Vect3f l_vDesviacionSpawnDir = l_treeInstanceParticle.GetVect3fProperty("Desviacion",Vect3f(0.0f),false);
		
		    m_szFormEmitter = l_treeInstanceParticle.GetPszISOProperty("FormEmitter","dummy",false);
        l_pParticleEmitter->SetFormEmitter(m_szFormEmitter);

   
		    if(m_szFormEmitter=="dummy")
		    {
			    m_fSizeX = l_treeInstanceParticle.GetFloatProperty("sizeX",1,true);
			    m_fSizeY = l_treeInstanceParticle.GetFloatProperty("sizeY",1,true);
			    m_fSizeZ = l_treeInstanceParticle.GetFloatProperty("sizeZ",1,true);
          Vect3f l_vPosFormEmitter(m_fSizeX, m_fSizeY, m_fSizeZ);

			    l_pParticleEmitter->SetPositionFormEmitter(l_vPosFormEmitter);
			    l_pParticleEmitter->SetFormEmitter(m_szFormEmitter);
		    }

        l_pParticleEmitter->m_vTimeDirection.push_back(0);
        Vect3f l_vAux = l_vSpawnDir- l_vDesviacionSpawnDir;
        l_pParticleEmitter->m_vDirection.push_back(l_vAux);
        l_vAux = l_vSpawnDir + l_vDesviacionSpawnDir;
        l_pParticleEmitter->m_vDirection.push_back(l_vAux);

        int numDirections = l_treeInstanceParticle.GetNumChildren();
      
		    for(int k=0;k<numDirections;++k)
		    {
			    const char *l_Name=l_treeInstanceParticle(k).GetName();
			    if(strcmp(l_Name,"Directions")==0)
			    {

            CXMLTreeNode l_treeParticleInstanceDiredtions = l_treeInstanceParticle["Directions"];

            if(l_treeParticleInstanceDiredtions.Exists())
            {
              int l_iNumChildrenDirections = l_treeParticleInstanceDiredtions.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmittersDirections.", l_iNumChildrenDirections);

              for(int j = 0; j < l_iNumChildrenDirections; j++)
              {
          
                CXMLTreeNode l_treeParticleInstanceDiredtion = l_treeParticleInstanceDiredtions(j);
                if(l_treeParticleInstanceDiredtion.IsComment())
				        continue;

         

                m_fTime = l_treeParticleInstanceDiredtion.GetFloatProperty("time",1,true);
                m_fTimeInterpolation = l_treeParticleInstanceDiredtion.GetFloatProperty("timeInterpolation",0,false);
            
			
			          l_vSpawnDir = l_treeParticleInstanceDiredtion.GetVect3fProperty("Direction",Vect3f(0,1,0),true);
			          l_vDesviacionSpawnDir = l_treeParticleInstanceDiredtion.GetVect3fProperty("Desviacion",Vect3f(0.0f),false);
            
            
		            Vect3f l_vAux = l_vSpawnDir - l_vDesviacionSpawnDir;
		           l_pParticleEmitter->m_vTimeDirection.push_back(m_fTime);
               l_pParticleEmitter->m_vDirection.push_back(l_vAux);
               l_vAux = l_vSpawnDir+ l_vDesviacionSpawnDir;
               l_pParticleEmitter->m_vDirection.push_back(l_vAux);
		           l_pParticleEmitter->m_vTimeDirectionInterpolation.push_back(m_fTimeInterpolation);
               m_iNumDirections++;
               
            
		          }
            }
          }
        }

    
        CParticleCore* l_pCore = new CParticleCore();
        
        l_pCore->LoadParticleCore("./Data/XML/ParticleCore.xml",l_szType);//posar aki el xml de les cores.
        if(!l_pCore)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CParticleManager:: No existeix el emiter de tipus %s", l_szType);
          continue;
        }
        l_pParticleEmitter->SetAttributes(l_pCore);
        l_pParticleEmitter->SetTexParticle(l_pCore->GetTexParticle());
			  m_vEmitterParticle.push_back(l_pParticleEmitter);
        delete l_pCore;
		  }
    }
	
		SetOk(true);
	}
  return true;

}

CParticleEmitter* CParticleManager::GetParticleEmitter(const string& _szName)
{
  string l_szNameEmitter;
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();

  while(it != end)
  {
    l_szNameEmitter=(*it)->GetName();
    if(_szName==l_szNameEmitter)
    {
      return (*it);
    }
    ++it;
  }
  LOGGER->AddNewLog(ELL_WARNING, "CParticleManager:: No existeix el emiter de tipus %s", _szName);
  return NULL;
}

void CParticleManager::Update(const float _fElapsedTime, CCamera* camera)
{
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();


  while(it != end)
  {
    (*it)->Update(_fElapsedTime,camera);
    
    ++it;
  }
}

void CParticleManager::Render(CRenderManager* _pRM)
{
  LPDIRECT3DDEVICE9 l_pd3dDevice = _pRM->GetDevice();

#ifdef __PARTICLE_VIA_SHADER__
  CEffectManager* l_pEM = CORE->GetEffectManager();

  assert(l_pEM && l_pEM->IsOk());
  CEffect* l_pEffect = l_pEM->GetEffect("Particle");
  CEffect* l_pPrevEffect = l_pEM->GetForcedStaticMeshEffect();
  l_pEM->SetForcedStaticMeshEffect(l_pEffect);

  //l_pd3dDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1   ));
#else
  _pRM->EnableAlphaBlend();
  l_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  l_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  l_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  
  l_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  l_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
#endif

  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();
  while(it != end)
  {
    (*it)->Render(_pRM);
    ++it;
  }

  
#ifdef __PARTICLE_VIA_SHADER__
  l_pd3dDevice->SetStreamSourceFreq(0, 1);
  l_pd3dDevice->SetStreamSourceFreq(1, 1);
  l_pEM->SetForcedStaticMeshEffect(l_pPrevEffect);
#endif
}

void CParticleManager::Init(CRenderManager* _pRM)
{
  SPARTICLE_VERTEX l_pVertexBuffer[4];

  l_pVertexBuffer[0].x = -1;
  l_pVertexBuffer[0].y = -1;
  l_pVertexBuffer[0].z =  0;

  l_pVertexBuffer[1].x =  1;
  l_pVertexBuffer[1].y = -1;
  l_pVertexBuffer[1].z =  0;

  l_pVertexBuffer[2].x = -1;
  l_pVertexBuffer[2].y =  1;
  l_pVertexBuffer[2].z =  0;

  l_pVertexBuffer[3].x =  1;
  l_pVertexBuffer[3].y =  1;
  l_pVertexBuffer[3].z =  0;

  uint16 l_iIndex[6] = {0,2,1,1,2,3};


  m_pParticleVertex = new CIndexedVertexs<SPARTICLE_VERTEX>(  _pRM,
                                                              (char*)l_pVertexBuffer,
                                                              l_iIndex,
                                                              4, 
                                                              6);

  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();
  while(it != end)
  {
    (*it)->Init(_pRM);
    ++it;
  }
}



void CParticleManager::SetAllEmittersActive(bool _bActive)
{
  for (size_t i=0;i<m_vEmitterParticle.size();++i)
  {
    m_vEmitterParticle[i]->SetActive(_bActive);
  }
}