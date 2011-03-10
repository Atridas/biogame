#include "ParticleManager.h"


CParticleManager::CParticleManager(void)
{
}

void CParticleManager::Reload()
{
  bool Load(const string& _szFileName);
}

//bool CParticleManager::Load(const SPaticleManagerParams& _params)
bool CParticleManager::Load(const string& _szFileName)
{
  //m_szFileName = _params.szFile;
  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CParticleManager::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLParticles;
  if(!l_XMLParticles.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CParticleManager:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    //SetOk(false);
    return false;
  }

	if(strcmp(l_XMLParticles.GetName(),"Particles") == 0)
  {
		CXMLTreeNode l_treeParticleEmitters = l_XMLParticles["ParticleEmitters"];
    int l_iNumChildren = l_treeParticleEmitters.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeParticleEmitter = l_treeParticleEmitters(i);
      if(l_treeParticleEmitter.IsComment())
				continue;
      
			SParticleInfo* l_pInfo= new SParticleInfo();
			Vect3f l_vVec3;
			Vect4f l_vVec4;
			l_pInfo->m_szId = l_treeParticleEmitter.GetPszISOProperty("id" ,"");
			l_pInfo->m_fMinEmitRate = l_treeParticleEmitter.GetFloatProperty("MinEmitRate");
			l_pInfo->m_fMaxEmitRate = l_treeParticleEmitter.GetFloatProperty("MaxEmitRate");
			l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			l_pInfo->m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color2",Vect4f(0.0f),true);
			l_pInfo->m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			l_pInfo->m_fMinSize = l_treeParticleEmitter.GetFloatProperty("MinSize");
			l_pInfo->m_fMaxSize = l_treeParticleEmitter.GetFloatProperty("MaxSize");
			l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction1",Vect3f(3.0f));
			l_pInfo->m_vSpawnDir1 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
			l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction2",Vect3f(0.0f));
			l_pInfo->m_vSpawnDir2 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);

			AddResource(l_pInfo->m_szId,l_pInfo);
		}

		CXMLTreeNode l_treeInstanceParticles = l_XMLParticles["InstanceParticles"];
    l_iNumChildren = l_treeInstanceParticles.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeInstanceParticle = l_treeInstanceParticles(i);
      if(l_treeInstanceParticle.IsComment())
				continue;
      
			SParticleInfo* l_pInfo = new SParticleInfo;

				
			CParticleEmitter l_particleEmitter;
			string l_szType = l_treeInstanceParticle.GetPszProperty("type","");
			l_pInfo = GetResource(l_szType); // s'ha de fer que la Id de la SInfo correspongui amb el type de la instancia 
			Vect3f l_vVec3;

			l_particleEmitter.SetId(l_pInfo->m_szId);
			l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("Position",Vect3f(0.0f));
			l_particleEmitter.SetPosition(D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z));
			l_particleEmitter.SetMinEmitRate(l_pInfo->m_fMinEmitRate);
			l_particleEmitter.SetMaxEmitRate(l_pInfo->m_fMaxEmitRate);
			l_particleEmitter.SetColor1(l_pInfo->m_Color1);
			l_particleEmitter.SetColor2(l_pInfo->m_Color2);
			l_particleEmitter.SetMinSize(l_pInfo->m_fMinSize);
			l_particleEmitter.SetMaxSize(l_pInfo->m_fMaxSize);
			l_particleEmitter.SetSpawnDir1(l_pInfo->m_vSpawnDir1);
			l_particleEmitter.SetSpawnDir2(l_pInfo->m_vSpawnDir2);
			//l_particleEmitter.SetTexParticle(NULL);

			//m_vEmitterParticle.push_back(l_particleEmitter);
			delete l_pInfo;
		}
	
		int jorls = 0;
		jorls = 0+12;
	}
/*
  CXMLTreeNode l_treeParticles = l_XMLParticles["Particles"];
  int l_iNumParticles = l_treeParticles.GetNumChildren();
  for(int i = 0; i < l_iNumParticles; i++)
	{
		//--------Particle  Emitter-------------
		CXMLTreeNode l_treeParticleEmitter = l_treeParticles(i)["ParticleEmitter"];
		if(l_treeParticleEmitter.Exists())
		{
					
			if(l_treeParticleEmitter.IsComment()) 
			{
				continue;
			}
    
    
    
			SInfo* l_pInfo= new SInfo();
			Vect3f l_vVec3;
			Vect4f l_vVec4;
			l_pInfo->m_szId = l_treeParticleEmitter.GetPszISOProperty("id" ,"");
			l_pInfo->m_fMinEmitRate = l_treeParticleEmitter.GetFloatProperty("MinEmitRate");
			l_pInfo->m_fMaxEmitRate = l_treeParticleEmitter.GetFloatProperty("MaxEmitRate");
			l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			l_pInfo->m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color2",Vect4f(0.0f),true);
			l_pInfo->m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			l_pInfo->m_fMinSize = l_treeParticleEmitter.GetFloatProperty("MinSize");
			l_pInfo->m_fMaxSize = l_treeParticleEmitter.GetFloatProperty("MaxSize");
			l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction1",Vect3f(0.0f));
			l_pInfo->m_vSpawnDir1 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
			l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Direction2",Vect3f(0.0f));
			l_pInfo->m_vSpawnDir2 = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);

			AddResource(l_pInfo->m_szId,l_pInfo);
			//---------------------------------------------------
		

		}

			//--------Instance  Particle -------------

		
		
			CXMLTreeNode l_treeInstanceParticle = l_treeParticles(i)["InstanceParticle"];
			
		if(l_treeInstanceParticle.Exists())
		{
				if(l_treeInstanceParticle.IsComment())
				{
					continue;
				}
				// depenen del tipus, carregar una font o una altre
				// posar tots els valor segons la font carregada
				// llegir les caracteristiques del instanceparticle, i posar-ho tot en un vector
				CParticleEmitter l_particleEmitter;
				string l_szType = l_treeInstanceParticle.GetPszProperty("type","");

				SInfo* l_pInfo= new SInfo();

				
				
				l_pInfo = GetResource(l_szType); // s'ha de fer que la Id de la SInfo correspongui amb el type de la instancia 
				Vect3f l_vVec3;

				l_particleEmitter.SetId(l_pInfo->m_szId);
				l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("Position",Vect3f(0.0f));
				l_particleEmitter.SetPosition(D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z));
				l_particleEmitter.SetMinEmitRate(l_pInfo->m_fMinEmitRate);
				l_particleEmitter.SetMaxEmitRate(l_pInfo->m_fMaxEmitRate);
				l_particleEmitter.SetColor1(l_pInfo->m_Color1);
				l_particleEmitter.SetColor2(l_pInfo->m_Color2);
				l_particleEmitter.SetMinSize(l_pInfo->m_fMinSize);
				l_particleEmitter.SetMaxSize(l_pInfo->m_fMaxSize);
				l_particleEmitter.SetSpawnDir1(l_pInfo->m_vSpawnDir1);
				l_particleEmitter.SetSpawnDir2(l_pInfo->m_vSpawnDir2);
				//l_particleEmitter.SetTexParticle();

				m_vEmitterParticle.push_back(l_particleEmitter);
		}
			//---------------------------------------

	/*
			CParticleEmitter* l_particleEmitter= new CParticleEmitter();
			l_particleEmitter->Init(l_XMLParticle);
			AddResource(l_particleEmitter->GetId(),l_particleEmitter);*/
    
		 /* l_particleEmitter.SetId(l_XMLParticle.GetPszISOProperty("id" ,""));
			//l_particleEmitter.SetPosition(l_XMLParticle.GetVect3fProperty("Position")); 
			l_particleEmitter.SetMinEmitRate(l_XMLParticle.GetFloatProperty("MinEmitRate"));
			l_particleEmitter.SetMaxEmitRate(l_XMLParticle.GetFloatProperty("MaxEmitRate"));
			//l_particleEmitter.SetColor1(l_XMLParticle.GetVect4fProperty("Color1",Vect4f(0.0f),true));
			//l_particleEmitter.SetColor2(l_XMLParticle.GetVect4fProperty("Color2",Vect4f(0.0f),true));
			l_particleEmitter.SetMinSize(l_XMLParticle.GetFloatProperty("MinSize"));
			l_particleEmitter.SetMaxSize(l_XMLParticle.GetFloatProperty("MaxSize"));
			//l_particleEmitter.SetSpawnDir1();
			//l_particleEmitter.SetSpawnDir2();
			//l_particleEmitter.SetTexParticle();

			m_vEmitterParticle.push_back(l_particleEmitter);


			
	}*/
 

   
  
  
 // SetOk(true);
 // return IsOk();
  return true;

}