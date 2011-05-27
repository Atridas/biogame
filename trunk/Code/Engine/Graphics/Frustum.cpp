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

  Vect3f n = (v ^ u).Normalize();
  
  plane[0] = n.x;
  plane[1] = n.y;
  plane[2] = n.z;
  plane[3] = -(n * p0);
}

void CFrustum::Update(CCamera* _pCamera) 
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

	Vect3f l_vFtl = l_vFc + (l_vUp * l_fHFar * .5f) - (l_vRight * l_fWFar * .5f);
  Vect3f l_vFtr = l_vFc + (l_vUp * l_fHFar * .5f) + (l_vRight * l_fWFar * .5f);
  Vect3f l_vFbl = l_vFc - (l_vUp * l_fHFar * .5f) - (l_vRight * l_fWFar * .5f);
  Vect3f l_vFbr = l_vFc - (l_vUp * l_fHFar * .5f) + (l_vRight * l_fWFar * .5f);

  Vect3f l_vNc = l_vPos + l_vDir * l_fZNear;

  Vect3f l_vNtl = l_vNc + (l_vUp * l_fHNear * .5f) - (l_vRight * l_fWNear * .5f);
  Vect3f l_vNtr = l_vNc + (l_vUp * l_fHNear * .5f) + (l_vRight * l_fWNear * .5f);
  Vect3f l_vNbl = l_vNc - (l_vUp * l_fHNear * .5f) - (l_vRight * l_fWNear * .5f);
  Vect3f l_vNbr = l_vNc - (l_vUp * l_fHNear * .5f) + (l_vRight * l_fWNear * .5f);
  
  //near
  ComputePlane(l_vNbr, l_vNbl, l_vNtr, m_frustum[0]);
  //far
  ComputePlane(l_vFbl, l_vFbr, l_vFtr, m_frustum[1]);
  
  //top
  ComputePlane(l_vNtr, l_vNtl, l_vFtr, m_frustum[2]);
  //bottom
  ComputePlane(l_vNbl, l_vNbr, l_vFbr, m_frustum[3]);
  
  //left
  ComputePlane(l_vNtl, l_vNbl, l_vFtl, m_frustum[4]);
  //right
  ComputePlane(l_vNbr, l_vNtr, l_vFbr, m_frustum[5]);
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
