#pragma once

#include "EnemyInstance.h"

class CCamera: public CEnemyInstance
{
private:
  void                  Release();
  
	bool                  m_bIsOk;
public:
  CCamera(void);
  virtual ~CCamera(void) {Done();};

  bool                  Init( const Vect3f&   _vPos, 
                              const Vect3f&   _vRot, 
                              const float     _fMoveSpeed, 
                              const float     _fRotateSpeed, 
                              const float     _fHealth, 
                              const Vect3f&   _vMaxAngle,
                              const Vect3f&   _vMinAngle);

  void                  Done() {if(IsOk()) Release(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};



private:

  Vect3f                m_vMaxAngle;
  Vect3f                m_vMinAngle;

  typedef CEnemyInstance Inherited;
};

