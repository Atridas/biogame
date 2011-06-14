#include "ParticleManager.h"
#include "Core.h"
#include "TextureManager.h"
#include "RenderManager.h"


CParticleManager::CParticleManager()
{
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
      int l_iNumChildrenEmitters = l_treeParticleEmitters.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmitters.", l_iNumChildrenEmitters);
      for(int i = 0; i < l_iNumChildrenEmitters; i++)
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
			  l_Event.m_fMinSize = l_treeParticleEmitter.GetFloatProperty("MinSize");
			  l_Event.m_fMaxSize = l_treeParticleEmitter.GetFloatProperty("MaxSize");
			  l_pInfo->m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeParticleEmitter.GetPszProperty("TexParticle",""));
        l_pInfo->m_fLife1 = l_treeParticleEmitter.GetFloatProperty("Life1");
        l_pInfo->m_fLife2 = l_treeParticleEmitter.GetFloatProperty("Life2");
        l_pInfo->m_fAngle1 = l_treeParticleEmitter.GetFloatProperty("Angle1");
        l_pInfo->m_fAngle2 = l_treeParticleEmitter.GetFloatProperty("Angle2");
        l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Gravity",Vect3f(3.0f));
        l_pInfo->m_vGravity = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
        l_vVec3 = l_treeParticleEmitter.GetVect3fProperty("Velocitate",Vect3f(3.0f));
        l_pInfo->m_vVel = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
        l_pInfo->m_bAnimated = false; // si te animacio despres la canvio
        l_pInfo->m_bBucleInfinit=l_treeParticleEmitter.GetBoolProperty("Bucle");
        if(l_pInfo->m_bBucleInfinit==false)
        {
          l_pInfo->m_iNumBucle=l_treeParticleEmitter.GetIntProperty("NumBucle");
          l_pInfo->m_fRebootEmitter1=l_treeParticleEmitter.GetFloatProperty("RebootEmitter1");
          l_pInfo->m_fRebootEmitter2=l_treeParticleEmitter.GetFloatProperty("RebootEmitter2");
          l_pInfo->m_bTotDeCop=l_treeParticleEmitter.GetBoolProperty("TotDeCop");
        }else
        {
          l_pInfo->m_iNumBucle=0;
          l_pInfo->m_fRebootEmitter1=0;
          l_pInfo->m_fRebootEmitter2=0;
          l_pInfo->m_bTotDeCop=false;
        }
        
			  

        l_pInfo->m_vTimeColor.push_back(0);
        l_pInfo->m_vColor.push_back(l_Event.m_Color1);
        l_pInfo->m_vColor.push_back(l_Event.m_Color2);
        l_pInfo->m_vTimeSize.push_back(0);
        l_pInfo->m_vSize.push_back(l_Event.m_fMinSize);
        l_pInfo->m_vSize.push_back(l_Event.m_fMaxSize);
        l_pInfo->m_vTextureAnimation.push_back(l_pInfo->m_pTexParticle);
        
        int num = l_treeParticleEmitter.GetNumChildren();
      
		    for(int k=0;k<num;++k)
		    {
			    const char *l_Name=l_treeParticleEmitter(k).GetName();
			    if(strcmp(l_Name,"Sizes")==0)
			    {
         
				    CXMLTreeNode l_treeParticleEmittersSizes = l_treeParticleEmitter["Sizes"];

            if(l_treeParticleEmittersSizes.Exists())
            {
              int l_iNumChildrenSizes = l_treeParticleEmittersSizes.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmitterSizes.", l_iNumChildrenSizes);

              for(int j = 0; j < l_iNumChildrenSizes; j++)
              {
              
                CXMLTreeNode l_treeParticleEmittersSize = l_treeParticleEmittersSizes(j);
                if(l_treeParticleEmittersSize.IsComment())
				        continue;

            

                l_Event.m_fTime = l_treeParticleEmittersSize.GetFloatProperty("time");
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersSize.GetFloatProperty("timeInterpolation");
            
                l_Event.m_fMinSize = l_treeParticleEmittersSize.GetFloatProperty("MinSize");
			          l_Event.m_fMaxSize = l_treeParticleEmittersSize.GetFloatProperty("MaxSize");

                l_pInfo->m_vTimeSize.push_back(l_Event.m_fTime);
                l_pInfo->m_vTimeSizeInterpolation.push_back(l_Event.m_fTimeInterpolation);
                l_pInfo->m_vSize.push_back(l_Event.m_fMinSize);
                l_pInfo->m_vSize.push_back(l_Event.m_fMaxSize);

              }
            }
			    }
			    else if(strcmp(l_Name,"Colors")==0)
			    {
				    CXMLTreeNode l_treeParticleEmittersColors = l_treeParticleEmitter["Colors"];
          
            if(l_treeParticleEmittersColors.Exists())
            {
              int l_iNumChildrenColors = l_treeParticleEmittersColors.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmittersColors.", l_iNumChildrenColors);

              for(int j = 0; j < l_iNumChildrenColors; j++)
              {
              
                CXMLTreeNode l_treeParticleEmittersColor = l_treeParticleEmittersColors(j);
                if(l_treeParticleEmittersColor.IsComment())
				        continue;
          
                l_Event.m_fTime = l_treeParticleEmittersColor.GetFloatProperty("time");
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersColor.GetFloatProperty("timeInterpolation");
            

                l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			          l_Event.m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			          l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color2",Vect4f(0.0f),true);
			          l_Event.m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);

                l_pInfo->m_vTimeColor.push_back(l_Event.m_fTime);
                l_pInfo->m_vTimeColorInterpolation.push_back(l_Event.m_fTimeInterpolation);
                l_pInfo->m_vColor.push_back(l_Event.m_Color1);
                l_pInfo->m_vColor.push_back(l_Event.m_Color2);

              }
            }
			    }
			    else if(strcmp(l_Name,"Animateds")==0)
			    {
				    CXMLTreeNode l_treeParticleEmittersAnimateds = l_treeParticleEmitter["Animateds"];

            if(l_treeParticleEmittersAnimateds.Exists())
            {
              int l_iNumChildrenAnimateds = l_treeParticleEmittersAnimateds.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleManager::Load Loading %d ParticleEmittersAnimated.", l_iNumChildrenAnimateds);

              for(int j = 0; j < l_iNumChildrenAnimateds; j++)
              {
            
                CXMLTreeNode l_treeParticleEmittersAnimated = l_treeParticleEmittersAnimateds(j);
                if(l_treeParticleEmittersAnimated.IsComment())
				        continue;

            
			          l_pInfo->m_bAnimated = true;
                l_Event.m_fTime = l_treeParticleEmittersAnimated.GetFloatProperty("time");
			          l_pInfo->m_vTimeAnimated.push_back(l_Event.m_fTime);
			          l_Event.m_fTime = l_treeParticleEmittersAnimated.GetFloatProperty("TimeDiapo");
			          l_pInfo->m_vTimeAnimated.push_back(l_Event.m_fTime);
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersAnimated.GetFloatProperty("timeInterpolation");
                l_pInfo->m_vTimeAnimatedInterpolation.push_back(l_Event.m_fTimeInterpolation);
                l_pInfo->m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeParticleEmittersAnimated.GetPszProperty("TexParticle",""));
            			
			          l_Event.m_iTexNumFiles = l_treeParticleEmittersAnimated.GetIntProperty("NumFiles");
			          l_Event.m_iTexNumColumnes = l_treeParticleEmittersAnimated.GetIntProperty("NumColumnes");
			          l_pInfo->m_vFilesColumnes.push_back(l_Event.m_iTexNumFiles);
			          l_pInfo->m_vFilesColumnes.push_back(l_Event.m_iTexNumColumnes);
                l_pInfo->m_vTextureAnimation.push_back(l_pInfo->m_pTexParticle);
                l_pInfo->m_iNumDirections++;
			
              }
            }
			    }
		    }
        AddResource(l_pInfo->m_szId,l_pInfo);
      }
    }

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
      
        
			  string l_szType = l_treeInstanceParticle.GetPszProperty("type","");
        SParticleInfo* l_pInfo = GetResource(l_szType);
        CParticleEmitter* l_pParticleEmitter = new CParticleEmitter();
        			  
        // s'ha de fer que la Id de la SInfo correspongui amb el type de la instancia
			  if(!l_pInfo)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CParticleManager:: No existeix el emiter de tipus %s", l_szType);
          continue;
        }

			  Vect3f l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("position",Vect3f(0.0f));
			  l_pParticleEmitter->SetPosition(D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z));
        l_pParticleEmitter->SetName(l_treeInstanceParticle.GetPszISOProperty("id" ,""));
        l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("Direction",Vect3f(3.0f));
        m_vSpawnDir = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
		    l_vVec3 = l_treeInstanceParticle.GetVect3fProperty("Desviacion",Vect3f(3.0f));
        m_vDesviacionSpawnDir = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
		
		    m_szFormEmitter = l_treeInstanceParticle.GetPszISOProperty("FormEmitter","");
        l_pParticleEmitter->SetFormEmitter(m_szFormEmitter);

   
		    if(m_szFormEmitter=="dummy")
		    {
			    m_fSizeX = l_treeInstanceParticle.GetFloatProperty("sizeX");
			    m_fSizeY = l_treeInstanceParticle.GetFloatProperty("sizeY");
			    m_fSizeZ = l_treeInstanceParticle.GetFloatProperty("sizeZ");
			    m_vPosFormEmitter.x=m_fSizeX;
          m_vPosFormEmitter.y=m_fSizeY;
          m_vPosFormEmitter.z=m_fSizeZ;
			    l_pParticleEmitter->SetPositionFormEmitter(m_vPosFormEmitter);
			    l_pParticleEmitter->SetFormEmitter(m_szFormEmitter);
		    }

        l_pParticleEmitter->m_vTimeDirection.push_back(0);
        D3DXVECTOR3 l_vAux = m_vSpawnDir- m_vDesviacionSpawnDir;
        l_pParticleEmitter->m_vDirection.push_back(l_vAux);
        l_vAux = m_vSpawnDir+ m_vDesviacionSpawnDir;
        l_pParticleEmitter->m_vDirection.push_back(l_vAux);

        
        //********************************************

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

         

                m_fTime = l_treeParticleInstanceDiredtion.GetFloatProperty("time");
                m_fTimeInterpolation = l_treeParticleInstanceDiredtion.GetFloatProperty("timeInterpolation");
            
			
			          l_vVec3 = l_treeParticleInstanceDiredtion.GetVect3fProperty("Direction",Vect3f(3.0f));
                m_vSpawnDir = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
			          l_vVec3 = l_treeParticleInstanceDiredtion.GetVect3fProperty("Desviacion",Vect3f(3.0f));
                m_vDesviacionSpawnDir = D3DXVECTOR3(l_vVec3.x,l_vVec3.y,l_vVec3.z);
            
            
		            D3DXVECTOR3 l_vAux = m_vSpawnDir- m_vDesviacionSpawnDir;
		           l_pParticleEmitter->m_vTimeDirection.push_back(m_fTime);
               l_pParticleEmitter->m_vDirection.push_back(l_vAux);
               l_vAux = m_vSpawnDir+ m_vDesviacionSpawnDir;
               l_pParticleEmitter->m_vDirection.push_back(l_vAux);
		           l_pParticleEmitter->m_vTimeDirectionInterpolation.push_back(m_fTimeInterpolation);
               l_pInfo->m_iNumDirections++;
            
		          }
            }
          }
        }

        //*********************************************
        l_pParticleEmitter->SetAttributes(l_pInfo);
			  m_vEmitterParticle.push_back(l_pParticleEmitter);
		  }
    }
	
		SetOk(true);
	}
  return true;

}

CParticleEmitter* CParticleManager::GetParticleEmitter(const string& _szName)
{
  
  vector<CParticleEmitter*>::iterator it  = m_vEmitterParticle.begin(),
                                      end = m_vEmitterParticle.end();

  while(it != end)
  {
    m_szFileName=(*it)->GetName();
    if(_szName==m_szFileName)
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
  /*Mat44f l_mat;
  l_mat.SetIdentity();

  _pRM->SetTransform(l_mat);*/

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



void CParticleManager::SetAllEmittersActive(bool _bActive)
{
  for (size_t i=0;i<m_vEmitterParticle.size();++i)
  {
    m_vEmitterParticle[i]->SetActive(_bActive);
  }
}