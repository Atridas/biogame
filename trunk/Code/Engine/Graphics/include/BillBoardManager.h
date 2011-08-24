#pragma once

#ifndef __BILLBOARD_MANAGER_H__
#define __BILLBOARD_MANAGER_H__

#include "base.h"
#include "BillBoard.h"
#include <XML/XMLTreeNode.h>
#include "Core.h"
#include "TextureManager.h"


class CRenderManager;

class CBillBoardManager:
  public CBaseControl
{
public:
  CBillBoardManager();
  ~CBillBoardManager(){Done();};

  bool Load(const string& _szFileName);
  void Update(const float _fElapsedTime, CCamera* camera);
  void Render(CRenderManager* _pRM);
  void Init(CRenderManager* _pRM);
  bool Reload();
  void Release();
  CBillBoard* GetBillBoard(const string& _szName);
  void SetAllBillboardsActive(bool _bActive);
  CBillBoard* CreateBillBorad(const string& _szName, const string& _szCore, const Vect3f& _vPosition);
  
  
private:
  string                    m_szFileName;
  CRenderManager*           m_pRM;
  map<string,CBillBoard*>   m_Billboards;
  
  
  bool                      m_bReload;
  CTexture*                 m_pTexParticle;

};

#endif