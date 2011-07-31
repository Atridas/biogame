#include "ParticleCore.h"

#include "Core.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>



bool CParticleCore::LoadParticleCore(const string& _szFileName, const string& _type)
{
 
  m_szFileName = _szFileName;
  LOGGER->AddNewLog(ELL_INFORMATION, "CParticleCore::Load \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_XMLParticles;
  if(!l_XMLParticles.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CParticleCore:: No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    SetOk(false);
    return false;
  }

	if(strcmp(l_XMLParticles.GetName(),"ParticleCore") == 0)
  {
		CXMLTreeNode l_treeParticleEmitters = l_XMLParticles["ParticleEmitters"];

    if(l_treeParticleEmitters.Exists())
    {
      int l_iNumChildrenEmitters = l_treeParticleEmitters.GetNumChildren();

      LOGGER->AddNewLog(ELL_INFORMATION,"CParticleCore::Load Loading %d ParticleEmitters.", l_iNumChildrenEmitters);
      for(int i = 0; i < l_iNumChildrenEmitters; i++)
      {
        CXMLTreeNode l_treeParticleEmitter = l_treeParticleEmitters(i);
        if(l_treeParticleEmitter.IsComment())
				  continue;
      
			  m_szId = l_treeParticleEmitter.GetPszISOProperty("id" ,"",true);
        if(_type!=m_szId)
          continue;
        SParticleEvent l_Event;
        Vect3f l_vVec3;
			  Vect4f l_vVec4;

       	
			  m_fMinEmitRate = l_treeParticleEmitter.GetFloatProperty("MinEmitRate",1,true);
			  m_fMaxEmitRate = l_treeParticleEmitter.GetFloatProperty("MaxEmitRate",m_fMinEmitRate,false);
			  l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			  l_Event.m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			  l_vVec4 = l_treeParticleEmitter.GetVect4fProperty("Color2",l_vVec4,false);
			  l_Event.m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			  l_Event.m_fMinSize = l_treeParticleEmitter.GetFloatProperty("MinSize",1,true);
			  l_Event.m_fMaxSize = l_treeParticleEmitter.GetFloatProperty("MaxSize",l_Event.m_fMinSize,false);
			  m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeParticleEmitter.GetPszProperty("TexParticle","",true));
        m_fLife1 = l_treeParticleEmitter.GetFloatProperty("Life1", 1, true);
        m_fLife2 = l_treeParticleEmitter.GetFloatProperty("Life2", m_fLife2, false);
        m_fAngle1 = l_treeParticleEmitter.GetFloatProperty("Angle1", 0, false);
        m_fAngle2 = l_treeParticleEmitter.GetFloatProperty("Angle2", m_fAngle1, false);
        m_vGravity = l_treeParticleEmitter.GetVect3fProperty("Gravity",Vect3f(0.0f),false);
        m_vVel = l_treeParticleEmitter.GetVect3fProperty("Velocitate",Vect3f(1,1,1),false);
        m_bAnimated = false; // si te animacio despres la canvio
        m_bBucleInfinit=l_treeParticleEmitter.GetBoolProperty("Bucle",true,false);
        if(m_bBucleInfinit==false)
        {
          m_iNumBucle=l_treeParticleEmitter.GetIntProperty("NumBucle",1,true);
          m_fRebootEmitter1=l_treeParticleEmitter.GetFloatProperty("RebootEmitter1",1,true);
          m_fRebootEmitter2=l_treeParticleEmitter.GetFloatProperty("RebootEmitter2",m_fRebootEmitter1,false);
          m_bTotDeCop=l_treeParticleEmitter.GetBoolProperty("TotDeCop",true,true);
        }else
        {
          m_iNumBucle=0;
          m_fRebootEmitter1=0;
          m_fRebootEmitter2=0;
          m_bTotDeCop=false;
        }
        m_vTimeColor.push_back(0);
        m_vColor.push_back(l_Event.m_Color1);
        m_vColor.push_back(l_Event.m_Color2);
        m_vTimeSize.push_back(0);
        m_vSize.push_back(l_Event.m_fMinSize);
        m_vSize.push_back(l_Event.m_fMaxSize);
        m_vTextureAnimation.push_back(m_pTexParticle);
        
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

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleCore::Load Loading %d ParticleEmitterSizes.", l_iNumChildrenSizes);

              for(int j = 0; j < l_iNumChildrenSizes; j++)
              {
              
                CXMLTreeNode l_treeParticleEmittersSize = l_treeParticleEmittersSizes(j);
                if(l_treeParticleEmittersSize.IsComment())
				        continue;

            

                l_Event.m_fTime = l_treeParticleEmittersSize.GetFloatProperty("time",0,true);
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersSize.GetFloatProperty("timeInterpolation",0,false);
            
                l_Event.m_fMinSize = l_treeParticleEmittersSize.GetFloatProperty("MinSize",1,true);
			          l_Event.m_fMaxSize = l_treeParticleEmittersSize.GetFloatProperty("MaxSize",l_Event.m_fMinSize,false);

                m_vTimeSize.push_back(l_Event.m_fTime);
                m_vTimeSizeInterpolation.push_back(l_Event.m_fTimeInterpolation);
                m_vSize.push_back(l_Event.m_fMinSize);
                m_vSize.push_back(l_Event.m_fMaxSize);

              }
            }
			    }
			    else if(strcmp(l_Name,"Colors")==0)
			    {
				    CXMLTreeNode l_treeParticleEmittersColors = l_treeParticleEmitter["Colors"];
          
            if(l_treeParticleEmittersColors.Exists())
            {
              int l_iNumChildrenColors = l_treeParticleEmittersColors.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleCore::Load Loading %d ParticleEmittersColors.", l_iNumChildrenColors);

              for(int j = 0; j < l_iNumChildrenColors; j++)
              {
              
                CXMLTreeNode l_treeParticleEmittersColor = l_treeParticleEmittersColors(j);
                if(l_treeParticleEmittersColor.IsComment())
				        continue;
          
                l_Event.m_fTime = l_treeParticleEmittersColor.GetFloatProperty("time", 0, true);
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersColor.GetFloatProperty("timeInterpolation",0, false);
            

                l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color1",Vect4f(0.0f),true);
			          l_Event.m_Color1 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);
			          l_vVec4 = l_treeParticleEmittersColor.GetVect4fProperty("Color2",l_vVec4,false);
			          l_Event.m_Color2 = D3DXCOLOR(l_vVec4.x,l_vVec4.y,l_vVec4.z,l_vVec4.w);

                m_vTimeColor.push_back(l_Event.m_fTime);
                m_vTimeColorInterpolation.push_back(l_Event.m_fTimeInterpolation);
                m_vColor.push_back(l_Event.m_Color1);
                m_vColor.push_back(l_Event.m_Color2);

              }
            }
			    }
			    else if(strcmp(l_Name,"Animateds")==0)
			    {
				    CXMLTreeNode l_treeParticleEmittersAnimateds = l_treeParticleEmitter["Animateds"];

            if(l_treeParticleEmittersAnimateds.Exists())
            {
              int l_iNumChildrenAnimateds = l_treeParticleEmittersAnimateds.GetNumChildren();

              LOGGER->AddNewLog(ELL_INFORMATION,"CParticleCore::Load Loading %d ParticleEmittersAnimated.", l_iNumChildrenAnimateds);

              for(int j = 0; j < l_iNumChildrenAnimateds; j++)
              {
            
                CXMLTreeNode l_treeParticleEmittersAnimated = l_treeParticleEmittersAnimateds(j);
                if(l_treeParticleEmittersAnimated.IsComment())
				        continue;

            
			          m_bAnimated = true;
                l_Event.m_fTime = l_treeParticleEmittersAnimated.GetFloatProperty("time",0,true);
			          m_vTimeAnimated.push_back(l_Event.m_fTime);
			          l_Event.m_fTime = l_treeParticleEmittersAnimated.GetFloatProperty("TimeDiapo",1,true);
			          m_vTimeAnimated.push_back(l_Event.m_fTime);
                l_Event.m_fTimeInterpolation = l_treeParticleEmittersAnimated.GetFloatProperty("timeInterpolation",0,false);
                m_vTimeAnimatedInterpolation.push_back(l_Event.m_fTimeInterpolation);
                m_pTexParticle = CORE->GetTextureManager()->GetResource(l_treeParticleEmittersAnimated.GetPszProperty("TexParticle","",true));
            			
			          l_Event.m_iTexNumFiles = l_treeParticleEmittersAnimated.GetIntProperty("NumFiles",1,true);
			          l_Event.m_iTexNumColumnes = l_treeParticleEmittersAnimated.GetIntProperty("NumColumnes",1,true);
			          m_vFilesColumnes.push_back(l_Event.m_iTexNumFiles);
			          m_vFilesColumnes.push_back(l_Event.m_iTexNumColumnes);
                m_vTextureAnimation.push_back(m_pTexParticle);
              }
            }
			    }
		    }
        
        return true;
      }
    }
  }
  return false;

}


void CParticleCore::Release() 
{
   m_vTextureAnimation.clear();
   m_vColor.clear();
  m_vTimeColor.clear();
   m_vTimeColorInterpolation.clear();
   m_vSize.clear();
   m_vTimeSize.clear();
   m_vTimeSizeInterpolation.clear();
 }