#pragma once

#ifndef __MINER_H__
#define __MINER_H__

#include "EnemyInstance.h"

class CMiner: public CEnemyInstance
{
public:

  CMiner(const string& _name) : CEnemyInstance(_name),
                                  m_fRotateSpeed(0.0f)
                                  {m_Type = CEnemy::MINER;};

  


  ~CMiner(void){};

  virtual void InitTemplate(CXMLTreeNode& _XMLParams);
  virtual void InitInstance(CXMLTreeNode& _XMLParams);

  //virtual void Render(CRenderManager* _pRM) const;
  
  // els set s'hauran de calcular
  void SetRotateSpeed(float _fRotateSpeed){m_fRotateSpeed = _fRotateSpeed;};

  float GetRotateSpeed() const {return m_fRotateSpeed;};

protected:
  void Release() {};

  float m_fRotateSpeed;
 
};
#endif
