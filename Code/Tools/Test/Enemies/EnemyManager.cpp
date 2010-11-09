#include "EnemyManager.h"

#include "XML/XMLTreeNode.h"

#include "Patroller.h"
#include "Camera.h"
#include "Sniper.h"
#include "Turret.h"

CEnemyManager::CEnemyManager(void)
{
  //---
}


void CEnemyManager::Relase(void)
{
  {
    tInstances::iterator It  = m_Instances.begin();
    tInstances::iterator End = m_Instances.end();
 
    while(It != End)
    {
      delete (*It);
      It++;
    }
    m_Instances.clear();
  }
  {
    tResources::iterator It  = m_Resources.begin();
    tResources::iterator End = m_Resources.end();

    while(It != End)
    {
      delete (*It).second;
      It++;
    }
    m_Resources.clear();
  }
}


bool CEnemyManager::Init(const char* _pcFile)
{
  CXMLTreeNode FileXML;

  FileXML.LoadFile(_pcFile);

  //CXMLTreeNode Enemies = fileXML(0);
  {
    CXMLTreeNode Templates = FileXML["templates"];

    int iNumChildren = Templates.GetNumChildren();
    for(int i = 0; i < iNumChildren; i++) {
      CXMLTreeNode EnemyTemplate = Templates(i);

      const char* pcId   = EnemyTemplate.GetPszProperty("id");
      const char* pcMesh = EnemyTemplate.GetPszProperty("mesh");

      CEnemyResource* pResource = new CEnemyResource();
      std::string szId = pcId;

      //TODO comprovar errors
      pResource->Init(pcId,pcMesh);

      m_Resources[szId] = pResource;
    
    }
  }
  {
    Vect3f Default(0,0,0);


    CXMLTreeNode Instances = FileXML["instances"];

    int iNumChildren = Instances.GetNumChildren();
    for(int i = 0; i < iNumChildren; i++) {
      CXMLTreeNode EnemyInstance = Instances(i);

      const char* pcType = EnemyInstance.GetPszProperty("type");
      const Vect3f vPos = EnemyInstance.GetVect3fProperty("pos",Default);
      const Vect3f vRot = EnemyInstance.GetVect3fProperty("rot",Default);
      const float fMoveSpeed = EnemyInstance.GetFloatProperty("move_speed");
      const float fRotateSpeed = EnemyInstance.GetFloatProperty("rotate_speed");
      const float fHealth = EnemyInstance.GetFloatProperty("health");

      if(strcmp(pcType,"PATROLLER") == 0)
      {
        const char* pcPaths = EnemyInstance.GetPszProperty("paths");

        CPatroller* pPatroller = new CPatroller();
        pPatroller->Init(vPos,vRot,fMoveSpeed,fRotateSpeed,fHealth,pcPaths);

        m_Instances.push_back(pPatroller);
      } else if(strcmp(pcType,"SNIPER") == 0)
      {
		    float fAimDistance = EnemyInstance.GetFloatProperty("aim_distance");

        CSniper* pSniper = new CSniper();
        pSniper->Init(vPos,vRot,fMoveSpeed,fRotateSpeed,fHealth,fAimDistance);

        m_Instances.push_back(pSniper);
      } else if(strcmp(pcType,"TURRET") == 0)
      {
        Vect3f vMaxAngle = EnemyInstance.GetVect3fProperty("max_angle",Default);
        Vect3f vMinAngle = EnemyInstance.GetVect3fProperty("min_angle",Default);

        CTurret* pTurret = new CTurret();
        pTurret->Init(vPos,vRot,fMoveSpeed,fRotateSpeed,fHealth,vMaxAngle,vMinAngle);

        m_Instances.push_back(pTurret);
      } else if(strcmp(pcType,"CAMERA") == 0)
      {
        Vect3f vMaxAngle = EnemyInstance.GetVect3fProperty("max_angle",Default);
        Vect3f vMinAngle = EnemyInstance.GetVect3fProperty("min_angle",Default);

        CCamera* pCamera = new CCamera();
        pCamera->Init(vPos,vRot,fMoveSpeed,fRotateSpeed,fHealth,vMaxAngle,vMinAngle);

        m_Instances.push_back(pCamera);
      } else {
        //TODO errors
      }
    
    }
  }

  return true;
}
