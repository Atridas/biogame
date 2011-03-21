#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>
#include "base.h"

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
  //void SetVectorTimeColor   (float _vTime)              {m_vTime = _vTime;};
  //void SetVectorColor       (D3DXCOLOR _vColor)         {m_vColor = _vColor;};

  float GetSize                   () const              {return m_fSize;};
  float GetLifeTimer              () const              {return m_fLifetime;};
  float GetAge                    () const              {return m_fAge;};
  const D3DXCOLOR& GetColor       () const              {return m_Color;};
  const D3DXVECTOR3& GetPos       () const              {return m_vPos;};
  const D3DXVECTOR3& GetDir       () const              {return m_vDir;};
  //float GetVectorTimeColor        (int i)               {return m_vTime[i];};
  //D3DXCOLOR GetVectorColor        (int i)               {return m_vColor[i];};

  bool  Update               (float fTimeDelta);

  
  vector<D3DXCOLOR>      m_vColor;
  vector<float>          m_vTime;

private:
  float             m_fSize;
  float             m_fLifetime; // temps de vida que tindra
  float             m_fAge; //la edat que te, es a dir, temps que porta inicialitzada
  D3DXCOLOR         m_Color;
  D3DXVECTOR3       m_vPos;
  D3DXVECTOR3       m_vDir; 
  
};
#endif
