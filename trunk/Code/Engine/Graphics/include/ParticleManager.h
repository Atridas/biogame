#pragma once

#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include "base.h"
#include "ParticleEmitter.h"
#include <XML/XMLTreeNode.h>
#include "params.h"
#include <Utils/MapManager.h>
#include "Camera.h"
#include "RenderableVertexs.h"

class CRenderManager;

/*struct SParticleInfo
{
  string                m_szId;
  float                 m_fMinEmitRate;
  float                 m_fMaxEmitRate;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;
  float                 m_fMinSize;
  float                 m_fMaxSize;
  //D3DXVECTOR3           m_vSpawnDir1;
  //D3DXVECTOR3           m_vSpawnDir2;
  CTexture*             m_pTexParticle;
  vector<CTexture*>     m_vTextureAnimation;
  float                 m_fLife1;
  float                 m_fLife2;
  float                 m_fAngle1;
  float                 m_fAngle2;
  vector<D3DXCOLOR>     m_vColor;
  vector<float>         m_vTimeColor;  //modificar tot els  m_vTime  per els m_vTimeColor
  vector<float>         m_vTimeColorInterpolation;
  vector<float>             m_vSize;
  vector<float>             m_vTimeSize;
  vector<float>             m_vTimeSizeInterpolation;
  //vector<D3DXVECTOR3>   m_vDirection;
  //vector<float>         m_vTimeDirection;
  //vector<float>         m_vTimeDirectionInterpolation;
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
  vector<int>           m_vFilesColumnes;//guarda el numero de files i de columnes que conte la textura animada
  vector<float>			    m_vTimeAnimated;//aqui guardem el temps en que pasa el event, i el temps de l'animacio
  vector<float>			    m_vTimeAnimatedInterpolation;
  int                   m_iNumDirections;
  
};*/

/*struct SParticleEvent
{
  
  float                 m_fTime;
  D3DXCOLOR             m_Color1;
  D3DXCOLOR             m_Color2;
  float                 m_fMinSize;
  float                 m_fMaxSize;
  //D3DXVECTOR3           m_vSpawnDir1;
  //D3DXVECTOR3           m_vSpawnDir2;
  int                   m_iTexNumFiles;
  int                   m_iTexNumColumnes;
  float                 m_fTimeInterpolation;
 
};*/

class CParticleManager : public CMapManager<CParticleCore>
{
public:
  CParticleManager();
  ~CParticleManager(){Done();};
  
  bool Load(const string& _szFileName);
  void Update(const float _fElapsedTime, CCamera* camera);
  void Render(CRenderManager* _pRM);
  void Init(CRenderManager* _pRM);
  bool Reload();
  void Release();
  CParticleEmitter* GetParticleEmitter(const string& _szName);
  void SetAllEmittersActive(bool _bActive);
 
  CRenderableVertexs* GetRenderableVertexs() {return m_pParticleVertex;};

private:
  string                    m_szFileName;
  CRenderManager*           m_pRM;
  vector<CParticleEmitter*> m_vEmitterParticle;
  bool                      m_bReload;

  vector<D3DXVECTOR3>       m_vDirection;
  vector<float>             m_vTimeDirection;
  vector<float>             m_vTimeDirectionInterpolation;

   float                    m_fTime;
   float                    m_fTimeInterpolation;
   string				            m_szFormEmitter;
   //Vect3f                   m_vPosFormEmitter;
   float				            m_fSizeX;
   float				            m_fSizeY;
   float				            m_fSizeZ;
   int                      m_iNumDirections;

   CRenderableVertexs*      m_pParticleVertex;
};
#endif
