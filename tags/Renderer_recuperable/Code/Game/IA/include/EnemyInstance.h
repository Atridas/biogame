#pragma once

#ifndef __ENEMY_INSTANCE_H__
#define __ENEMY_INSTANCE_H__

#include <XML/XMLTreeNode.h>
#include "Named.h"
#include "Enemy.h"
//#include "Utils/Object3D.h"

class CEnemyInstance : 
  public CEnemy
  //public CObject3D
{
public:
  CEnemyInstance(const string& _name) : CEnemy(_name),
                                        m_vRotate(Vect3f(0.0f))    {};

  ~CEnemyInstance(){};

  virtual void InitInstance(CXMLTreeNode& _XMLParams);

  void SetPosition(Vect3f& _vPosition) {m_vPosition = _vPosition;};
  void SetRotate(Vect3f& _vRotate) {m_vRotate = _vRotate;};


  Vect3f GetPosition() {return m_vPosition;};
  Vect3f GetRotate() {return m_vRotate;};

protected:
  Vect3f  m_vRotate;
};
#endif
