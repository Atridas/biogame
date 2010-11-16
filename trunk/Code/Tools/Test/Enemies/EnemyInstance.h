#pragma once


#include "Math/Vector3.h"
#include "Math/MathTypes.h"

class CEnemyInstance
{
private:
  void                  Release();
  
	bool                  m_bIsOk;
public:
  CEnemyInstance(void);
  ~CEnemyInstance(void) {Done();};

  bool                  Init( const Vect3f&   _vPos, 
                              const Vect3f&   _vRot, 
                              const float     _fMoveSpeed, 
                              const float     _fRotateSpeed, 
                              const float     _fHealth);

  void                  Done() {if(IsOk()) Release(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};



private:

  Vect3f                m_vPos;
  Vect3f                m_vRot;
  float                 m_fMoveSpeed;
  float                 m_fRotateSpeed;
  float                 m_fHealth;
};

