#pragma once

#include "EnemyInstance.h"

class CSniper: public CEnemyInstance
{
private:
  void                  Relase();
  
	bool                  m_bIsOk;
public:
  CSniper(void);
  ~CSniper(void) {Done();};

  bool                  Init( const Vect3f&   _vPos, 
                              const Vect3f&   _vRot, 
                              const float     _fMoveSpeed, 
                              const float     _fRotateSpeed, 
                              const float     _fHealth, 
                              const float     _fAimDistance);

  void                  Done() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};



private:

  float                 m_fAimDistance;

  typedef CEnemyInstance Inherited;
};

