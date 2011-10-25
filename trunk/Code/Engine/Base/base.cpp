#include "base.h"
#include "Math/Quaternion.h"

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
    //_iNumA ^= _iNumB;
    //_iNumB ^= _iNumA;
    //_iNumA ^= _iNumB;
    int auxA = _iNumA;
    int auxB = _iNumB;
    _iNumA = auxB;
    _iNumB = auxA;
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

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


void ConcatenateRotations(const Vect3f& _vAxis1, float _fAngle1, 
                          const Vect3f& _vAxis2, float _fAngle2, 
                          Vect3f& vAxis_, float& fAngle_)
{
  /*
  float qx1, qy1, qz1, qw1;
  float qx2, qy2, qz2, qw2;
  
  float sin1 = sin(_fAngle1 * .5f);
  float cos1 = cos(_fAngle1 * .5f);
  float sin2 = sin(_fAngle2 * .5f);
  float cos2 = cos(_fAngle2 * .5f);

	qx1 = _vAxis1.x * sin1;
	qy1 = _vAxis1.y * sin1;
	qz1 = _vAxis1.z * sin1;
	qw1 = cos1;

	qx2 = _vAxis2.x * sin2;
	qy2 = _vAxis2.y * sin2;
	qz2 = _vAxis2.z * sin2;
	qw2 = cos2;
		
	vAxis_.x = qw2 * qx1 + qx2 * qw1 + qy2 * qz1 - qz2 * qy1;
	vAxis_.y = qw2 * qy1 - qx2 * qz1 + qy2 * qw1 + qz2 * qx1;
	vAxis_.z = qw2 * qz1 + qx2 * qy1 - qy2 * qx1 + qz2 * qw1;
	fAngle_  = qw2 * qw1 - qx2 * qx1 - qy2 * qy1 - qz2 * qz1;

  */
  Quat4f q1(_vAxis1, _fAngle1);
  Quat4f q2(_vAxis2, _fAngle2);

  Quat4f q3 = q2 * q1;

  q3.GetAxisAngle(vAxis_, fAngle_);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


Mat33f GetFastestRotationFromDirToDir(const Vect3f& _vOriginalDir, const Vect3f& _vDestinationDir)
{
  /*assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetFastestRotationFromDirToDir _vOriginalDir i _vDestinationDir cal que siguin unitaris");

  Mat33f mRot_;
  float l_fAngleCos = _vOriginalDir * _vDestinationDir;
  if(abs(l_fAngleCos) < 0.99f)
  {
    //invertim si estem a més de 90º
    Vect3f l_vDir = _vOriginalDir ^_vDestinationDir;
    float  l_fAngleSin = l_vDir.Length();
    float l_fAngle = acos(l_fAngleCos);

    l_vDir.Normalize();
    mRot_.FromAxisAngle(l_vDir,l_fAngle);
  }
  else if(l_fAngleCos < 0)
  {
    Vect3f l_vDir = _vOriginalDir ^ Vect3f(1,0,0);
    if(l_vDir.SquaredLength() < 0.01f)
    {
      l_vDir = _vOriginalDir ^ Vect3f(0,1,0);
    }
    
    float l_fAngle = FLOAT_PI_VALUE;

    l_vDir.Normalize();
    mRot_.FromAxisAngle(l_vDir,l_fAngle);
  }
  else
  {
    mRot_.SetIdentity();
  }
  return mRot_;*/


  
  Vect3f vAxis_;
  float fAngle_;
  GetFastestRotationFromDirToDir(_vOriginalDir, _vDestinationDir,
                                 vAxis_, fAngle_);

  Mat33f m2;
  m2.FromAxisAngle(vAxis_, fAngle_);
  return m2;
}

Mat33f GetRotationFromOrientationToOrientation(
                              const Vect3f& _vOriginalDir,    const Vect3f& _vOriginalUp, 
                              const Vect3f& _vDestinationDir, const Vect3f& _vDestinationUp)
{
  /*
  assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalDir i _vDestinationDir cal que siguin unitaris");
  assert(abs(_vOriginalUp.SquaredLength() - 1.f)  < 0.01f && abs(_vDestinationUp.SquaredLength() - 1.f)  < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalUp i _vDestinationUp cal que siguin unitaris");

  Mat33f m1 = GetFastestRotationFromDirToDir(_vOriginalDir, _vDestinationDir);

  Vect3f l_vTransformedUp = m1 * _vOriginalUp;

  Mat33f m2 = GetFastestRotationFromDirToDir(l_vTransformedUp, _vDestinationUp);

  return m2 * m1;*/

  Vect3f vAxis_;
  float fAngle_;
  GetRotationFromOrientationToOrientation(_vOriginalDir, _vOriginalUp, 
                                          _vDestinationDir, _vDestinationUp,
                                          vAxis_, fAngle_);

  Mat33f m2;
  m2.FromAxisAngle(vAxis_, fAngle_);
  return m2;
}


void GetFastestRotationFromDirToDir(const Vect3f& _vOriginalDir, const Vect3f& _vDestinationDir,
                                          Vect3f& vAxis_, float& fAngle_)
{
  assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetFastestRotationFromDirToDir _vOriginalDir i _vDestinationDir cal que siguin unitaris");

  float l_fAngleCos = _vOriginalDir * _vDestinationDir;
  if(abs(l_fAngleCos) < 0.99f)
  {
    //invertim si estem a més de 90º
    vAxis_ = _vOriginalDir ^_vDestinationDir;
    float  l_fAngleSin = vAxis_.Length();
    fAngle_ = acos(l_fAngleCos);

    vAxis_.Normalize();
  }
  else if(l_fAngleCos < 0)
  {
    vAxis_ = _vOriginalDir ^ Vect3f(1,0,0);
    if(vAxis_.SquaredLength() < 0.01f)
    {
      vAxis_ = _vOriginalDir ^ Vect3f(0,1,0);
    }
    
    fAngle_ = FLOAT_PI_VALUE;

    vAxis_.Normalize();
  }
  else
  {
    vAxis_  = Vect3f(0,0,0);
    fAngle_ = 0;
  }
}

void GetRotationFromOrientationToOrientation(
                              const Vect3f& _vOriginalDir,    const Vect3f& _vOriginalUp, 
                              const Vect3f& _vDestinationDir, const Vect3f& _vDestinationUp,
                                    Vect3f& vAxis_, float& fAngle_)
{
  assert(abs(_vOriginalDir.SquaredLength() - 1.f) < 0.01f && abs(_vDestinationDir.SquaredLength() - 1.f) < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalDir i _vDestinationDir cal que siguin unitaris");
  assert(abs(_vOriginalUp.SquaredLength() - 1.f)  < 0.01f && abs(_vDestinationUp.SquaredLength() - 1.f)  < 0.01f && "GetRotationFromOrientationToOrientation _vOriginalUp i _vDestinationUp cal que siguin unitaris");

  Vect3f l_vAxis , l_vAxis2 ;
  float  l_fAngle, l_fAngle2;

  GetFastestRotationFromDirToDir(_vOriginalDir, _vDestinationDir, l_vAxis, l_fAngle);

  Mat33f m1;
  m1.FromAxisAngle(l_vAxis, l_fAngle);

  Vect3f l_vTransformedUp = m1 * _vOriginalUp;

  GetFastestRotationFromDirToDir(l_vTransformedUp, _vDestinationUp, l_vAxis2, l_fAngle2);

  ConcatenateRotations(l_vAxis , l_fAngle, 
                       l_vAxis2, l_fAngle2, 
                       vAxis_,   fAngle_);
}
