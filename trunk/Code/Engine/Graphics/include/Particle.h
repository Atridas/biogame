#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>
#include "base.h"

class CParticle
{
public:
	CParticle   ();
  ~CParticle  (){Release();}; //pq no funciona el done??

  //---Set andf Get Functions---
  void SetSize              (float _fSize)                    {m_fSize = _fSize;};
  void SetLifeTimer         (float _fLifetime)                {m_fLifetime = _fLifetime;};
  void SetAge               (float _fAge)                     {m_fAge = _fAge;};
  void SetColor             (const D3DXCOLOR& _Color)         {m_Color = _Color;};
  void SetPos               (const D3DXVECTOR3& _vPos)        {m_vPos = _vPos;};
  void SetDir               (const D3DXVECTOR3& _vDir)        {m_vDir = _vDir;};
  void SetVel               (const D3DXVECTOR3& _vVel)        {m_vVel = _vVel;};
  void SetGravity           (const D3DXVECTOR3& _vGravity)    {m_vGravity = _vGravity;};
  //void SetAngle             (float _fAngle)                         {m_fAngle = _fAngle;};
  

  float GetSize                   () const              {return m_fSize;};
  float GetLifeTimer              () const              {return m_fLifetime;};
  float GetAge                    () const              {return m_fAge;};
  const D3DXCOLOR& GetColor       () const              {return m_Color;};
  const D3DXVECTOR3& GetPos       () const              {return m_vPos;};
  const D3DXVECTOR3& GetDir       () const              {return m_vDir;};
  const D3DXVECTOR3& GetVel       () const              {return m_vVel;};
  const D3DXVECTOR3& GetGravity   () const              {return m_vGravity;};
  //float GetAngle                  () const              {return m_fAngle;};
  
  bool  Update               (float fTimeDelta);
  void  Release              ();

  
  vector<D3DXCOLOR>      m_vColor;
  vector<float>          m_vTimeColor; 
  vector<D3DXVECTOR3>    m_vDirection;
  vector<float>          m_vTimeDirection;

private:
  float             m_fSize;
  float             m_fLifetime; // temps de vida que tindra
  float             m_fAge; //la edat que te, es a dir, temps que porta inicialitzada
  D3DXCOLOR         m_Color;
  D3DXVECTOR3       m_vPos;
  D3DXVECTOR3       m_vDir; 
  D3DXVECTOR3       m_vVel;
  D3DXVECTOR3       m_vGravity;
  //float             m_fAngle;
  
};
#endif
