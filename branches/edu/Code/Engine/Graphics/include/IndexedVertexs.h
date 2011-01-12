#pragma once
#ifndef __INDEXED_VERTEXS_H__
#define __INDEXED_VERTEXS_H__

#include "RenderableVertexs.h"
#include "RenderManager.h"

/**
 * Classe template d'Indexed Vertexs.
 * Classe template que cont� un V�rtex Buffer i un �ndex Buffer i els gestiona internament per facilitar-ne l'us.
**/
template<class T>
class CIndexedVertexs:
  public CRenderableVertexs
{
protected:
  /**
   * Getter del tamany de v�rtex.
   * @return El tamany del v�rtex en bytes.
  **/
  inline size_t GetVertexSize() const {return sizeof(T);}
  /**
   * Getter del tamany de l'�ndex.
   * @return El tamany de l'�ndex en bytes.
  **/
  inline size_t GetIndexSize() const {return sizeof(unsigned short);}

public:
  /**
   * Constructor.
   * @param _pRM Render Manager.
   * @param _pVertexAddress Adre�a d'inici d'on s'allotgen els v�rtexs.
   * @param _pIndexAddress Adre�a d'inici d'on s'allotgen els �ndexs.
   * @param _iVertexCount Nombre de v�rtexs allotjats.
   * @param _iIndexCount Nombre d'�ndexs allotjats.
  **/
  CIndexedVertexs(CRenderManager *_pRM, char *_pVertexAddress, uint16 *_pIndexAddress, size_t _iVertexCount, size_t _iIndexCount);
  /**
   * Constructor.
   * @param _pRM Render Manager.
   * @param _pVertexAddress Adre�a d'inici d'on s'allotgen els v�rtexs.
   * @param _pIndexAddress Adre�a d'inici d'on s'allotgen els �ndexs.
   * @param _iVertexCount Nombre de v�rtexs allotjats.
   * @param _iIndexCount Nombre d'�ndexs allotjats.
  **/
  CIndexedVertexs(CRenderManager *_pRM, T *_pVertexAddress, uint16 *_pIndexAddres, size_t _iVertexCount, size_t _iIndexCount);
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  virtual ~CIndexedVertexs(){};
  /**
   * M�tode de render.
   * Aquest m�tode renderitza els v�rtexs segons l'�ndex buffer especificat.
   * @param _pRM Render Manager.
   * @return True si s'ha renderitzat correctament, false sino.
  **/
  virtual bool Render(CRenderManager *_pRM) const;
  /**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  virtual inline unsigned short GetVertexType() const {return T::GetFVF();}
};


template<class T>
CIndexedVertexs<T>::CIndexedVertexs(CRenderManager *_pRM, T *_pVertexAddress, uint16 *_pIndexAddress, size_t _iVertexCount, size_t _iIndexCount)
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
  memcpy      (l_p, _pIndexAddress,                 GetIndexSize()*_iIndexCount);
  m_pIB->Unlock();
};

template<class T>
CIndexedVertexs<T>::CIndexedVertexs(CRenderManager *_pRM, char *_pVertexAddress, uint16 *_pIndexAddress, size_t _iVertexCount, size_t _iIndexCount)
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
  memcpy      (l_p, _pIndexAddress,                 GetIndexSize()*_iIndexCount);
  m_pIB->Unlock();
};

template<class T>
bool CIndexedVertexs<T>::Render(CRenderManager *_pRM) const
{
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  l_pDevice->SetIndices(m_pIB);
  l_pDevice->SetStreamSource(0,m_pVB,0,GetVertexSize());
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