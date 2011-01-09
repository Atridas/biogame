#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <Utils/MapManager.h>

//---- forward declarations ----
//class CTexture;
//-------------------------------
//nota: hi ha problemes si no s'inclou CTexture, així que simplement l'inclourem aquí
#include "Texture.h"

/**
 * Classe Texture Manager.
 * Classe d'administració per a les Textures del motor.
**/
class CTextureManager:
  public CMapManager<CTexture>
{
public:
  /**
   * Constructor per defecte.
   * Aquest constructor deixa la classe en Ok.
  **/
  CTextureManager() {SetOk(true);};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CTextureManager() {Done();};
  /**
   * Mètode de recàrrega.
   * Aquest mètode recarrega totes les textures enmagatzemades.
  **/
  void Reload ();
  /**
   * Getter de textura.
   * Retorna la textura amb l'identificador especificat. Si aquesta textura no existia, la crea i la retorna.
   * @param _szName Identificador de la textura.
   * @return Textura sol·licitada.
  **/
  virtual CTexture* GetResource(const std::string &_szName);
};

#endif