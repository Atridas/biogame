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
 * Classe abstracta que cont� un Index buffer i un V�rtex buffer i s'encarrega de fer-ne la gesti�, per tal de facilitar el codi.
**/
class CRenderableVertexs
{
protected:
  /**
   * V�rtex Buffer.
  **/
  LPDIRECT3DVERTEXBUFFER9 m_pVB;
  /**
   * Index Buffer.
  **/
  LPDIRECT3DINDEXBUFFER9 m_pIB;
  /**
   * Nombre de v�rtexs i �ndexs.
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
   * M�tode de renderitzaci� dels v�rtexs.
   * @param _pRM Render Manager.
   * @return True si s'ha renderitzat correctament, false sino.
  **/
  virtual bool Render(CRenderManager *_pRM) const         = 0;

  /**
   * M�tode de renderitzaci� dels v�rtexs usant shaders.
   * @param _pRM Render Manager.
   * @param _pEffectTechnique T�cnica.
   * @return True si s'ha renderitzat correctament, false sino.
  **/
  virtual bool Render(CRenderManager *_pRM, CEffectTechnique *_pEffectTechnique) const         = 0;

  /**
   * Getter del nombre de triangles.
   * @return El nombre de triangles.
  **/
  virtual inline size_t GetFacesCount() const           {return m_iIndexCount/3;};
  /**
   * Getter del nombre de v�rtexs.
   * @return El nombre de v�rtexs.
  **/
  virtual inline size_t GetVertexsCount() const         {return m_iVertexCount;};

  /**
   * Getter del tipus de v�rtex.
   * @return El tipus de v�rtex.
  **/
  virtual inline unsigned short GetVertexType() const   = 0;
  /**
   * Getter del tamany del v�rtex.
   * @return El tamany del v�rtex en bytes.
  **/
  virtual inline size_t GetVertexSize() const           = 0;
  /**
   * Getter del tamany de l'�ndex.
   * @return El tamany de l'�ndex en bytes.
  **/
  virtual inline size_t GetIndexSize() const            = 0;
};

#endif