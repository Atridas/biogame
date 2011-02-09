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

class CObject3D
{
public:
  CObject3D(): m_vPosition(Vect3f(0.0f)), m_fYaw(0.0f),
               m_fPitch(0.0f), m_fRoll(0.0f), m_bVisible(true)
           {};
	CObject3D(const Vect3f& _vPos, float _fYaw, float _fPitch, float _fRoll = 0);
	virtual ~CObject3D(void) {/*Nothing*/;}

	//---Get Functions
	float						GetYaw			() const {return m_fYaw;}
	float						GetRoll			() const {return m_fRoll;}
	float						GetPitch		() const {return m_fPitch;}
	const Vect3f&		GetPosition	() const {return m_vPosition;}
  bool						GetVisible	() const {return m_bVisible;}

	//---Set Functions
	void		SetPosition	( const Vect3f& _vPos );
	void		SetYaw			( float _fYaw );
	void		SetPitch		( float _fPitch );
	void		SetRoll			( float _fRoll );
  void		SetVisible	( bool  _bVisible ) {m_bVisible = _bVisible;};

protected:
	Vect3f	    m_vPosition;
	float		    m_fYaw;
	float		    m_fPitch;
	float		    m_fRoll;
  bool        m_bVisible;
  BoundingBox m_BoundingBox;
};

#endif //INC_CORE_H_