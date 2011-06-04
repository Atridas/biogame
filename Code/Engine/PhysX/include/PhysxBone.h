#pragma once
#ifndef INC_PHYSICS_BONE_H_
#define INC_PHYSICS_BONE_H_

#include "base.h"
#include "Named.h"

class CalBone;
class CPhysicActor;
class CPhysicUserData;
class CXMLTreeNode;


class CPhysxBone :
  public CNamed,
  public CBaseControl
{
public:
  CPhysxBone(string _szName) : CNamed(_szName),m_pCalBone(0), m_iParentID(-1), m_pActor(0), m_bRoot(true),m_pBoneUserData(0) {};
  ~CPhysxBone() {Done();};

  bool                Init(CalBone* _pBone,Mat44f _vMat44);
  virtual void        Release();

  void                UpdateCal3dFromPhysx();

  //GETERS I SETERS
  CalBone*            GetCalBone()                           {return m_pCalBone;};
  Vect3f              GetMiddlePoint()                       {return m_vMiddlePoint;};
  CPhysicActor*       GetPhysxActor()                        {return m_pActor;};
  const vector<int>   GetChildList()                         {return m_vChildListID;};
  int                 GetParentID()                          {return m_iParentID;};
  void                SetPhysxActor(CPhysicActor* _pActor)   {m_pActor = _pActor;};
  void                SetCalBone(CalBone* _pCalBone)         {m_pCalBone = _pCalBone;};
  
  
  bool                IsBoneRoot()                           {return m_bRoot;};
  Mat44f              GetBoneLeftHandedAbsoluteTransformation(CalBone* _pBone);

  //Funcions per afegir la info del actor de fisica.
  bool				        AddBoxActor(CXMLTreeNode _XMLObjects);
  bool				        AddSphereActor(CXMLTreeNode _XMLObjects);
  bool				        AddCapsuleActor(CXMLTreeNode _XMLObjects);

  //Funcions per inicialitzar la info del Bone (primer matrius, segon actors, tercer joints)
  void                InitBoneMatrix();
  void                InitPhysXActor();
  void                InitPhysXJoint(CPhysxBone* _pParent);


  
private:

  Mat44f              m_vMatAnimatedModel;
  CalBone*            m_pCalBone;
  int                 m_iParentID;
  Vect3f              m_vMiddlePoint;
  bool                m_bRoot;
  vector<int>         m_vChildListID;

  Mat44f              m_vMatActor;

  CPhysicActor*       m_pActor;
  CPhysicUserData*    m_pBoneUserData;
};

#endif