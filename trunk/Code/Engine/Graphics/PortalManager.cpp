#include "PortalManager.h"

#include <XML\XMLTreeNode.h>

bool CPortalManager::Init(CXMLTreeNode& _xmlLevel)
{
  assert(strcmp(_xmlLevel.GetName(), "Level") == 0);
  SetOk(true);

  LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init");

  {
    CXMLTreeNode l_xmlRooms = _xmlLevel["Rooms"];

    set<string> _UsedGameObjects;

    if(l_xmlRooms.Exists())
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init Recorrent Rooms");

      int l_iRooms = l_xmlRooms.GetNumChildren();
      for(int i = 0; i < l_iRooms; ++i)
      {
        CXMLTreeNode l_xmlRoom = l_xmlRooms(i);
        if(strcmp(l_xmlRoom.GetName(), "Room") == 0)
        {
          bool l_bUndefined = l_xmlRoom.GetBoolProperty("undefined",false,false);
          string l_szName = l_xmlRoom.GetPszISOProperty("name", "undefined", l_bUndefined);

          if(m_Rooms.find(l_szName) != m_Rooms.end())
          {
            CRoom l_Room;
            if(l_Room.Init(l_xmlRoom, _UsedGameObjects))
            {
              m_Rooms[l_szName] = l_Room;
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Room \"%s\" no OK", l_szName.c_str());
            }
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Room \"%s\" repetida", l_szName.c_str());
          }
        }
        else if(!l_xmlRoom.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Element no reconegut! \"%s\"", l_xmlRoom.GetName());
        }
      }
    }
    else
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init No Rooms!");
      SetOk(false);
    }
  }

  {
    CXMLTreeNode l_xmlPortals = _xmlLevel["Portals"];

    if(l_xmlPortals.Exists())
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init Recorrent Portals");

      int l_iPortals = l_xmlPortals.GetNumChildren();
      for(int i = 0; i < l_iPortals; ++i)
      {
        CXMLTreeNode l_xmlPortal = l_xmlPortals(i);
        if(strcmp(l_xmlPortal.GetName(), "Portal") == 0)
        {
          string l_szName = l_xmlPortal.GetPszISOProperty("name", "", true);

          if(m_Portals.find(l_szName) != m_Portals.end())
          {
            CPortal l_Portal;
            if(l_Portal.Init(l_xmlPortal, this))
            {
              m_Portals[l_szName] = l_Portal;
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Portal \"%s\" no OK", l_szName.c_str());
            }
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Portal \"%s\" repetit", l_szName.c_str());
          }
        }
        else if(!l_xmlPortal.IsComment())
        {
          LOGGER->AddNewLog(ELL_WARNING, "CPortalManager::Init Element no reconegut! \"%s\"", l_xmlPortal.GetName());
        }
      }
    }
    else
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init No Portals!");
      SetOk(false);
    }
  }

  return IsOk();
}

CRoom* CPortalManager::GetRoom  (const string& _szName)
{
  map<string,CRoom>::iterator l_it = m_Rooms.find(_szName);
  if(l_it == m_Rooms.cend())
  {
    return 0;
  }
  else
  {
    return &(l_it->second);
  }
}

CPortal* CPortalManager::GetPortal(const string& _szName)
{
  map<string,CPortal>::iterator l_it = m_Portals.find(_szName);
  if(l_it == m_Portals.cend())
  {
    return 0;
  }
  else
  {
    return &(l_it->second);
  }
}
