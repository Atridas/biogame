#include "base.h"
// Disable warning for loss of data
#pragma warning( disable : 4244 )  

//main include files
#include "Frustum.h"
#include "Camera.h"


void CFrustum::Update(const D3DXMATRIX &clip) 
{
 	// Extract the numbers for the RIGHT plane 
	m_frustum[0][0] = clip._14 - clip._11;
	m_frustum[0][1] = clip._24 - clip._21;
	m_frustum[0][2] = clip._34 - clip._31;
	m_frustum[0][3] = clip._44 - clip._41;

	// Normalize the result 
	float t = 1.0f / sqrtf( (m_frustum[0][0] * m_frustum[0][0]) + (m_frustum[0][1] * m_frustum[0][1]) + (m_frustum[0][2] * m_frustum[0][2]) );
	m_frustum[0][0] *= t;
	m_frustum[0][1] *= t;
	m_frustum[0][2] *= t;
	m_frustum[0][3] *= t;

	// Extract the numbers for the LEFT plane 
	m_frustum[1][0] = clip._14 + clip._11;
	m_frustum[1][1] = clip._24 + clip._21;
	m_frustum[1][2] = clip._34 + clip._31;
	m_frustum[1][3] = clip._44 + clip._41;

	// Normalize the result 
	t = 1.0f / sqrtf( (m_frustum[1][0] * m_frustum[1][0]) + (m_frustum[1][1] * m_frustum[1][1]) + (m_frustum[1][2] * m_frustum[1][2]) );
	m_frustum[1][0] *= t;
	m_frustum[1][1] *= t;
	m_frustum[1][2] *= t;
	m_frustum[1][3] *= t;

	// Extract the BOTTOM plane 
	m_frustum[2][0] = clip._14 + clip._12;
	m_frustum[2][1] = clip._24 + clip._22;
	m_frustum[2][2] = clip._34 + clip._32;
	m_frustum[2][3] = clip._44 + clip._42;

	// Normalize the result 
	t = 1.0f / sqrtf( (m_frustum[2][0] * m_frustum[2][0]) + (m_frustum[2][1] * m_frustum[2][1]) + (m_frustum[2][2] * m_frustum[2][2]) );
	m_frustum[2][0] *= t;
	m_frustum[2][1] *= t;
	m_frustum[2][2] *= t;
	m_frustum[2][3] *= t;

	// Extract the TOP plane 
	m_frustum[3][0] = clip._14 - clip._12;
	m_frustum[3][1] = clip._24 - clip._22;
	m_frustum[3][2] = clip._34 - clip._32;
	m_frustum[3][3] = clip._44 - clip._42;

	// Normalize the result 
	t = 1.0f / sqrtf( (m_frustum[3][0] * m_frustum[3][0]) + (m_frustum[3][1] * m_frustum[3][1]) + (m_frustum[3][2] * m_frustum[3][2]) );
	m_frustum[3][0] *= t;
	m_frustum[3][1] *= t;
	m_frustum[3][2] *= t;
	m_frustum[3][3] *= t;

	// Extract the FAR plane 
	m_frustum[4][0] = clip._14 - clip._13;
	m_frustum[4][1] = clip._24 - clip._23;
	m_frustum[4][2] = clip._34 - clip._33;
	m_frustum[4][3] = clip._44 - clip._43;

	// Normalize the result 
	t = 1.0f / sqrtf( (m_frustum[4][0] * m_frustum[4][0]) + (m_frustum[4][1] * m_frustum[4][1]) + (m_frustum[4][2] * m_frustum[4][2]) );
	m_frustum[4][0] *= t;
	m_frustum[4][1] *= t;
	m_frustum[4][2] *= t;
	m_frustum[4][3] *= t;

	// Extract the NEAR plane 
	m_frustum[5][0] = clip._14 + clip._13;
	m_frustum[5][1] = clip._24 + clip._23;
	m_frustum[5][2] = clip._34 + clip._33;
	m_frustum[5][3] = clip._44 + clip._43;

	// Normalize the result 
	t = 1.0f / sqrtf( (m_frustum[5][0] * m_frustum[5][0]) + (m_frustum[5][1] * m_frustum[5][1]) + (m_frustum[5][2] * m_frustum[5][2]) );
	m_frustum[5][0] *= t;
	m_frustum[5][1] *= t;
	m_frustum[5][2] *= t;
	m_frustum[5][3] *= t;
}

void ComputePlane(const Vect3f& p0,const Vect3f& p1,const Vect3f& p2, float* plane)
{
  Vect3f v = p1 - p0;
  Vect3f u = p2 - p0;

  Vect3f n = (v ^ u).GetNormalized();
  
  plane[0] = n.x;
  plane[1] = n.y;
  plane[2] = n.z;
  plane[3] = -(n * p0);
}

void CFrustum::Update(const CCamera* _pCamera) 
{
  float  l_fFov    = _pCamera->GetFov();
	float  l_fAspect = _pCamera->GetAspectRatio();
	float  l_fZNear  = _pCamera->GetZn();
	float  l_fZFar   = _pCamera->GetZf();

  Vect3f l_vPos    = _pCamera->GetEye();
  Vect3f l_vUp     = _pCamera->GetVecUp().Normalize();
  Vect3f l_vDir    = (_pCamera->GetLookAt() - l_vPos).Normalize();

  Vect3f l_vRight  = (l_vDir   ^ l_vUp ).Normalize();
         l_vUp     = (l_vRight ^ l_vDir).Normalize();

  
  float l_fHNear = 2 * tan(l_fFov * .5) * l_fZNear;
  //float l_fHNear = tan(l_fFov * .5) * l_fZNear; //Alçada del planol "near"
	float l_fWNear = l_fHNear * l_fAspect;

  float l_fHFar = 2 * tan(l_fFov * .5) * l_fZFar;
  //float l_fHFar = tan(l_fFov * .5) * l_fZFar; //Alçada del planol "far"
	float l_fWFar = l_fHFar * l_fAspect;


  // (F) = far, (c) = center, (n) = near, (l) = left, (r) = right, (t) = top, (b) = bottom
  Vect3f l_vFc = l_vPos + l_vDir * l_fZFar;

	Vect3f l_vFtl = l_vFc + (l_vUp * l_fHFar * .5f) + (l_vRight * l_fWFar * .5f);
  Vect3f l_vFtr = l_vFc + (l_vUp * l_fHFar * .5f) - (l_vRight * l_fWFar * .5f);
  Vect3f l_vFbl = l_vFc - (l_vUp * l_fHFar * .5f) + (l_vRight * l_fWFar * .5f);
  Vect3f l_vFbr = l_vFc - (l_vUp * l_fHFar * .5f) - (l_vRight * l_fWFar * .5f);

  Vect3f l_vNc = l_vPos + l_vDir * l_fZNear;

  Vect3f l_vNtl = l_vNc + (l_vUp * l_fHNear * .5f) + (l_vRight * l_fWNear * .5f);
  Vect3f l_vNtr = l_vNc + (l_vUp * l_fHNear * .5f) - (l_vRight * l_fWNear * .5f);
  Vect3f l_vNbl = l_vNc - (l_vUp * l_fHNear * .5f) + (l_vRight * l_fWNear * .5f);
  Vect3f l_vNbr = l_vNc - (l_vUp * l_fHNear * .5f) - (l_vRight * l_fWNear * .5f);
  
  //right
  ComputePlane(l_vNbr, l_vFbr, l_vNtr,  m_frustum[0]);
  //left
  ComputePlane(l_vNtl, l_vFtl, l_vNbl, m_frustum[1]);
  
  //bottom
  ComputePlane(l_vNbl, l_vFbr, l_vNbr, m_frustum[2]);
  //top
  ComputePlane(l_vNtr, l_vFtr, l_vNtl, m_frustum[3]);
  
  //far
  ComputePlane(l_vFbl, l_vFtr, l_vFbr, m_frustum[4]);
  //near
  ComputePlane(l_vNbr, l_vNtr, l_vNbl, m_frustum[5]);
  
}

bool CFrustum::SphereVisible(const D3DXVECTOR3 &center,float radius) const 
{
	for (int i=0; i < 6; ++i)	
	{
		if (( (m_frustum[i][0]*center.x) + (m_frustum[i][1]*center.y) + 
			 (m_frustum[i][2]*center.z) + (m_frustum[i][3] )) <= -radius )
		{
				return false;
		}
	}

	return true;
}

bool CFrustum::BoxVisibleByVertexs( const Vect3f* points) const 
{  
  int iInCount;
	for(int p=0; p<6; p++)
	{
		iInCount = 8;
		for(int i=0; i<8; i++)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p][0]*points[i].x + m_frustum[p][1]*points[i].y + m_frustum[p][2]*points[i].z + m_frustum[p][3]) <= 0 )
				--iInCount;
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}
	
	// Si todos los puntos están dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

bool CFrustum::BoxVisibleByVertexsNoNear	( const Vect3f* points) const
{  
  int iInCount;
	for(int p=0; p<5; p++) //No tallem el near plane
	{
		iInCount = 8;
		for(int i=0; i<8; i++)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p][0]*points[i].x + m_frustum[p][1]*points[i].y + m_frustum[p][2]*points[i].z + m_frustum[p][3]) <= 0 )
				--iInCount;
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}
	
	// Si todos los puntos están dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

bool CFrustum::BoxVisible( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min) const 
{
	float points[24];

	// calculamos los 8 puntos de la bounding box
	points[0] = max.x;
	points[1] = max.y;
	points[2] = max.z;

	points[3] = min.x;
	points[4] = max.y;
	points[5] = max.z;

	points[6] = min.x;
	points[7] = min.y;
	points[8] = max.z;

	points[9] = max.x;
	points[10] = min.y;
	points[11] = max.z;

	points[12] = min.x;
	points[13] = min.y;
	points[14] = min.z;

	points[15] = max.x;
	points[16] = min.y;
	points[17] = min.z;

	points[18] = max.x;
	points[19] = max.y;
	points[20] = min.z;

	points[21] = min.x;
	points[22] = max.y;
	points[23] = min.z;

	int iInCount;
	for(int p=0; p<6; p++)
	{
		iInCount = 8;
		for(int i=0; i<8; i++)
		{
			// Probamos el punto contra todos los planos
			if( (m_frustum[p][0]*points[3*i] + m_frustum[p][1]*points[3*i+1] + m_frustum[p][2]*points[3*i+2] + m_frustum[p][3]) <= 0 )
			{
				--iInCount;
			}
		}
		// ¿Están todos los puntos fuera?
		if (iInCount == 0)
			return false;
	}
	
	// Si algun punto está dentro, entonces la caja
	// está dentro del frustum o parcialmente
	return true;
}

void  CFrustum::Update( const Vect3f& _vCameraEye, const Vect3f* _vPoints, uint32 _iNumPoints)
{
  Vect3f* l_vDirections = new Vect3f[_iNumPoints];

  for(uint32 i = 0; i < _iNumPoints; ++i)
  {
    l_vDirections[i] = _vPoints[i] - _vCameraEye;
    l_vDirections[i].Normalize();
  }

  //només els plans rigt/left / bottom/top
  for(int i = 0; i < 4; ++i)
  {
    UpdatePlane(i, _vCameraEye, _vPoints, l_vDirections, _iNumPoints);
  }


  delete l_vDirections;
}

inline float DistPointPlane(const Vect3f& _vPoint, const float _pfPlane[4])
{
  return _vPoint.x * _pfPlane[0] + _vPoint.y * _pfPlane[1] + _vPoint.z * _pfPlane[2] + _pfPlane[3];
}

inline bool IsPointInPlane(const Vect3f& _vPoint, const float _pfPlane[4])
{
  return abs(DistPointPlane(_vPoint,_pfPlane)) < .05f;
}

inline bool IsPointInPlane(float _pfPoint[3] , const float _pfPlane[4])
{
  return IsPointInPlane(Vect3f(_pfPoint[0], _pfPoint[1], _pfPoint[2]), _pfPlane);
}

void CFrustum::UpdatePlane(uint32 _iPlane, const Vect3f& _vCameraEye, const Vect3f* _vPoints, const Vect3f* _vDirections, uint32 _iNumPoints)
{
  Vect3f l_vPlaneNormal(m_frustum[_iPlane][0], m_frustum[_iPlane][1], m_frustum[_iPlane][2]);
  assert(l_vPlaneNormal.SquaredLength() > 0.99f && l_vPlaneNormal.SquaredLength() < 1.01f);

  uint32 l_iPoint1 = 0, l_iPoint2 = 0;
  float  l_fMaxDot = -1.f;

  for(uint32 i = 0; i < _iNumPoints-1; ++i)
  {
    Vect3f l_vDir1 = _vDirections[i];
    if(_vPoints[i] * l_vPlaneNormal + m_frustum[_iPlane][3] < 0) 
      return; //anular el planol, ja que ens sortim de l'anterior
    for(uint32 j = i+1; j < _iNumPoints; ++j)
    {
      if(_vPoints[j] * l_vPlaneNormal + m_frustum[_iPlane][3] < 0) 
        return; //anular el planol, ja que ens sortim de l'anterior
      Vect3f l_vDir2 = _vDirections[j];
      Vect3f l_vNormal = (l_vDir1 ^ l_vDir2).GetNormalized();

      float l_fDot = l_vNormal * l_vPlaneNormal;
      if(abs(l_fDot) > l_fMaxDot)
      {
        l_fMaxDot = abs(l_fDot);
        if(l_fDot > 0)
        {
          l_iPoint1 = i;
          l_iPoint2 = j;
        }
        else
        {
          l_iPoint1 = j;
          l_iPoint2 = i;
        }
      }
    }
  }
  
  if(l_iPoint1 != l_iPoint2)
  {
    ComputePlane(_vCameraEye, _vPoints[l_iPoint1],_vPoints[l_iPoint2], m_frustum[_iPlane]);
    assert(Vect3f(m_frustum[_iPlane][0], m_frustum[_iPlane][1], m_frustum[_iPlane][2]) * l_vPlaneNormal > 0);

  
    assert(IsPointInPlane(_vPoints[l_iPoint2], m_frustum[_iPlane]));
    assert(IsPointInPlane(_vPoints[l_iPoint1], m_frustum[_iPlane]));
    assert(IsPointInPlane(_vCameraEye, m_frustum[_iPlane]));
  }
}


Vect3f SolveMatrix(const float _fPlane1[4], const float _fPlane2[4], const float _fPlane3[4])
{
  //simple algorisme de diagonalitzar matrius

  float l_Row1[4] = {_fPlane1[0], _fPlane1[1], _fPlane1[2], -_fPlane1[3]};
  float l_Row2[4] = {_fPlane2[0], _fPlane2[1], _fPlane2[2], -_fPlane2[3]};
  float l_Row3[4] = {_fPlane3[0], _fPlane3[1], _fPlane3[2], -_fPlane3[3]};
  
  float l_fAux;

  uint16 l_iCol1 = 0;
  while(l_Row1[l_iCol1] == 0.0f)
  {
    ++l_iCol1;
    if(l_iCol1 == 3)
      return Vect3f(0);
  }
  
  l_fAux = l_Row2[l_iCol1] / l_Row1[l_iCol1];
  for(int i = 0; i < 4; ++i) l_Row2[i] -= l_fAux * l_Row1[i];
  l_fAux = l_Row3[l_iCol1] / l_Row1[l_iCol1];
  for(int i = 0; i < 4; ++i) l_Row3[i] -= l_fAux * l_Row1[i];

  uint16 l_iCol2 = 0;
  while(l_iCol1 == l_iCol2 || l_Row2[l_iCol2] == 0.0f)
  {
    ++l_iCol2;
    if(l_iCol2 == 3)
      return Vect3f(0);
  }
  
  l_fAux = l_Row3[l_iCol2] / l_Row2[l_iCol2];
  for(int i = 0; i < 4; ++i) l_Row3[i] -= l_fAux * l_Row2[i];
  l_fAux = l_Row1[l_iCol2] / l_Row2[l_iCol2];
  for(int i = 0; i < 4; ++i) l_Row1[i] -= l_fAux * l_Row2[i];
  
  uint16 l_iCol3 = 0;
  while(l_iCol1 == l_iCol3 || l_iCol2 == l_iCol3 || l_Row3[l_iCol3] == 0.0f)
  {
    ++l_iCol3;
    if(l_iCol3 == 3)
      return Vect3f(0);
  }

  
  l_fAux = l_Row2[l_iCol3] / l_Row3[l_iCol3];
  for(int i = 0; i < 4; ++i) l_Row2[i] -= l_fAux * l_Row3[i];
  l_fAux = l_Row1[l_iCol3] / l_Row3[l_iCol3];
  for(int i = 0; i < 4; ++i) l_Row1[i] -= l_fAux * l_Row3[i];

  float res[3];
  res[l_iCol1] = l_Row1[3]/l_Row1[l_iCol1];
  res[l_iCol2] = l_Row2[3]/l_Row2[l_iCol2];
  res[l_iCol3] = l_Row3[3]/l_Row3[l_iCol3];

  return Vect3f(res[0], res[1], res[2]); 
}

void CFrustum::GetPoints( Vect3f* _vPoints ) const
{
  _vPoints[0] = SolveMatrix(m_frustum[2], m_frustum[1], m_frustum[4]);
  _vPoints[1] = SolveMatrix(m_frustum[2], m_frustum[0], m_frustum[4]);
  _vPoints[2] = SolveMatrix(m_frustum[2], m_frustum[1], m_frustum[5]);
  _vPoints[3] = SolveMatrix(m_frustum[2], m_frustum[0], m_frustum[5]);
  _vPoints[4] = SolveMatrix(m_frustum[3], m_frustum[1], m_frustum[4]);
  _vPoints[5] = SolveMatrix(m_frustum[3], m_frustum[0], m_frustum[4]);
  _vPoints[6] = SolveMatrix(m_frustum[3], m_frustum[1], m_frustum[5]);
  _vPoints[7] = SolveMatrix(m_frustum[3], m_frustum[0], m_frustum[5]);
}

void CFrustum::GetPoints( Vect3f* _vPoints )
{
  _vPoints[0] = SolveMatrix(m_frustum[2], m_frustum[1], m_frustum[4]);
  _vPoints[1] = SolveMatrix(m_frustum[2], m_frustum[0], m_frustum[4]);
  _vPoints[2] = SolveMatrix(m_frustum[2], m_frustum[1], m_frustum[5]);
  _vPoints[3] = SolveMatrix(m_frustum[2], m_frustum[0], m_frustum[5]);
  _vPoints[4] = SolveMatrix(m_frustum[3], m_frustum[1], m_frustum[4]);
  _vPoints[5] = SolveMatrix(m_frustum[3], m_frustum[0], m_frustum[4]);
  _vPoints[6] = SolveMatrix(m_frustum[3], m_frustum[1], m_frustum[5]);
  _vPoints[7] = SolveMatrix(m_frustum[3], m_frustum[0], m_frustum[5]);
  
  //TODO encara hi ha alguns problemes de planos que es fan paralels misteriosament, etc...
  /*
  assert(IsPointInPlane(_vPoints[0], m_frustum[2]));
  assert(IsPointInPlane(_vPoints[0], m_frustum[1]));
  assert(IsPointInPlane(_vPoints[0], m_frustum[4]));
  
  assert(IsPointInPlane(_vPoints[1], m_frustum[2]));
  assert(IsPointInPlane(_vPoints[1], m_frustum[0]));
  assert(IsPointInPlane(_vPoints[1], m_frustum[4]));
  
  assert(IsPointInPlane(_vPoints[2], m_frustum[2]));
  assert(IsPointInPlane(_vPoints[2], m_frustum[1]));
  assert(IsPointInPlane(_vPoints[2], m_frustum[5]));
  
  assert(IsPointInPlane(_vPoints[3], m_frustum[2]));
  assert(IsPointInPlane(_vPoints[3], m_frustum[0]));
  assert(IsPointInPlane(_vPoints[3], m_frustum[5]));
  
  
  assert(IsPointInPlane(_vPoints[4], m_frustum[3]));
  assert(IsPointInPlane(_vPoints[4], m_frustum[1]));
  assert(IsPointInPlane(_vPoints[4], m_frustum[4]));
  
  assert(IsPointInPlane(_vPoints[5], m_frustum[3]));
  assert(IsPointInPlane(_vPoints[5], m_frustum[0]));
  assert(IsPointInPlane(_vPoints[5], m_frustum[4]));
  
  assert(IsPointInPlane(_vPoints[6], m_frustum[3]));
  assert(IsPointInPlane(_vPoints[6], m_frustum[1]));
  assert(IsPointInPlane(_vPoints[6], m_frustum[5]));
  
  assert(IsPointInPlane(_vPoints[7], m_frustum[3]));
  assert(IsPointInPlane(_vPoints[7], m_frustum[0]));
  assert(IsPointInPlane(_vPoints[7], m_frustum[5]));
  */
}

inline Vect3f PuntAresta(const Vect3f _vPoints[8], int n)
{

  switch(n)
  {
  case 0 : return (_vPoints[0] + _vPoints[1]) * .5f;
  case 1 : return (_vPoints[1] + _vPoints[3]) * .5f;
  case 2 : return (_vPoints[3] + _vPoints[2]) * .5f;
  case 3 : return (_vPoints[2] + _vPoints[0]) * .5f;
         
  case 4 : return (_vPoints[4] + _vPoints[5]) * .5f;
  case 5 : return (_vPoints[5] + _vPoints[7]) * .5f;
  case 6 : return (_vPoints[7] + _vPoints[6]) * .5f;
  case 7 : return (_vPoints[6] + _vPoints[4]) * .5f;
         
  case 8 : return (_vPoints[0] + _vPoints[4]) * .5f;
  case 9 : return (_vPoints[1] + _vPoints[5]) * .5f;
  case 10: return (_vPoints[3] + _vPoints[7]) * .5f;
  case 11: return (_vPoints[2] + _vPoints[6]) * .5f;
  }
  assert(false);
  return Vect3f(0);
}

inline Vect3f PuntAresta1(const Vect3f _vPoints[8], uint32 n)
{

  switch(n)
  {
  case 0 : return _vPoints[0];
  case 1 : return _vPoints[1];
  case 2 : return _vPoints[3];
  case 3 : return _vPoints[2];
         
  case 4 : return _vPoints[4];
  case 5 : return _vPoints[5];
  case 6 : return _vPoints[7];
  case 7 : return _vPoints[6];
         
  case 8 : return _vPoints[0];
  case 9 : return _vPoints[1];
  case 10: return _vPoints[3];
  case 11: return _vPoints[2];
  }
  assert(false);
  return Vect3f(0);
}

inline Vect3f PuntAresta2(const Vect3f _vPoints[8], uint32 n)
{

  switch(n)
  {
  case 0 : return _vPoints[1];
  case 1 : return _vPoints[3];
  case 2 : return _vPoints[2];
  case 3 : return _vPoints[0];
         
  case 4 : return _vPoints[5];
  case 5 : return _vPoints[7];
  case 6 : return _vPoints[6];
  case 7 : return _vPoints[4];
         
  case 8 : return _vPoints[4];
  case 9 : return _vPoints[5];
  case 10: return _vPoints[7];
  case 11: return _vPoints[6];
  }
  assert(false);
  return Vect3f(0);
}
/*
inline uint32 GetNumArestaVertical(uint32 n)
{
  assert(n < 4);
  return n + 8;
}

inline uint32 GetNumArestaHoritzontal(uint32 n)
{
  assert(n < 8);
  return n;
}*/

Vect3f ProjectLineOnPlane(const Vect3f& _vPoint, const float _pfPlane[4])
{
  float l_fParam = _vPoint.x * _pfPlane[0] + _vPoint.y * _pfPlane[1] + _vPoint.z * _pfPlane[2] + _pfPlane[3];
  l_fParam /= _pfPlane[0] * _pfPlane[0] + _pfPlane[1] * _pfPlane[1] + _pfPlane[2] * _pfPlane[2];

  return Vect3f(  _vPoint.x - _pfPlane[0] * l_fParam, 
                  _vPoint.y - _pfPlane[1] * l_fParam,
                  _vPoint.z - _pfPlane[2] * l_fParam );
}

#define SWAP(a, b) a ^= b; b ^= a; a ^= b

void Select4Edges(const Vect3f _vPoints[8], const Vect3f& _ParalelVector, int iResult_[4])
{
  assert(_ParalelVector.SquaredLength() > 0.95f && _ParalelVector.SquaredLength() < 1.05f);

  float l_fCos[4] = { 0, 0, 0, 0};
  float l_fAux;

  for(int i = 0; i < 12; ++i)
  {
    Vect3f l_vPunt1 = PuntAresta1(_vPoints, i);
    Vect3f l_vPunt2 = PuntAresta2(_vPoints, i);

    Vect3f l_vEdge = (l_vPunt2 - l_vPunt1).GetNormalized();

    float l_fCosEdge = abs(l_vEdge * _ParalelVector);
    if(l_fCosEdge > l_fCos[0])
    {
      l_fCos[0] = l_fCosEdge;
      iResult_[0] = i;
      if(l_fCos[0] > l_fCos[1])
      {
        l_fAux    = l_fCos[0];
        l_fCos[0] = l_fCos[1];
        l_fCos[1] = l_fAux;

        SWAP(iResult_[0], iResult_[1]);
        if(l_fCos[1] > l_fCos[2])
        {
          l_fAux    = l_fCos[2];
          l_fCos[2] = l_fCos[1];
          l_fCos[1] = l_fAux;

          SWAP(iResult_[2], iResult_[1]);
          if(l_fCos[2] > l_fCos[3])
          {
            l_fAux    = l_fCos[2];
            l_fCos[2] = l_fCos[3];
            l_fCos[3] = l_fAux;

            SWAP(iResult_[2], iResult_[3]);
          }
        }
      }
    }
  }
}


void CFrustum::ClosePlane(const Vect3f& _vEye, const Vect3f& _vDir, const Vect3f _vPoints[8], const int _iArestes[4], uint32 _iPlane)
{
  assert(_iPlane < 4);
  Vect3f l_DirProjected = (ProjectLineOnPlane(_vEye + _vDir, m_frustum[_iPlane]) - _vEye).GetNormalized();

  int l_iArestaProx = 0;
  float l_fCosArestaProx = -1;


  for(uint32 i = 0; i < 4; ++i)
  {
    int l_iAresta = _iArestes[i];
    Vect3f l_vPuntAresta = PuntAresta(_vPoints, l_iAresta);
    
    if(DistPointPlane(l_vPuntAresta, m_frustum[_iPlane]) < 0)
    {
      return; // descartem tancar el pla
    }
    
    Vect3f l_vDirAresta  = (l_vPuntAresta - _vEye).GetNormalized();

    float l_fCosAresta = l_vDirAresta * l_DirProjected;
    if(l_fCosAresta > l_fCosArestaProx)
    {
      l_fCosArestaProx = l_fCosAresta;
      l_iArestaProx = l_iAresta;
    }
  }
  
  Vect3f l_vPunt1 = PuntAresta1(_vPoints, l_iArestaProx);
  Vect3f l_vPunt2 = PuntAresta2(_vPoints, l_iArestaProx);

  Vect3f l_vOldPlaneNormal = Vect3f(m_frustum[_iPlane][0], m_frustum[_iPlane][1], m_frustum[_iPlane][2]);

  ComputePlane(_vEye, l_vPunt1, l_vPunt2, m_frustum[_iPlane]);
  
  Vect3f l_vNewPlaneNormal = Vect3f(m_frustum[_iPlane][0], m_frustum[_iPlane][1], m_frustum[_iPlane][2]);

  if(l_vOldPlaneNormal * l_vNewPlaneNormal < 0) for(int i = 0; i < 4; ++i) m_frustum[_iPlane][i] = -m_frustum[_iPlane][i];
}

void  CFrustum::Update( const CCamera* _pCamera, const Vect3f _vPoints[8])
{
  Vect3f l_vEye = _pCamera->GetEye();
  Vect3f l_vDir = _pCamera->GetDirection();
  Vect3f l_vUp  = _pCamera->GetVecUp();
  Vect3f l_vRight = (l_vDir ^ l_vUp).GetNormalized();


  int l_VerticalEdges[4], l_HorizontalEdges[4];
  Select4Edges(_vPoints, l_vUp, l_VerticalEdges);
  Select4Edges(_vPoints, l_vRight, l_HorizontalEdges);
  
  ClosePlane(l_vEye, l_vDir, _vPoints, l_VerticalEdges,   0);//right
  ClosePlane(l_vEye, l_vDir, _vPoints, l_VerticalEdges,   1);//left 
  ClosePlane(l_vEye, l_vDir, _vPoints, l_HorizontalEdges, 2);//bottom
  ClosePlane(l_vEye, l_vDir, _vPoints, l_HorizontalEdges, 3);//top
}

