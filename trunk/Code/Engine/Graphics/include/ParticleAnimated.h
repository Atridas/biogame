#pragma once

#ifndef __PARTICLE_ANIMATED_H__
#define __PARTICLE_ANIMATED_H__

#include "Particle.h"

class CParticleAnimated : CParticle
{
public:
  CParticleAnimated();
  ~CParticleAnimated(){Release();};

  //SET
  void SetNumFiles            (int _iTexNumFiles)                     {m_iTexNumFiles = _iTexNumFiles;};
  void SetNumColumnes         (int _iTexNumColumnes)                  {m_iTexNumColumnes = _iTexNumColumnes;};
  void SetTimeAnimationDiapo  (float _fTimeAnimationDiapo)            {m_fTimeAnimationDiapo = _fTimeAnimationDiapo;};

  //GET
  int               GetNumFiles             () const {return m_iTexNumFiles;};
  int               GetNumColumnes          () const {return m_iTexNumColumnes;};
  float             GetTimeAnimationDiapo   () const {return m_fTimeAnimationDiapo;};

  bool  Update               (float fTimeDelta, CCamera* camera);

private:
  int        m_iTexNumFiles;// numero de files que te la texture
  int        m_iTexNumColumnes;// numero de columnes que te la textura
  float      m_fTimeAnimationActual;//temps k porta durant l'animacio
  float      m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  int        m_iNumDiapo;//numero de diapositiva a la que ens trobem
  float      m_fIncrementV; //alçada de cada diapositiva 
  float      m_fIncrementU; //amplada de cada diapositiva
  int        m_iTotalDiapos;
};
#endif;
