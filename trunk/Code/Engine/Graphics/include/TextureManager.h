#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <Utils/MapManager.h>

//---- forward declarations ----
//class CTexture;
//-------------------------------
//nota: hi ha problemes si no s'inclou CTexture, així que simplement l'inclourem aquí
#include "Texture.h"

class CTextureManager : private CMapManager<CTexture>
{
public:
  CTextureManager() {};
  ~CTextureManager() {Destroy();};
  void Reload ();
  virtual CTexture* GetResource(const std::string &_szName);
};

#endif