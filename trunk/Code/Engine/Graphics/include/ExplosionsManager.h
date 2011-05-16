#pragma once

#ifndef __EXPLOSIONS_MANAGER_H__
#define __EXPLOSIONS_MANAGER_H__


#include "base.h"
#include <XML/XMLTreeNode.h>
#include "params.h"
#include <Utils/MapManager.h>
#include "Camera.h"
#include <d3dx9.h>
#include "Camera.h"
#include "Texture.h"
#include "Explosion.h"

class CRenderManager;

struct SExplosionInfo
{
  string      m_szId;
  Vect3f		  m_Position;
	float			  m_SizeX, m_SizeY;
	CTexture*   m_pTexExplosion; 
  int         m_iTexNumFiles;// numero de files que te la texture
  int         m_iTexNumColumnes;// numero de columnes que te la textura
  float       m_fTimeAnimationDiapo;//(akest valor es pasa per el xml)temps que es destina a cada diapositiva
};

class CExplosionsManager : public CMapManager<SExplosionInfo>
{
public:
  CExplosionsManager();
  ~CExplosionsManager(){Done();};

  bool Load(const string& _szFileName);
  void Update(const float _fElapsedTime, CCamera* camera);
  void Render(CRenderManager* _pRM);
  void Init(CRenderManager* _pRM);
  bool Reload();
  void Release();

private:
  string              m_szFileName;
  CRenderManager*     m_pRM;
  //vector<CExplosion*> m_vExplosions;
  
};
#endif;
