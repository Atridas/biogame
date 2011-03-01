#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>

class CParticle
{
public:
	CParticle(void);
	~CParticle(void);

  bool Update(float fTimeDelta);

private:

	float		        m_fSize;
  float           m_fLifetime;
  float           m_fAge;
  D3DXCOLOR       m_Color;
  D3DXVECTOR3     m_vPos;
  D3DXVECTOR3     m_vDir; 
};
#endif
