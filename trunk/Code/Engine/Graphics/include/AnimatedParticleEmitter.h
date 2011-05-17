#pragma once

#ifndef __ANIMATED_PARTICLE_EMITTER_H__
#define __ANIMATED_PARTICLE_EMITTER_H__

#include "ParticleEmitter.h"
#include "Camera.h"

struct SParticleInfo;

class CAnimatedParticleEmitter : public CParticleEmitter
{
public:
	CAnimatedParticleEmitter();
	~CAnimatedParticleEmitter(){Done();};

  //SET
  void SetNumFiles            (int _iTexNumFiles)                     {m_iTexNumFiles = _iTexNumFiles;};
  void SetNumColumnes         (int _iTexNumColumnes)                  {m_iTexNumColumnes = _iTexNumColumnes;};
  void SetTimeAnimationDiapo  (float _fTimeAnimationDiapo)            {m_fTimeAnimationDiapo = _fTimeAnimationDiapo;};

  //GET
  int               GetNumFiles             () const {return m_iTexNumFiles;};
  int               GetNumColumnes          () const {return m_iTexNumColumnes;};
  float             GetTimeAnimationDiapo   () const {return m_fTimeAnimationDiapo;};

  void      Release             ();
  void      SetAttributes       (SParticleInfo* _info);
  void      Render              (CRenderManager* _pRM);

private:
  int                 m_iTexNumFiles;// numero de files que te la texture
  int                 m_iTexNumColumnes;// numero de columnes que te la textura
  float               m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  

};
#endif;
