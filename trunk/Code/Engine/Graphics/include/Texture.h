#pragma once
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <base.h>
#include <d3dx9.h>

class CTexture
{
protected:
  LPDIRECT3DBASETEXTURE9 m_Texture;
  std::string m_FileName;

  virtual bool LoadFile();
  void Unload();
public:
  CTexture():m_Texture(0),m_FileName(""){};
  ~CTexture(){};
  const std::string & GetFileName() const {m_FileName;};
  bool Load(const std::string &FileName);
  bool Reload();
  void Activate(size_t StageId);
};

#endif