#pragma once

#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

// NO INCLOURE AQUEST ARXIU !!!!!!!!!!!!!!!! !!!!!!!!!!!!!!!!!!!!!!

// incloure "EntityDefines.h"

#include "base.h"

#ifndef __ENTITY_DEFINES_H__
#error S'ha d'incloure "EntityDefines.h" i no "GameEntity.h"
#endif

class CBaseComponent;
class CEntityManager;
class CRenderManager;

class CGameEntity:
  public CBaseControl
{
public:

  int GetGUID() const {return m_iGUID;};
  
  template<class T>
  T* GetComponent(CBaseComponent::Type _type) const;
  template<class T>
  T* GetComponent() const;
  
  
  void         PreUpdate(float _fDeltaTime)      ;
  void         Update(float _fDeltaTime)         ;
  void         UpdatePrePhysX(float _fDeltaTime) ;
  void         UpdatePostPhysX(float _fDeltaTime);
  void         UpdatePostAnim(float _fDeltaTime) ;
  void         PostUpdate(float _fDeltaTime)     ;

  void         DebugRender(CRenderManager*)      ;


  string GetName() const;

protected:

  virtual void Release();

private:
  CGameEntity(int _iId):m_iGUID(_iId),m_pszName(0) {SetOk(true);};
  ~CGameEntity() {Done();};
  void AddComponent(CBaseComponent* _pComponent);
  void AddCachedComponents();
  void ReceiveEvent(const SEvent& _Event);

  map<CBaseComponent::Type, CBaseComponent*> m_vNewEntities;

  map<CBaseComponent::Type, CBaseComponent*> m_mComponents;
  vector<CBaseComponent*>                    m_vComponents;
  int                     m_iGUID;
  const string*           m_pszName;
  
  friend class CBaseComponent;
  friend class CEntityManager;
};


template<class T>
T* CGameEntity::GetComponent(CBaseComponent::Type _type) const
{
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_it = m_mComponents.find(_type);

  if(l_it == m_mComponents.cend())
  {
    l_it = m_vNewEntities.find(_type);
    if(l_it == m_vNewEntities.cend())
    {
      return 0;
    }
  }
  return dynamic_cast<T*>(l_it->second);
}


template<class T>
T* CGameEntity::GetComponent() const
{
  map<CBaseComponent::Type, CBaseComponent*>::const_iterator l_it = m_mComponents.find(T::GetStaticType());

  if(l_it == m_mComponents.cend())
  {
    l_it = m_vNewEntities.find(T::GetStaticType());
    if(l_it == m_vNewEntities.cend())
    {
      return 0;
    }
  }
  return dynamic_cast<T*>(l_it->second);
}


#endif
