#pragma once
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <base.h>
#include <d3dx9.h>

class CTexture
{
protected:
  LPDIRECT3DTEXTURE9 m_pTexture;
  std::string m_szFileName;

  virtual bool LoadFile();
  void Unload();
public:
  CTexture():m_pTexture(0),m_szFileName(""){};
  ~CTexture(){Unload();};
  const std::string & GetFileName() const {m_szFileName;};
  bool Load(const std::string &_szFileName);
  bool Reload();
  void Activate(size_t _StageId);
};

#endif