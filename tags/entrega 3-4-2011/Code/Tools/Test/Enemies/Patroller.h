#pragma once

#include <vector>

#include "Math/Vector3.h"
#include "Math/MathTypes.h"

#include "EnemyInstance.h"

class CPatroller: public CEnemyInstance
{
private:
  void                  Release();
  
	bool                  m_bIsOk;
public:
  CPatroller(void);
  ~CPatroller(void) {Done();};

  bool                  Init( const Vect3f&   _vPos, 
                              const Vect3f&   _vRot, 
                              const float     _fMoveSpeed, 
                              const float     _fRotateSpeed, 
                              const float     _fHealth, 
                              const char*     _pcPath);

  void                  Done() {if(IsOk()) Release(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};



private:

  std::string           m_szPath;

  typedef CEnemyInstance Inherited;
};

