#pragma once
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <base.h>
#include <d3dx9.h>

/**
 * Classe Texture.
 * Classe que s'encarrega d'administrar una textura. En aquest cas s'ha implementat per a DirectX9.
**/
class CTexture:
  public CBaseControl
{
protected:
  /**
   * Textura en format de DirectX9.
  **/
  LPDIRECT3DTEXTURE9 m_pTexture;
  /**
   * Path relatiu a la textura.
  **/
  std::string m_szFileName;

  /**
   * Mètode de càrrega.
   * Aquest mètode carregarà la textura especificada a m_pTexture a la VRAM.
   * @return True si s'ha carregat correctament, false sino.
  **/
  virtual bool LoadFile();
  /**
   * Mètode d'alliberament de recursos.
  **/
  virtual void Release();
public:
  /**
   * Constructor per defecte.
  **/
  CTexture():m_pTexture(0),m_szFileName(""){};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CTexture(){Done();};
  /**
   * Getter del path de la textura.
   * @return Path relatiu al fitxer de textura.
  **/
  const std::string & GetFileName() const {m_szFileName;};
  /**
   * Mètode de càrrega.
   * Aquest mètode instanciarà i carregarà la textura especificada.
   * @param _szFileName Path relatiu al fitxer de textura.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Load(const std::string &_szFileName);
  /**
   * Mètode de recàrrega.
   * Aquest mètode recarregarà la textura especificada anteriorment.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Reload();
  /**
   * Mètode d'activació de textura.
   * Aquest mètode activarà la textura segons l'StageId proporcionat.
   * @param _StageId.
  **/
  void Activate(size_t _StageId);
};

#endif