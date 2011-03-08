#include "include\ParticleManager.h"


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


  
  int l_iNumParticleEmitters = l_XMLParticles.GetNumChildren();
  for(int i = 0; i < l_iNumParticleEmitters; i++)
  {
    CXMLTreeNode l_XMLParticle = l_XMLParticles(i);
    if(l_XMLParticle.IsComment()) 
    {
      continue;
    }

    
    CParticleEmitter l_particleEmitter;
    l_particleEmitter.Init(l_XMLParticle);
    
    
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


    */
    
 

   
  
  }
 // SetOk(true);
 // return IsOk();
  return true;

}