#pragma once


#include <base.h>
#include "Named.h"
//#include <XML/XMLTreeNode.h>

//Forward declarations---------------------
class CPhysicActor;
class CRenderableObject;
class CPhysicUserData;
//-----------------------------------------

class CGameObject:  
  public CNamed,
  public CBaseControl
{
public:
  CGameObject(string _szName) : m_pRenderableObject(0), m_pPhysXActor(0),CNamed(_szName),m_pUserData(0) {};
  ~CGameObject(){Done();};

  //Set Functions
  void                    SetRenderableObject(CRenderableObject* _pRO)  {m_pRenderableObject = _pRO;};
  
  void                    SetPhysXActor(CPhysicActor* _pPA)             {m_pPhysXActor = _pPA;};

  //Get Functions
  CRenderableObject*      GetRenderableObject()                         {return m_pRenderableObject;};
  CPhysicActor*           GetPhysXActor()                               {return m_pPhysXActor;};

  //Other Functions
  void                    Init(CRenderableObject* _pRO,CPhysicActor* _pPA);
  void                    Update(float _fElapsedTime);
protected:
  void                    Release() {};    

private:
  CRenderableObject*          m_pRenderableObject;
  CPhysicActor*               m_pPhysXActor;
  CPhysicUserData*            m_pUserData;
};

