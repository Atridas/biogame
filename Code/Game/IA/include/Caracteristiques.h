#pragma once

#ifndef __CARACTERISTIQUES_H__
#define __CARACTERISTIQUES_H__

#include "base.h"
#include "Named.h"
#include "Object3D.h"

class CCaracteristiques : 
    public CObject3D,
    public CNamed,
    public CBaseControl
{
public:
  CCaracteristiques();
  ~CCaracteristiques();

  Vect3f GetPosition() {return CObject3D::   m_vPosition;};
  void SetName(const string& _name) {CNamed::SetName(_name);};

  //els set s'hauran de calcular correctament segons de cidim, pero de moment aixi kda llest
  void SetVida(const int _iVida){m_iVida = _iVida;};
  void SetVelocitat(float _fVelocitat){m_fVelocitat = _fVelocitat;};
  void SetDanyAtac(float _fDanyAtac){m_fDanyAtac = _fDanyAtac;};
  
  int GetVida() const{return m_iVida;};
  float GetVelocitat() const{return m_fVelocitat;};
  float GetDanyAtac() const{return m_fDanyAtac;};

protected:

  int   m_iVida;
  float m_fVelocitat;
  float m_fDanyAtac;

};

#endif