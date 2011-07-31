#pragma once
#ifndef __PARTICLE_CORE_H__
#define __PARTICLE_CORE_H__

#include <base.h>
#include <Named.h>

#include <d3d9.h>
#include "Texture.h"

//Forward declarations-------------
class CXMLTreeNode;
//---------------------------------

struct SParticleEvent
{
  
  float                 m_fTime;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;
  float                 m_fMinSize;
  float                 m_fMaxSize;
  int                   m_iTexNumFiles;
  int                   m_iTexNumColumnes;
  float                 m_fTimeInterpolation;
 
};

class CParticleCore :
  public CBaseControl
  //public CNamed
{
public:
  CParticleCore() : 
			m_szFileName(""),
      m_szId(""),
      m_fMinEmitRate(10.0f),
			m_fMaxEmitRate(30.0f),
			m_Color1(1.0f,1.0f,1.0f,1.0f),
			m_Color2(1.0f,1.0f,1.0f,1.0f),
			m_fMinSize(1.0f),
			m_fMaxSize(3.0f),
      m_vVel(1.0f,1.0f,1.0f),
			m_vGravity(0.0f,0.0f,0.0f),
			m_pTexParticle(0),
			m_fLife1              (0.0f ),
			m_fLife2              (0.0f ),
			m_fAngle1             (0.0f ),
			m_fAngle2             (0.0f ),
			m_bBucleInfinit       (true ),
			m_iNumBucle           (0    ),
			m_fRebootEmitter1     (0.0f ),
			m_fRebootEmitter2     (0.0f ),
      m_iTexNumFiles        (0    ),
      m_iTexNumColumnes     (0    ),
      m_fTimeAnimationDiapo (0.0f ),
			
			m_bTotDeCop           (false),
			m_bAnimated  ( false) {};

  virtual ~CParticleCore(void) {Done();};


  
  bool LoadParticleCore(const string& _szFileName, const string& _type);

  //  Set
  
  void SetId                (string _szId)                          {m_szId = _szId;};
  void SetMinEmitRate       (float _fMinEmitRate)                   {m_fMinEmitRate = _fMinEmitRate;};
  void SetMaxEmitRate       (float _fMaxEmitRate)                   {m_fMaxEmitRate = _fMaxEmitRate;};
  void SetColor1            (D3DXCOLOR& _Color1)                    {m_Color1 = _Color1;};
  void SetColor2            (D3DXCOLOR& _Color2)                    {m_Color2 = _Color2;};
  void SetMinSize           (float _fMinSize)                       {m_fMinSize = _fMinSize;};
  void SetMaxSize           (float _fMaxSize)                       {m_fMaxSize = _fMaxSize;};
  void SetLife1             (float _fLife1)                         {m_fLife1 = _fLife1;};
  void SetLife2             (float _fLife2)                         {m_fLife2 = _fLife2;};

  void SetTexParticle       (CTexture* _pTexParticle)               {m_pTexParticle = _pTexParticle;};
  void SetVel               (Vect3f&      _vVel)                    {m_vVel = _vVel;};
  void SetGravity           (Vect3f&      _vGravity)                {m_vGravity = _vGravity;};
  void SetAngle1            (float _fAngle1)                        {m_fAngle1 = _fAngle1;};
  void SetAngle2            (float _fAngle2)                        {m_fAngle2 = _fAngle2;};

  

  //Get

  string                          GetId               () const {return m_szId;};
  float                           GetMinEmitRate      () const {return m_fMinEmitRate;};
  float                           GetMaxEmitRate      () const {return m_fMaxEmitRate;};
  const D3DXCOLOR&                GetColor1           () const {return m_Color1;};
  const D3DXCOLOR&                GetColor2           () const {return m_Color2;};
  float                           GetMinSize          () const {return m_fMinSize;};
  float                           GetMaxSize          () const {return m_fMaxSize;};
  CTexture*                       GetTexParticle      () const {return m_pTexParticle;};        
  const Vect3f&                   GetVel              () const {return m_vVel;};
  const Vect3f&                   GetGravity          () const {return m_vGravity;};
  float                           GetAngle1           () const {return m_fAngle1;};
  float                           GetAngle2           () const {return m_fAngle2;};
  float                           GetSizeY            () const {return m_vSize[1];};
  float                           GetLife1            () const {return m_fLife1;};
  float                           GetLife2            () const {return m_fLife2;};
  bool                            GetBucleInfinit     () const {return m_bBucleInfinit;};
  int                             GetNumBucle         () const {return m_iNumBucle;};
  float                           GetRebootEmitter1   () const {return m_fRebootEmitter1;};     
  float                           GetRebootEmitter2   () const {return m_fRebootEmitter2;};     
  bool                            GetTotDeCop         () const {return m_bTotDeCop;};
  bool                            GetAnimated         () const {return m_bAnimated;};
  
 
  
  
  vector<D3DXCOLOR>               m_vColor;
  vector<float>                   m_vTimeColor;
  vector<float>                   m_vTimeColorInterpolation;
  vector<float>                   m_vSize;
  vector<float>                   m_vTimeSize;
  vector<float>                   m_vTimeSizeInterpolation;
  vector<int>                     m_vFilesColumnes;//guarda el numero de files i de columnes que conte la textura animada
  vector<float>			              m_vTimeAnimated;//aqui guardem el temps en que pasa el event, i el temps de l'animacio
  vector<float>			              m_vTimeAnimatedInterpolation;
  
protected:
  void Release();
private:
  string                m_szFileName;
  string                m_szId;
  float                 m_fMinEmitRate;
  float                 m_fMaxEmitRate;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;
  float                 m_fMinSize;
  float                 m_fMaxSize;
  CTexture*             m_pTexParticle;
  vector<CTexture*>     m_vTextureAnimation;
  float                 m_fLife1;
  float                 m_fLife2;
  float                 m_fAngle1;
  float                 m_fAngle2;
  Vect3f                m_vVel; 
  Vect3f                m_vGravity;
  bool                  m_bAnimated;
  bool                  m_bBucleInfinit;//si es false s'ha d'indicar quantes pasades de crear particules volem, si es true crea infinitament
  int                   m_iNumBucle;//numero de vegades que volem crear particlues
  float                 m_fRebootEmitter1; //Temps que tarde en tornar a emetre particules. Si esta a 0, no torna
  float                 m_fRebootEmitter2;
  bool                  m_bTotDeCop; //Si esta a true crea totes les particules de cop. 
  int                   m_iTexNumFiles;
  int                   m_iTexNumColumnes;
  float                 m_fTimeAnimationDiapo;

  

  

};

#endif
