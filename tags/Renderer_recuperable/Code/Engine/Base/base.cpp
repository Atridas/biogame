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
    l_iNumber = rand(); // aix� es fa per evitar esviaixaments
  }



  return ( l_iNumber % l_iDiff ) + _iNumA;
}

Vect3f RandomUnitVector()
{
  float l_fAnglePlaneXZ = RandomNumber(- FLOAT_PI_VALUE, FLOAT_PI_VALUE );
  float l_fAngleY = RandomNumber(- FLOAT_PI_VALUE / 2.f, FLOAT_PI_VALUE / 2.f );
  
  float aux = cos(l_fAngleY);

  float x = sin(l_fAnglePlaneXZ) * aux;
  float y = sin(l_fAngleY);
  float z = cos(l_fAnglePlaneXZ) * aux;

  return Vect3f(x,y,z);
}

Mat33f RandomRotationMatrix(float _fMaxAngle)
{
  Vect3f l_vDir = RandomUnitVector();
  float  l_fAngle = RandomNumber( - _fMaxAngle, _fMaxAngle );
  Mat33f m_;
  m_.FromAxisAngle(l_vDir, l_fAngle);
  return m_;
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



Mat33f GetFastestRotationFromDirToDir(const Vect3f& _vOriginalDir, const Vect3f& _vDestinationDir)
{
  assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetFastestRotationFromDirToDir _vOriginalDir i _vDestinationDir cal que siguin unitaris");

  Mat33f mRot_;
  float l_fAngleCos = _vOriginalDir * _vDestinationDir;
  if(abs(l_fAngleCos) < 0.99f)
  {
    //invertim si estem a m�s de 90�
    Vect3f l_vDir = _vOriginalDir ^_vDestinationDir;
    float  l_fAngleSin = l_vDir.Length();
    float l_fAngle = acos(l_fAngleCos);

    l_vDir.Normalize();
    mRot_.FromAxisAngle(l_vDir,l_fAngle);
  }
  else
  {
    mRot_.SetIdentity();
  }
  return mRot_;
}

Mat33f GetRotationFromOrientationToOrientation(
                              const Vect3f& _vOriginalDir,    const Vect3f& _vOriginalUp, 
                              const Vect3f& _vDestinationDir, const Vect3f& _vDestinationUp)
{
  assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalDir i _vDestinationDir cal que siguin unitaris");
  assert(abs(_vOriginalUp.SquaredLength() - 1.f)  < 0.01f && abs(_vDestinationUp.SquaredLength() - 1.f)  < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalUp i _vDestinationUp cal que siguin unitaris");

  //TODO 
  return GetFastestRotationFromDirToDir(_vOriginalDir, _vDestinationDir);
}

