#pragma once
#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <base.h>
#include <d3dx9.h>
#include "Named.h"

/**
 * Classe Texture.
 * Classe que s'encarrega d'administrar una textura. En aquest cas s'ha implementat per a DirectX9.
**/
class CTexture:
  public CBaseControl, CNamed
{
public:
  /**
   * Constructor per defecte.
  **/
  CTexture(): CNamed(""),m_pTexture(0),
                         m_pRenderTargetTexture(0),
                         m_pDepthStencilRenderTargetTexture(0),
                         m_pOldRenderTarget(0),
                         m_pOldDepthStencilRenderTarget(0),
                         m_pTextureSurface0(0),
                         m_uiWidth(0),
                         m_uiHeight(0),
                         m_szFileName("") {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CTexture(){Done();};
  /**
   * Getter del path de la textura.
   * @return Path relatiu al fitxer de textura.
  **/
  const string & GetFileName() const {return m_szFileName;};
  /**
   * Mètode de càrrega.
   * Aquest mètode instanciarà i carregarà la textura especificada.
   * @param _szFileName Path relatiu al fitxer de textura.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Load(const string &_szFileName);
  /**
   * Mètode de recàrrega.
   * Aquest mètode recarregarà la textura especificada anteriorment.
   * @return True si s'ha carregat correctament, false sino.
   * @see Load(const std::string &_szFileName)
  **/
  bool Reload();
  /**
   * Mètode d'activació de textura.
   * Aquest mètode activarà la textura segons l'StageId proporcionat.
   * @param _StageId.
  **/
  void Activate(size_t _StageId);

  
  uint32 GetWidth()  const {return m_uiWidth; };
  uint32 GetHeight() const {return m_uiHeight;};

  enum TPoolType {
    DEFAULT=0,
    SYSTEMMEM
  };

  enum TUsageType {
    DYNAMIC=0,
    RENDERTARGET
  };

  enum TTextureType {
    TGA=0,
    JPG,
    BMP
  };

  enum TFormatType {
    A8R8G8B8=0,
    R8G8B8,
    X8R8G8B8,
    R32F
  };

  bool Create(const string& _szName,
              unsigned int _uiWidth,
              unsigned int _uiHeight,
              unsigned int _uiMipMaps,
              TUsageType _UsageType,
              TPoolType _PoolType,
              TFormatType _FormatType);

  void Deactivate(size_t Stage);
  bool SetAsRenderTarget();
  void UnsetAsRenderTarget();
  static CTexture::TFormatType GetFormatTypeFromString(const string &FormatType);
  
  bool FillTextureWithColor(uint32 width, uint32 height, const CColor& color);
  bool FillDefaultTexture();
  bool FillTextureFromBuffer (uint32 width, uint32 height, uint8* pImgBuffer);

  LPDIRECT3DSURFACE9 GetSurface() const {return m_pTextureSurface0;};
  LPDIRECT3DTEXTURE9 GetD3DTexture() const {return m_pTexture;};
  
protected:
  /**
   * Textura en format de DirectX9.
  **/
  LPDIRECT3DTEXTURE9 m_pTexture;
  /**
   * Path relatiu a la textura.
  **/
  string m_szFileName;

  uint32 m_uiWidth;

  uint32 m_uiHeight;
  
  LPDIRECT3DSURFACE9 m_pDepthStencilRenderTargetTexture, m_pOldDepthStencilRenderTarget,
                     m_pOldRenderTarget, m_pRenderTargetTexture, m_pTextureSurface0;

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
};



#endif