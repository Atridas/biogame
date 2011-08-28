#include "base.h"


float Random01()
{
  return rand()/static_cast<float>(RAND_MAX);
}


float RandomNumber(float _fNumA, float _fNumB)
{
  float l_fNumber = Random01();

  return SIMPLE_INTERPOLATION(_fNumA, _fNumB, l_fNumber);
}

int RandomNumber(int _iNumA, int _iNumB)
{
  int l_iNumber;
  if(_iNumA > _iNumB)
  {
    //ofuscar coses mola
    _iNumA ^= _iNumB;
    _iNumB ^= _iNumA;
    _iNumA ^= _iNumB;
  }
  int l_iDiff = (_iNumB - _iNumA);
  l_iNumber = rand();

  int l_iLlindarMaxim = RAND_MAX / l_iDiff;
  l_iLlindarMaxim *= l_iDiff;
  while( l_iLlindarMaxim < l_iNumber )
  {
    l_iNumber = rand(); // això es fa per evitar esviaixaments
  }



  return ( l_iNumber % l_iDiff ) + _iNumA;
}


D3DXVECTOR3 InterpolateNumber(const D3DXVECTOR3& _vInicial, const D3DXVECTOR3& _vFinal,float _fTime,float _fTimeDelta)
{
  D3DXVECTOR3 l_vVectAux;
  l_vVectAux=_vInicial-_vFinal;
  l_vVectAux.x= l_vVectAux.x/_fTime*_fTimeDelta;
  l_vVectAux.y= l_vVectAux.y/_fTime*_fTimeDelta;
  l_vVectAux.z= l_vVectAux.z/_fTime*_fTimeDelta;
  l_vVectAux=_vInicial-l_vVectAux;
  return l_vVectAux;
}

Vect3f InterpolateNumber(const Vect3f& _vInicial, const Vect3f& _vFinal,float _fTime,float _fTimeDelta)
{
  Vect3f l_vVectAux;
  l_vVectAux=_vInicial-_vFinal;
  l_vVectAux.x= l_vVectAux.x/_fTime*_fTimeDelta;
  l_vVectAux.y= l_vVectAux.y/_fTime*_fTimeDelta;
  l_vVectAux.z= l_vVectAux.z/_fTime*_fTimeDelta;
  l_vVectAux=_vInicial-l_vVectAux;
  return l_vVectAux;
}

D3DXCOLOR InterpolateNumber(const D3DXCOLOR& _vInicial, const D3DXCOLOR& _vFinal,float _fTime,float _fTimeDelta)
{
  D3DXCOLOR l_vVectAux;
  l_vVectAux=_vInicial-_vFinal;
  //S'ha de provar si aixi tira b, o s'ha de fer cada valor per separat com tinc en el de direccio
  // si aixi funciona posar-ho tambe al de direccio
  l_vVectAux= l_vVectAux/_fTime*_fTimeDelta;
  l_vVectAux=_vInicial-l_vVectAux;
  return l_vVectAux;
}

float InterpolateNumber(float _fInicial, float _fFinal,float _fTime, float _fTimeDelta)
{
  float l_fAux;
  l_fAux=_fInicial-_fFinal;
  l_fAux= l_fAux/_fTime*_fTimeDelta;

  l_fAux=_fInicial-l_fAux;
  return l_fAux;
}
