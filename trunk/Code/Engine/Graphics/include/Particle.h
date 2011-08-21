#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <d3dx9.h>
#include "base.h"
#include "Camera.h"
#include "Texture.h"

#define __PARTICLE_VIA_SHADER__

struct SParticleRenderInfo 
{
  float x, y, z, size;
  float minU, minV, maxU, maxV;
  float angleSin, angleCos;
  uint32 color;
};

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
  void SetPos               (const Vect3f& _vPos)        {m_vPos = _vPos;};
  void SetDir               (const Vect3f& _vDir)        {m_vDir = _vDir;};
  void SetVel               (const Vect3f& _vVel)        {m_vVel = _vVel;};
  void SetGravity           (const Vect3f& _vGravity)    {m_vGravity = _vGravity;};
  void SetTexParticle       (CTexture* _pTexParticle)               {m_pTexParticle = _pTexParticle;};
  void SetAngle             (float _fAngle)                         {m_fAngle = _fAngle;};
  void SetInitAngle         (float _fIncrementAngle)          {m_fIncrementAngle = _fIncrementAngle;};
  //void SetSize	            ( const float sizeX, const float sizeY ) {m_fSizeX = sizeX; m_fSizeY = sizeY;}
  void SetPointA            (const Vect3f& _pointA)      {m_PointA=_pointA;};
  void SetPointB            (const Vect3f& _pointB)      {m_PointB=_pointB;};
  void SetPointC            (const Vect3f& _pointC)      {m_PointC=_pointC;};
  void SetPointD            (const Vect3f& _pointD)      {m_PointD=_pointD;};
  void SetAU                (float _fAU)                      {m_fAU = _fAU;};
  void SetAV                (float _fAV)                      {m_fAV = _fAV;};
  void SetBU                (float _fBU)                      {m_fBU = _fBU;};
  void SetBV                (float _fBV)                      {m_fBV = _fBV;};
  void SetCU                (float _fCU)                      {m_fCU = _fCU;};
  void SetCV                (float _fCV)                      {m_fCV = _fCV;};
  void SetDU                (float _fDU)                      {m_fDU = _fDU;};
  void SetDV                (float _fDV)                      {m_fDV = _fDV;};

  void SetNumFiles            (int _iTexNumFiles)                     {m_iTexNumFiles = _iTexNumFiles;};
  void SetNumColumnes         (int _iTexNumColumnes)                  {m_iTexNumColumnes = _iTexNumColumnes;};
  void SetTimeAnimationDiapo  (float _fTimeAnimationDiapo)            {m_fTimeAnimationDiapo = _fTimeAnimationDiapo;};
  void SetAnimated            (bool _bAnimated)                       {m_bAnimated = _bAnimated;};
  

  float GetSize                   () const              {return m_fSize;};
  
  float GetLifeTimer              () const              {return m_fLifetime;};
  float GetAge                    () const              {return m_fAge;};
  const D3DXCOLOR& GetColor       () const              {return m_Color;};
  const Vect3f& GetPos            () const                   {return m_vPos;};
  const Vect3f& GetDir            () const                   {return m_vDir;};
  const Vect3f& GetVel            () const                   {return m_vVel;};
  const Vect3f& GetGravity        () const                   {return m_vGravity;};
  CTexture*   GetTexParticle      () const              {return m_pTexParticle;};  
  float GetAngle                  () const              {return m_fAngle;};
  const Vect3f& GetPointA         () const                   {return m_PointA;};
  const Vect3f& GetPointB         () const                   {return m_PointB;};
  const Vect3f& GetPointC         () const                   {return m_PointC;};
  const Vect3f& GetPointD         () const                   {return m_PointD;};

  float GetAU                     () const              {return m_fAU;};
  float GetAV                     () const              {return m_fAV;};
  float GetBU                     () const              {return m_fBU;};
  float GetBV                     () const              {return m_fBV;};
  float GetCU                     () const              {return m_fCU;};
  float GetCV                     () const              {return m_fCV;};
  float GetDU                     () const              {return m_fDU;};
  float GetDV                     () const              {return m_fDV;};

  void  FillInstanceData          (SParticleRenderInfo*);
 

  int   GetNumFiles               () const              {return m_iTexNumFiles;};
  int   GetNumColumnes            () const              {return m_iTexNumColumnes;};
  float GetTimeAnimationDiapo     () const              {return m_fTimeAnimationDiapo;};
  bool  GetAnimated               () const              {return m_bAnimated;};
  
  bool          IsAlive           () const              { return m_fAge < m_fLifetime; };
  
  virtual bool  Update               (float _fDeltaTime, CCamera* _pCamera);
  virtual void  Release              ();

  
  vector<D3DXCOLOR>      m_vColor;
  vector<float>          m_vTimeColor; 
  vector<float>          m_vTimeColorInterpolation;
  vector<float>          m_vSize;
  vector<float>          m_vTimeSize; 
  vector<float>          m_vTimeSizeInterpolation;
  vector<Vect3f>         m_vDirection;
  vector<float>          m_vTimeDirection;
  vector<float>          m_vTimeDirectionInterpolation;
  // animated
  vector<int>            m_vFilesColumnes;//guarda el numero de files i de columnes que conte la textura animada
  vector<float>			     m_vTimeAnimated;//aqui guardem el temps en que pasa el event, i el temps de l'animacio
  vector<float>			     m_vTimeAnimatedInterpolation;
  vector<CTexture*>      m_vTextureAnimation;

private:
  bool UpdateState(float _fDeltaTime);
  void UpdateBillboard(CCamera* _pCamera);


  float             m_fSize;
  float             m_fLifetime; // temps de vida que tindra
  float             m_fAge; //la edat que te, es a dir, temps que porta inicialitzada
  D3DXCOLOR         m_Color;
  Vect3f            m_vDir; 
  Vect3f            m_vVel;
  Vect3f            m_vGravity;
  float             m_fAngle;
  float             m_fIncrementAngle;
  Vect3f            m_vPos;
  Vect3f     		    m_PointA;
  Vect3f            m_PointB;
  Vect3f            m_PointC; 
  Vect3f            m_PointD;
  Vect3f            m_VDirection;
  Vect3f            m_VUp;
  Vect3f            m_VRight;
  //**************************
  int                 m_iTexNumFiles;// numero de files que te la texture
  int                 m_iTexNumColumnes;// numero de columnes que te la textura
  float               m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  bool                m_bAnimated;
  float      m_fTimeAnimationActual;//temps k porta durant l'animacio
  int        m_iNumDiapo;//numero de diapositiva a la que ens trobem
  float      m_fIncrementV; //alçada de cada diapositiva 
  float      m_fIncrementU; //amplada de cada diapositiva
  int        m_iTotalDiapos;
  float      m_fTimeInterpolation;
  

  CTexture*  m_pTexParticle;


  // cordenades de textura per a cada punt.
  // la mínima (0,0) és la C i la màxima (1,1) la B
  float      m_fAU;
  float      m_fAV;
  float      m_fBU;
  float      m_fBV;
  float      m_fCU;
  float      m_fCV;
  float      m_fDU;
  float      m_fDV;
  
};
#endif
