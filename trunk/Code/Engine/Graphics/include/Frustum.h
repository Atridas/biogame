//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase 
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FRUSTUM_H_
#define INC_FRUSTUM_H_

#include <d3dx9math.h>
#include "Math/Vector3.h"

class CCamera;

class CFrustum 
{
public:
	
  void	Update							( const D3DXMATRIX &viewproj );
  void  Update              ( const CCamera* _pCamera );
  void  Update              ( const CCamera* _pCamera, const Vect3f _vPoints[8]);
  void  Update              ( const Vect3f& _vCameraEye, const Vect3f* _vPoints, uint32 _iNumPoints);
  bool	SphereVisible				( const D3DXVECTOR3 &p, float radius ) const;
  bool	BoxVisible					( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const;
  bool  BoxVisibleByVertexs	( const Vect3f* points) const;

  //els punts van en el mateix ordre que a bounding box
  void  GetPoints           ( Vect3f* _vPoints );
  void  GetPoints           ( Vect3f* _vPoints ) const;
   
private:

  void ClosePlane(const Vect3f& _vEye, const Vect3f& _vDir, const Vect3f _vPoints[8], const int _iArestes[4], uint32 _iPlane);
  void UpdatePlane(uint32 _iPlane, const Vect3f& _vCameraEye, const Vect3f* _vPoints, const Vect3f* _vDirections, uint32 _iNumPoints);

  //rigt/left / bottom/top / far/near
  float m_frustum[6][4];

  //f[0] * x + f[1] * y + f[2] * z + f[3] = 0
};

#endif //INC_FRUSTUM_H_
