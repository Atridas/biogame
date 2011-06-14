//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "base.h"
#include "Utils/BoundingBox.h"
#include "Utils/BoundingSphere.h"

class CObject3D
{
public:
  CObject3D(): m_vPosition(Vect3f(0.0f)),m_vScale(Vect3f(1.0f)), m_fYaw(0.0f),
               m_fPitch(0.0f), m_fRoll(0.0f), m_bVisible(true),m_bRenderBoundingBox(false),
               m_bRenderBoundingSphere(false)
           {m_vMat44.SetIdentity();};
	CObject3D(const Vect3f& _vPos, float _fYaw, float _fPitch, float _fRoll = 0);
	virtual ~CObject3D(void) {/*Nothing*/;}

	//---Get Functions
	float						    GetYaw			    () const {return m_fYaw;}
	float						    GetRoll			    () const {return m_fRoll;}
	float						    GetPitch		    () const {return m_fPitch;}
	const Vect3f&		    GetPosition	    () const {return m_vPosition;}
  Vect3f		          GetCenterPosition	    ();
  bool						    GetVisible	    () const {return m_bVisible;}
  Mat44f              GetMat44        () const {return m_vMat44;}
  void                GetMat44        (Mat44f& _Mat) const {_Mat = m_vMat44;}
  bool						    GetRenderBoundingBox  () const {return m_bRenderBoundingBox;}
  bool						    GetRenderBoundingSphere  () const {return m_bRenderBoundingSphere;}
  CBoundingBox*		    GetBoundingBox	() {return &m_BoundingBox;}
  CBoundingSphere*		GetBoundingSphere	() {return &m_BoundingSphere;}

	//---Set Functions
	void    SetPosition	( const Vect3f& _vPos );
	void		SetYaw			( float _fYaw );
	void		SetPitch		( float _fPitch );
	void		SetRoll			( float _fRoll );
  void		SetVisible	( bool  _bVisible ) {m_bVisible = _bVisible;};
  void    SetMat44    (Mat44f _vMat44);
  void		SetRenderBoundingBox	( bool  _bValue ) {m_bRenderBoundingBox = _bValue;};
  void		SetRenderBoundingSphere	( bool  _bValue ) {m_bRenderBoundingSphere = _bValue;};
  void    SetScale	  ( const Vect3f& _vScale ) {m_vScale = _vScale;};

  //---Other Functions
  void    InitMat44   ();

protected:
	Vect3f	        m_vPosition;
  Vect3f	        m_vScale;
	float		        m_fYaw;
	float		        m_fPitch;
	float		        m_fRoll;
  Mat44f          m_vMat44;
  bool            m_bVisible;
  bool            m_bRenderBoundingBox;
  bool            m_bRenderBoundingSphere;
  CBoundingBox    m_BoundingBox;
  CBoundingSphere m_BoundingSphere;
};

#endif //INC_CORE_H_