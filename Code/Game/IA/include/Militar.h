#pragma once

#ifndef __MILITAR_H__
#define __MILITAR_H__

#include "Enemy.h"

class CMilitar: public CEnemy
{
public:

  CMilitar(const string& _name) : CEnemy(_name),
                                  m_fRotateSpeed(0.0f)
                                  {m_Type = CEnemy::MILITAR;};

  


  ~CMilitar(void){};

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
