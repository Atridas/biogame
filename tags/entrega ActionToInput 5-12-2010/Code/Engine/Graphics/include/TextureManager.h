#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <Utils/MapManager.h>

//---- forward declarations ----
class CTexture;
//-------------------------------

class CTextureManager : public CMapManager<CTexture>
{
public:
  CTextureManager() {};
  ~CTextureManager() {Destroy();};
  void Reload ();
};

#endif