#pragma once 
#ifndef __RECYCLING_ARRAY_H__
#define __RECYCLING_ARRAY_H__

#include "base.h"
#include "Utils/BaseControl.h"

template<class T>
class CRecyclingArray:
  public CBaseControl
{
public:
  CRecyclingArray(uint32 _uiMax):m_vRecyclingArray(0),m_vRecyclingArrayStatus(0) { Reset(_uiMax); };
  ~CRecyclingArray() { Done(); };
  
  void Reset(uint32 _uiMax);

  const uint32 GetNumFreeElements() const { return m_uiFreeElements; };
  const uint32 GetNumUsedElements() const { return m_uiUsedElements; };
  const uint32 GetTotalElements() const { return m_uiTotalElements; };

  T* GetAt(uint32 _uiIndex) const;
  bool IsFree(uint32 _uiIndex) const;
  
  T* New();
  int NewIndex();
  void Free(uint32 _uiIndex);
  void DeleteAllElements();

protected:
  void Release();

private:

  uint32  m_uiFreeElements;
  uint32  m_uiUsedElements;
  uint32  m_uiTotalElements;
  T*      m_vRecyclingArray;
  T       m_TDefault;
  //true si est� lliure, false si est� ocupat
  bool*   m_vRecyclingArrayStatus;
};

template<class T>
void CRecyclingArray<T>::Reset(uint32 _uiMax)
{
   m_uiUsedElements = 0;
   m_uiFreeElements  = 
   m_uiTotalElements = _uiMax;
   
                    
  if(_uiMax > 0)
    SetOk(true);
  else
  {
    return;
    SetOk(false);
  }

  CHECKED_DELETE_ARRAY(m_vRecyclingArray);
  CHECKED_DELETE_ARRAY(m_vRecyclingArrayStatus);

  m_vRecyclingArray       = new T[m_uiTotalElements];
  m_vRecyclingArrayStatus = new bool[m_uiTotalElements];

  //tots els elements estan lliures
  for(uint32 i = 0; i < m_uiTotalElements; i++)
  {
    m_vRecyclingArrayStatus[i] = true;
  }
};

template<class T>
void CRecyclingArray<T>::Release(void)
{
  CHECKED_DELETE_ARRAY(m_vRecyclingArray);
  CHECKED_DELETE_ARRAY(m_vRecyclingArrayStatus);
};

template<class T>
void CRecyclingArray<T>::DeleteAllElements(void)
{
  assert(IsOk());

  //tots els elements estan lliures
  for(uint32 i = 0; i < m_uiTotalElements; i++)
    m_vRecyclingArrayStatus[i] = true;

  m_uiFreeElements = m_uiTotalElements;
  m_uiUsedElements = 0;
};

template<class T>
void CRecyclingArray<T>::Free(uint32 _uiIndex)
{
  assert(IsOk());

  if(!m_vRecyclingArrayStatus[_uiIndex])
  {
    m_uiUsedElements--;
    m_uiFreeElements++;
    m_vRecyclingArrayStatus[_uiIndex] = true;
  }
};

template<class T>
T* CRecyclingArray<T>::New(void)
{
  assert(IsOk());

  bool   l_bFound  = false;
  uint32 l_uiIndex = 0;
  
  if(m_uiFreeElements > 0)
  {
    while(!l_bFound && l_uiIndex < m_uiTotalElements)
    {
      l_bFound = m_vRecyclingArrayStatus[l_uiIndex];
      l_uiIndex++;
    }
  }

  if(l_bFound)
  {
    l_uiIndex--;
    m_uiFreeElements--;
    m_uiUsedElements++;
    m_vRecyclingArrayStatus[l_uiIndex] = false;
    m_vRecyclingArray[l_uiIndex] = m_TDefault;
    return &(m_vRecyclingArray[l_uiIndex]);
  }else
    return 0;
};

template<class T>
int CRecyclingArray<T>::NewIndex(void)
{
  assert(IsOk());

  bool   l_bFound  = false;
  uint32 l_uiIndex = 0;
  
  if(m_uiFreeElements > 0)
  {
    while(!l_bFound && l_uiIndex < m_uiTotalElements)
    {
      l_bFound = m_vRecyclingArrayStatus[l_uiIndex];
      l_uiIndex++;
    }
  }

  if(l_bFound)
  {
    l_uiIndex--;
    m_uiFreeElements--;
    m_uiUsedElements++;
    m_vRecyclingArrayStatus[l_uiIndex] = false;
    m_vRecyclingArray[l_uiIndex] = m_TDefault;
    return l_uiIndex;
  }else
    return -1;
};

template<class T>
bool CRecyclingArray<T>::IsFree(uint32 _uiIndex) const
{
  assert(IsOk());

  return m_vRecyclingArrayStatus[_uiIndex];
};

template<class T>
T* CRecyclingArray<T>::GetAt(uint32 _uiIndex) const
{
  assert(IsOk());

  return &(m_vRecyclingArray[_uiIndex]);
};

#endif