#pragma once
#ifndef __RENDERABLE_VERTEXS_H__
#define __RENDERABLE_VERTEXS_H__

#include <d3d9.h>

#include <base.h>

//forward declarations ------------------------
class CRenderManager;
class CEffectTechnique;
class CTexture;
//---------------------------------------------

/**
 * Classe abstracta RenderableVertex.
 * Classe abstracta que conté un Index buffer i un Vèrtex buffer i s'encarrega de fer-ne la gestió, per tal de facilitar el codi.
**/
class CRenderableVertexs
{
protected:
  /**
   * Vèrtex Buffer.
  **/
  LPDIRECT3DVERTEXBUFFER9 m_pVB;
  /**
   * Index Buffer.
  **/
  LPDIRECT3DINDEXBUFFER9 m_pIB;
  /**
   * Nombre de vèrtexs i índexs.
  **/
  size_t m_iIndexCount, m_iVertexCount;

public:
  /**
   * Constructor per defecte.
  **/
  CRenderableVertexs():m_pVB(0),m_pIB(0),m_iIndexCount(0),m_iVertexCount(0) 
                                                        {};
  /**
   * Destructor.
   * Allibera els recursos abans d'eliminar-se.
  **/
  virtual ~CRenderableVertexs()                         {CHECKED_RELEASE(m_pVB)CHECKED_RELEASE(m_pIB)};

  /**
   * Mètode de renderització dels vèrtexs.
   * @param _pRM Render Manager.
   * @return True si s'ha renderitzat correctament, false sino.
  **/
  virtual bool Render(CRenderManager *_pRM) const         = 0;

  /**
   * Mètode de renderització dels vèrtexs usant shaders.
   * @param _pRM Render Manager.
   * @param _pEffectTechnique Tècnica.
   * @return True si s'ha renderitzat correctament, false sino.
  **/
  virtual bool Render(CRenderManager *_pRM, CEffectTechnique *_pEffectTechnique) const         = 0;

  /**
   * Getter del nombre de triangles.
   * @return El nombre de triangles.
  **/
  virtual inline size_t GetFacesCount() const           {return m_iIndexCount/3;};
  /**
   * Getter del nombre de vèrtexs.
   * @return El nombre de vèrtexs.
  **/
  virtual inline size_t GetVertexsCount() const         {return m_iVertexCount;};

  /**
   * Getter del tipus de vèrtex.
   * @return El tipus de vèrtex.
  **/
  virtual inline unsigned short GetVertexType() const   = 0;
  /**
   * Getter del tamany del vèrtex.
   * @return El tamany del vèrtex en bytes.
  **/
  virtual inline size_t GetVertexSize() const           = 0;
  /**
   * Getter del tamany de l'índex.
   * @return El tamany de l'índex en bytes.
  **/
  virtual inline size_t GetIndexSize() const            = 0;
};

#endif