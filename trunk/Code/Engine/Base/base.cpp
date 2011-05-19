#include "base.h"

float RandomNumber(float _fMin, float _fMax)
{
  float l_fNumber;
  if(_fMin> _fMax)
  {
    float l_fAux = _fMax;
    _fMax = _fMin;
    _fMin=l_fAux;
  }
  l_fNumber=( (rand()/static_cast<float>(RAND_MAX)) * (_fMax - _fMin) + _fMin);

  return l_fNumber;
}

int RandomNumber(int _iMin, int _iMax)
{
  int l_iNumber;
  if(_iMin > _iMax)
  {
    //ofuscar coses mola
    _iMin ^= _iMax;
    _iMax ^= _iMin;
    _iMin ^= _iMax;
  }
  //millorar això per no esbiaixar els resultats
  l_iNumber=( rand() % (_iMax - _iMin) + _iMin) ;// / RAND_MAX;

  return l_iNumber;
}

D3DXVECTOR3 InterPolaterNumber(D3DXVECTOR3& _vInicial, D3DXVECTOR3& _vFinal,float _fTime)
{
  _vFinal = _vInicial-(_vFinal*_fTime);
  return _vFinal;
}