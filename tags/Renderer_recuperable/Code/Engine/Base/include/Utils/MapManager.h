#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include "Base.h"
#include "Utils/Logger.h"
#include <string>
#include <map>

/**
 * Template MapManager.
 * Classe template que servirà com a base per els diferents Managers implementats al motor.
**/
template<class T>
class CMapManager:
  public CBaseControl
{
protected:
  typedef std::map<std::string, T*> TMapResource;
  /**
   * Mapa Hash on s'enmagatzemaran els recursos.
  **/
  TMapResource m_Resources;

public:
  /**
   * Getter de recursos.
   * Aquest mètode retornarà el recurs mapejat segons el nom indicat.
   * @param _szName Index on es troba el recurs a retornar.
   * @return El recurs.
  **/
  virtual T * GetResource(const std::string &_szName) const
  {
    TMapResource::const_iterator l_It = m_Resources.find(_szName);
    if(l_It == m_Resources.end()) 
    {
      return 0;
    }
    return l_It->second;
  }

  /**
   * Mètode per afegir recursos.
   * Aquest mètode afegirà un recurs T segons el hash especificat.
   * @param _szName Hash on situar el recurs.
   * @param _pResource Recurs a enmagatzemar.
  **/
  virtual void AddResource(const std::string &_szName, T *_pResource) 
  {
    m_Resources[_szName] = _pResource;
  }

  /**
   * Mètode d'alliberament de recursos.
  **/
  virtual void Release() 
  {
    TMapResource::iterator l_It = m_Resources.begin();
    TMapResource::iterator l_End = m_Resources.end();
    while(l_It != l_End) 
    {
      CHECKED_DELETE(l_It->second);
      ++l_It;
    }

    m_Resources.clear();
  }
};

#endif //INC_MAP_MANAGER_H_
