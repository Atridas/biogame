#pragma once

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "base.h"
#include <d3dx9.h>
#include "RenderManager.h"
#include <XML/XMLTreeNode.h>
#include "Camera.h"
#include "ExplosionsManager.h"


struct SExplosionInfo;

class CExplosion : public CBaseControl
{
public:
  CExplosion();
  ~CExplosion(){Done(); Release();};

  // SET
  void SetName                (string _szName)                        {m_szName = _szName;};
  void SetId                  (string _szId)                          {m_szId = _szId;};
  void SetPosition            (Vect3f& _vPos)                         {m_vPosition=_vPos;};
  void SetSize                (float _fSizeX, float _fSizeY)          {m_fSizeX = _fSizeX; m_fSizeY = _fSizeY;};
  void SetTexExplosion        (CTexture* _pTexExplosion)              {m_pTexExplosion = _pTexExplosion;};
  void SetNumFiles            (int _iTexNumFiles)                     {m_iTexNumFiles = _iTexNumFiles;};
  void SetNumColumnes         (int _iTexNumColumnes)                  {m_iTexNumColumnes = _iTexNumColumnes;};
  void SetTimeAnimationDiapo  (float _fTimeAnimationDiapo)            {m_fTimeAnimationDiapo = _fTimeAnimationDiapo;};

  
  //GET
  string            GetName                 () const {return m_szName;};
  string            GetId                   () const {return m_szId;};
  Vect3f            GetPosition             () const {return m_vPosition;};
  float             GetSize                 () const {return m_fSizeX, m_fSizeY;};
  CTexture*         GetTexExplosion         () const {return m_pTexExplosion;};
  int               GetNumFiles             () const {return m_iTexNumFiles;};
  int               GetNumColumnes          () const {return m_iTexNumColumnes;};
  float             GetTimeAnimationDiapo   () const {return m_fTimeAnimationDiapo;};
  
  void                    Update              (float fElapsedTime, CCamera* camera);
  void                    Init                (CRenderManager* rm);
  void                    Release             ();
  void                    Render              (CRenderManager* _pRM);
  void                    SetAttributes       (SExplosionInfo* _info);


private:
  string              m_szName;
  string              m_szId;
  Vect3f		          m_vPosition;
	float			          m_fSizeX, m_fSizeY;
	CTexture*           m_pTexExplosion; 
  int                 m_iTexNumFiles;// numero de files que te la texture
  int                 m_iTexNumColumnes;// numero de columnes que te la textura
  float               m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
  Vect3f		          m_PointA, m_PointB, m_PointC, m_PointD;
  int                 m_iNumDiapo;//numero de diapositiva a la que ens trobem
  float               m_fIncrementV; //alçada de cada diapositiva 
  float               m_fIncrementU; //amplada de cada diapositiva
  float               m_fTimeAnimationActual;//temps k porta durant l'animacio
  int                 m_iTotalDiapos;

  // cordenades de textura per a cada punt.
  float               m_fAU;
  float               m_fAV;
  float               m_fBU;
  float               m_fBV;
  float               m_fCU;
  float               m_fCV;
  float               m_fDU;
  float               m_fDV;
};

#endif;
