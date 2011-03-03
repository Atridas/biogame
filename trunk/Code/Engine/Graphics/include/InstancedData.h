#pragma once
#ifndef INC_INSTANCED_DATA_H_
#define INC_INSTANCED_DATA_H_

#include <d3d9.h>
#include "RenderManager.h"
#include <base.h>

template<class T>
class CInstancedData : CBaseControl
{
public:
  CInstancedData(void) : m_uiMaxNumInstances(0),m_pVB(0),m_pBuffer(0)
  {
  
  }

  ~CInstancedData(void)
  {
    Done();
  }

  bool Init(CRenderManager* _pRM, uint32 _uiMaxNumInstances)
  {
    m_uiMaxNumInstances = _uiMaxNumInstances;

    LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();

    UINT l_uiSize = sizeof(T)*m_uiMaxNumInstances;

    if(!FAILED(l_pDevice->CreateVertexBuffer(l_uiSize,D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&m_pVB,NULL)))
    {
      SetOk(true);
    }else{
      LOGGER->AddNewLog(ELL_ERROR,"CInstancedData::Init Error al crear el buffer a la vram");
      SetOk(false);
    }

    if(IsOk())
    {
      m_pBuffer = new T[_uiMaxNumInstances];
    }

    return IsOk();
  }

  bool SetData(const T* _pData, uint32 _uiNumInstances, CRenderManager* _pRM)
  {
    assert(IsOk());
    
    if(_uiNumInstances > m_uiMaxNumInstances)
    {
      Release();

      if(!Init(_pRM,_uiNumInstances))
        return false;
    }

    UINT l_uiSize = sizeof(T)*_uiNumInstances;

    void* l_p;
    if(FAILED(m_pVB->Lock (0,l_uiSize,&l_p,0)))
    {
      return false;
    }
    memcpy(l_p,_pData,l_uiSize);
    return !FAILED(m_pVB->Unlock());
  }

  bool SetStreamSource(CRenderManager* _pRM, int _iStream = 1)
  {
    LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
    return !FAILED(l_pDevice->SetStreamSource(_iStream, m_pVB, 0, sizeof(T)));
  }

  T* GetBuffer(uint32 _uiNumInstances, CRenderManager* _pRM)
  {
    assert(IsOk());
    
    if(_uiNumInstances > m_uiMaxNumInstances)
    {
      Release();

      if(!Init(_pRM,_uiNumInstances))
        return 0;
    }

    return m_pBuffer;
  }

private:

  virtual void Release()
  {
    CHECKED_RELEASE(m_pVB);
    CHECKED_DELETE(m_pBuffer);
  }

  uint32                  m_uiMaxNumInstances;
  LPDIRECT3DVERTEXBUFFER9 m_pVB;
  T*                      m_pBuffer;

};

#endif //INC_INSTANCED_DATA_H_

