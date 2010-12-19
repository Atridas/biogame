//----------------------------------------------------------------------------------
// CMapManager class
// Author: Enric Vergara
//
// Description:
// Template class for creating Map Managers
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MAP_MANAGER_H_
#define INC_MAP_MANAGER_H_

#include "Base.h"
#include "Utils/Logger.h"
#include <string>
#include <map>

template<class T>
class CMapManager
{
protected:
  typedef std::map<std::string, T*> TMapResource;
  TMapResource m_Resources;

public:
  virtual T * GetResource(const std::string &Name) 
  {
    TMapResource::iterator l_It = m_Resources.find(Name);
    if(l_It == m_Resources.end()) 
    {
      return 0;
    }
    return l_It->second;
  }

  virtual void AddResource(const std::string &Name, T *Resource) 
  {
    m_Resources[Name] = Resource;
  }

  void Destroy() 
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
