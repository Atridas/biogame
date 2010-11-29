#pragma once
#ifndef __INDEXED_VERTEXS_H__
#define __INDEXED_VERTEXS_H__

#include "RenderableVertexs.h"
#include "RenderManager.h"

template<class T>
class CIndexedVertexs:
  public CRenderableVertexs
{
protected:
  inline size_t GetVertexSize() {return sizeof(T);}
  inline size_t GetIndexSize() {return sizeof(unsigned short);}
public:
  CIndexedVertexs(CRenderManager *_pRM, void *_pVertexAddress, void *_pIndexAddres, size_t _iVertexCount, size_t _iIndexCount);
  virtual ~CIndexedVertexs(){};
  virtual bool Render(CRenderManager *_pRM) const;
  virtual inline unsigned short GetVertexType() const {return T::GetFVF();}
};


template<class T>
CIndexedVertexs<T>::CIndexedVertexs(CRenderManager *_pRM, void *_pVertexAddress, void *_pIndexAddres, size_t _iVertexCount, size_t _iIndexCount)
{
  m_iIndexCount   = _iIndexCount;
  m_iVertexCount  = _iVertexCount;
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  l_pDevice->CreateVertexBuffer(GetVertexSize()*_iVertexCount,0,GetVertexType(),D3DPOOL_DEFAULT,&m_pVB,NULL);
  l_pDevice->CreateIndexBuffer(GetIndexSize()*_iIndexCount,0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_pIB,NULL);

  void* l_p;
  m_pVB->Lock (0,   GetVertexSize()*_iVertexCount,  &l_p,0);
  memcpy      (l_p, _pVertexAddress,                GetVertexSize()*_iVertexCount);
  m_pVB->Unlock();

  m_pIB->Lock (0,   GetIndexSize()*_iIndexCount,    &l_p,0);
  memcpy      (l_p, _pIndexAddres,                  GetIndexSize()*_iIndexCount);
  m_pIB->Unlock();
};

template<class T>
bool CIndexedVertexs<T>::Render(CRenderManager *_pRM) const
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  l_pDevice->SetIndices(m_pIB);
  l_pDevice->SetStreamSource(0,m_pVB,0,GetVertexType());
  l_pDevice->SetFVF(T::GetFVF());
  l_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, // PrimitiveType
                                  0,                  // BaseVertexIndex
                                  0,                  // MinIndex
                                  GetVertexsCount(),  // NumVertices
                                  0,                  // StartIndex
                                  GetFacesCount() );

  return true;
}

#endif