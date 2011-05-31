#pragma once
#ifndef INC_PHYSICS_BONE_H_
#define INC_PHYSICS_BONE_H_

#include "base.h"
#include "Named.h"

class CalBone;
class CPhysicActor;
class CPhysicUserData;


class CPhysxBone :
  public CNamed,
  public CBaseControl
{
public:
  CPhysxBone(string _szName) : CNamed(_szName),m_pCalBone(0), m_iParentID(-1), m_pActor(0), m_bRoot(true),m_pBoneUserData(0) {};
  ~CPhysxBone() {Done();};

  bool                Init(CalBone* _pBone);
  virtual void        Release();

  void                UpdateCal3dFromPhysx();
  CalBone*            GetCalBone()                           {return m_pCalBone;};
  void                SetCalBone(CalBone* _pCalBone)         {m_pCalBone = _pCalBone;};
  CPhysicActor*       GetPhysxActor()                        {return m_pActor;};
  void                SetPhysxActor(CPhysicActor* _pActor)   {m_pActor = _pActor;};
  bool                IsBoneRoot()                           {return m_bRoot;};
  Mat44f              GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone);

  //Funcions per inicialitzar la info del Bone (primer matrius, segon actors, tercer joints)
  void                Load(float _fDensity, string _szType, Vect3f _fMiddlePoint,Vect3f _vSize,string _szName);
  void                InitBoneMatrix();
  void                InitPhysXActor();
  void                InitPhysXJoint(CPhysxBone* _pParent);


  
private:

  CalBone*            m_pCalBone;
  int                 m_iParentID;
  bool                m_bRoot;
  vector<int>         m_vChildListID;

  Mat44f              m_vMatActor;

  CPhysicActor*       m_pActor;
  CPhysicUserData*    m_pBoneUserData;
};

#endif