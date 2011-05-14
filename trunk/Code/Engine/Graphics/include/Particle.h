#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>
#include "base.h"
#include "Camera.h"
#include "Texture.h"

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
  //void SetSize	            ( const float sizeX, const float sizeY ) {m_fSizeX = sizeX; m_fSizeY = sizeY;}
  void SetPointA            (const D3DXVECTOR3& _pointA)      {m_PointA=_pointA;};
  void SetPointB            (const D3DXVECTOR3& _pointB)      {m_PointB=_pointB;};
  void SetPointC            (const D3DXVECTOR3& _pointC)      {m_PointC=_pointC;};
  void SetPointD            (const D3DXVECTOR3& _pointD)      {m_PointD=_pointD;};
  

  float GetSize                   () const              {return m_fSize;};
  
  float GetLifeTimer              () const              {return m_fLifetime;};
  float GetAge                    () const              {return m_fAge;};
  const D3DXCOLOR& GetColor       () const              {return m_Color;};
  const D3DXVECTOR3& GetPos       () const              {return m_vPos;};
  const D3DXVECTOR3& GetDir       () const              {return m_vDir;};
  const D3DXVECTOR3& GetVel       () const              {return m_vVel;};
  const D3DXVECTOR3& GetGravity   () const              {return m_vGravity;};
  //float GetAngle                  () const              {return m_fAngle;};
  const D3DXVECTOR3& GetPointA    () const              {return m_PointA;};
  const D3DXVECTOR3& GetPointB    () const              {return m_PointB;};
  const D3DXVECTOR3& GetPointC    () const              {return m_PointC;};
  const D3DXVECTOR3& GetPointD    () const              {return m_PointD;};
  
  bool  Update               (float fTimeDelta, CCamera* camera);
  //void  Render	             (LPDIRECT3DDEVICE9 device, CTexture* texture);
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
  //D3DXVECTOR3       m_vPos;
  D3DXVECTOR3       m_vDir; 
  D3DXVECTOR3       m_vVel;
  D3DXVECTOR3       m_vGravity;
  float             m_fAngle;
  D3DXVECTOR3       m_vPos;
  // float             m_fSizeX;
  //float             m_fSizeY;
  D3DXVECTOR3		    m_PointA;
  D3DXVECTOR3       m_PointB;
  D3DXVECTOR3       m_PointC; 
  D3DXVECTOR3       m_PointD;
  D3DXVECTOR3       m_VDirection;
  D3DXVECTOR3       m_VUp;
  D3DXVECTOR3       m_VRight;
  
  
};
#endif
