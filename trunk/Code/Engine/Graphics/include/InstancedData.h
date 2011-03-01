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
  CInstancedData(void) : m_uiMaxNumInstances(0),m_pVB(0)
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

    if(!FAILED(l_pDevice->CreateVertexBuffer(sizeof(T)*m_uiMaxNumInstances,0,0,D3DPOOL_DEFAULT,&m_pVB,NULL)))
    {
      SetOk(true);
    }else{
      LOGGER->AddNewLog(ELL_ERROR,"CInstancedData::Init Error al crear el buffer a la vram");
      SetOk(false);
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

    void* l_p;
    m_pVB->Lock (0,sizeof(T)*_uiNumInstances,&l_p,0);
    memcpy(l_p,_pData,sizeof(T)*_uiNumInstances);
    m_pVB->Unlock();
  }

private:

  virtual void Release()
  {
    CHECKED_RELEASE(m_pVB)
  }

  uint32 m_uiMaxNumInstances;
  
  LPDIRECT3DVERTEXBUFFER9 m_pVB;

};

#endif //INC_INSTANCED_DATA_H_

