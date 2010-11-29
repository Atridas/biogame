#pragma once
#ifndef __RENDERABLE_VERTEXS_H__
#define __RENDERABLE_VERTEXS_H__

#include <d3d9.h>

#include <base.h>

//forward declarations ------------------------
class CRenderManager;
//---------------------------------------------

class CRenderableVertexs
{
protected:
  LPDIRECT3DVERTEXBUFFER9 m_pVB;
  LPDIRECT3DINDEXBUFFER9 m_pIB;
  size_t m_iIndexCount, m_iVertexCount;
public:
  CRenderableVertexs():m_pVB(0),m_pIB(0),m_iIndexCount(0),m_iVertexCount(0) 
                                                        {};
  virtual ~CRenderableVertexs()                         {CHECKED_RELEASE(m_pVB)CHECKED_RELEASE(m_pIB)};

  virtual bool Render(CRenderManager *RM) const         = 0;

  virtual inline size_t GetFacesCount() const           {return m_iIndexCount/3;};
  virtual inline size_t GetVertexsCount() const         {return m_iVertexCount;};

  virtual inline unsigned short GetVertexType() const   = 0;
  virtual inline size_t GetVertexSize()                 = 0;
  virtual inline size_t GetIndexSize()                  = 0;
};

#endif