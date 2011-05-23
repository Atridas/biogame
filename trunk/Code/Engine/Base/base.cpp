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

//D3DXVECTOR3 InterPolaterNumber(D3DXVECTOR3& _vInicial, D3DXVECTOR3& _vFinal,float _fTime,float _fTimeDelta)
//{
//  D3DXVECTOR3 l_vVectAux;
//  l_vVectAux=_vInicial-_vFinal;
//  l_vVectAux.x= l_vVectAux.x/_fTime*_fTimeDelta;
//  l_vVectAux.y= l_vVectAux.y/_fTime*_fTimeDelta;
//  l_vVectAux.z= l_vVectAux.z/_fTime*_fTimeDelta;
//  l_vVectAux=_vInicial-l_vVectAux;
//  return l_vVectAux;
//}
//
//D3DXCOLOR InterPolaterNumber(D3DXCOLOR& _vInicial, D3DXCOLOR& _vFinal,float _fTime,float _fTimeDelta)
//{
//  D3DXCOLOR l_vVectAux;
//  l_vVectAux=_vInicial-_vFinal;
//  //S'ha de provar si aixi tira b, o s'ha de fer cada valor per separat com tinc en el de direccio
//  // si aixi funciona posar-ho tambe al de direccio
//  l_vVectAux= l_vVectAux/_fTime*_fTimeDelta;
//  l_vVectAux=_vInicial-l_vVectAux;
//  return l_vVectAux;
//}