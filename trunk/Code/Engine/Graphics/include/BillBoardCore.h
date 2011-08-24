#pragma once

#ifndef __BILLBOARD_CORE_H__
#define __BILLBOARD_CORE_H__

#include "base.h"
#include "BillBoard.h"
#include <XML/XMLTreeNode.h>
#include "Core.h"
#include "TextureManager.h"


class CRenderManager;

class CBillBoardCore:
  public CBaseControl
{
public:
  CBillBoardCore();
  ~CBillBoardCore(){Done();};

  bool Load(const string& _szFileName);
  void Release();
  CBillBoard* GetBillBoard(const string& _szName);



private:
  string                    m_szFileName;
  map<string,CBillBoard*>   m_BillboardsCore;
  bool                      m_bReload;
  CTexture*                 m_pTexParticle;

};

#endif