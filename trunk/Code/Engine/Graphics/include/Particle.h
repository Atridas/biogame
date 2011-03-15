#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>

class CParticle
{
public:
	CParticle   (void);
	~CParticle  (void);

  //---Set andf Get Functions---
  void SetSize              (float _fSize)              {m_fSize = _fSize;};
  void SetLifeTimer         (float _fLifetime)          {m_fLifetime = _fLifetime;};
  void SetAge               (float _fAge)               {m_fAge = _fAge;};
  void SetColor             (const D3DXCOLOR& _Color)   {m_Color = _Color;};
  void SetPos               (const D3DXVECTOR3& _vPos)  {m_vPos = _vPos;};
  void SetDir               (const D3DXVECTOR3& _vDir)  {m_vDir = _vDir;};

  float GetSize             () const                    {return m_fSize;};
  float GetLifeTimer        () const                    {return m_fLifetime;};
  float GetAge              () const                    {return m_fAge;};
  const D3DXCOLOR& GetColor () const                    {return m_Color;};
  const D3DXVECTOR3& GetPos () const                    {return m_vPos;};
  const D3DXVECTOR3& GetDir () const                    {return m_vDir;};

  bool  Update               (float fTimeDelta);

private:
  float           m_fSize;
  float           m_fLifetime;
  float           m_fAge;
  D3DXCOLOR       m_Color;
  D3DXVECTOR3     m_vPos;
  D3DXVECTOR3     m_vDir; 
};
#endif
