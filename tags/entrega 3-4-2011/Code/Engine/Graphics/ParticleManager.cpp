#include "ParticleManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "RenderManager.h"


CParticleManager::CParticleManager()
{
  m_szFileName = "";
  m_bReload = false;
}

/*bool CParticleManager::Reload()
{
  Release();

  bool Load(const string& _szFileName);
 return true; 
}*/

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
		CXMLTreeNode l_treeParticleEmitters = l_XMLParticles["ParticleEmitters"];

    if(l_treeParticleEmitters.Exists())
    {
      int l_iNumChildren = l_treeParticleEmitters.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmitters.", l_iNumChildren);

      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_treeParticleEmitter = l_treeParticleEmitters(i);
        if(l_treeParticleEmitter.IsComment())
				  continue;
      
			  SParticleInfo* l_pInfo= new SParticleInfo;
        SParticleEvent l_Event;
        Vect3f l_vVec3;
			  Vect4f l_vVec4;

       	    
        
        l_pInfo->m_szId = l_treeParticleEmitter.GetPszISOProperty("id" ,"");
			  l_pInfo->m_fMinEmitRate = l_treeParticleEmitter.GetFloatProperty("MinEmitRate");
			  l_pInfo->m_fMaxEmitRate = l_treeParticleEmitter.GetFloatProperty("MaxEmitRate");
			  l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			  l_Event.m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			  l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color2",Vect4f(0.0f),true);
			  l_Event.m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			  l_pInfo->m_fMinSize = l_treeParticleEmitter.GetFloatProperty("MinSize");
			  l_pInfo->m_fMaxSize = l_treeParticleEmitter.GetFloatProperty("MaxSize");
			  l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction1",Vect3f(3.0f));
        l_Event.m_vSpawnDir1 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
			  l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction2",Vect3f(0.0f));
        l_Event.m_vSpawnDir2 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
        l_pInfo->m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeParticleEmitter.GetPszProperty("TexParticle",""));
        l_pInfo->m_fLife1 = l_treeParticleEmitter.GetFloatProperty("Life1");
        l_pInfo->m_fLife2 = l_treeParticleEmitter.GetFloatProperty("Life2");
        
			  

        l_pInfo->m_vTimeColor.push_back(0);
        l_pInfo->m_vColor.push_back(l_Event.m_Color1);
        l_pInfo->m_vColor.push_back(l_Event.m_Color2);
        l_pInfo->m_vTimeDirection.push_back(0);
        l_pInfo->m_vDirection.push_back(l_Event.m_vSpawnDir1);
        l_pInfo->m_vDirection.push_back(l_Event.m_vSpawnDir2);
      
        CXMLTreeNode l_treeParticleEmittersColors = l_treeParticleEmitter["Colors"];

        if(l_treeParticleEmittersColors.Exists())
        {
          int l_iNumChildren = l_treeParticleEmittersColors.GetNumChildren();

          LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmittersColors.", l_iNumChildren);

          for(int i = 0; i < l_iNumChildren; i++)
          {
            //SParticleEvent l_Event;
            CXMLTreeNode l_treeParticleEmittersColor = l_treeParticleEmittersColors(i);
            if(l_treeParticleEmittersColor.IsComment())
				    continue;

            

            l_Event.m_fTime = l_treeParticleEmittersColor.GetFloatProperty("time");
            

            l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			      l_Event.m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			      l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color2",Vect4f(0.0f),true);
			      l_Event.m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);

            l_pInfo->m_vTimeColor.push_back(l_Event.m_fTime);
            l_pInfo->m_vColor.push_back(l_Event.m_Color1);
            l_pInfo->m_vColor.push_back(l_Event.m_Color2);

            //m_vParticleEvent.push_back(l_pEvent);
          }
        }

          CXMLTreeNode l_treeParticleEmittersDiredtions = l_treeParticleEmitter["Directions"];

        if(l_treeParticleEmittersDiredtions.Exists())
        {
          int l_iNumChildren = l_treeParticleEmittersDiredtions.GetNumChildren();

          LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmittersDirections.", l_iNumChildren);

          for(int i = 0; i < l_iNumChildren; i++)
          {
            //SParticleEvent l_Event;
            CXMLTreeNode l_treeParticleEmittersDirection = l_treeParticleEmittersDiredtions(i);
            if(l_treeParticleEmittersDirection.IsComment())
				    continue;

            //SParticleEvent l_Event;

            l_Event.m_fTime = l_treeParticleEmittersDirection.GetFloatProperty("time");
            
			
			      l_vVec3 = l_treeParticleEmittersDirection.GetVect3fProperty("Direction1",Vect3f(3.0f));
            l_Event.m_vSpawnDir1 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
			      l_vVec3 = l_treeParticleEmittersDirection.GetVect3fProperty("Direction2",Vect3f(3.0f));
            l_Event.m_vSpawnDir2 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);


            l_pInfo->m_vTimeDirection.push_back(l_Event.m_fTime);
            l_pInfo->m_vDirection.push_back(l_Event.m_vSpawnDir1);
            l_pInfo->m_vDirection.push_back(l_Event.m_vSpawnDir2);


          }
        }
        AddResource(l_pInfo->m_szId,l_pInfo);
        
      }
    }

		CXMLTreeNode l_treeInstanceParticles = l_XMLParticles["InstanceParticles"];

    if(l_treeInstanceParticles.Exists())
    {
      int l_iNumChildren = l_treeInstanceParticles.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d particules.", l_iNumChildren);

      for(int i = 0; i < l_iNumChildren; i++)
      {
        CXMLTreeNode l_treeInstanceParticle = l_treeInstanceParticles(i);

        if(l_treeInstanceParticle.IsComment())
				  continue;
      
        
			  CParticleEmitter* l_pParticleEmitter = new CParticleEmitter();
			  
			  string l_szType = l_treeInstanceParticle.GetPszProperty("type","");
        // s'ha de fer que la Id de la SInfo correspongui amb el type de la instancia
			  SParticleInfo* l_pInfo = GetResource(l_szType);

        if(!l_pInfo)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CParticleManager:: No existeix el emiter de tipus %s", l_szType);
          continue;
        }

			  Vect3f l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("Position",Vect3f(0.0f));
			  l_pParticleEmitter->SetPosition(D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z));
        l_pParticleEmitter->SetName(l_treeInstanceParticle.GetPszISOProperty("id" ,""));
        l_pParticleEmitter->SetReload(m_bReload);
        l_pParticleEmitter->SetAttributes(l_pInfo);

			  m_vEmitterParticle.push_back(l_pParticleEmitter);
		  }

    }
	
		SetOk(true);
	}
  return true;

}



void CParticleManager::Update(const float _fElapsedTime)
{
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();


  while(it != end)
  {
    (*it)->Update(_fElapsedTime);
    
    ++it;
  }
}

void CParticleManager::Render(CRenderManager* _pRM)
{
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();
  while(it != end)
  {
    (*it)->Render(_pRM);
    ++it;
  }
}

void CParticleManager::Init(CRenderManager* _pRM)
{
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();
  while(it != end)
  {
    (*it)->Init(_pRM);
    ++it;
  }
}