#include "Portal.h"

#include "XML\XMLTreeNode.h"
#include "PortalManager.h"

bool CPortal::Init(CXMLTreeNode& _xmlPortal, CPortalManager* _pPortalManager)
{
  assert(strcmp(_xmlPortal.GetName(), "Portal") == 0);

  SetOk(true);

  string l_szName = _xmlPortal.GetPszISOProperty("name", "undefined", false);

  SetName(l_szName);

  LOGGER->AddNewLog(ELL_INFORMATION, "CPortal::Init loading portal \"%s\"", l_szName.c_str());
  
  string l_szRoomA = _xmlPortal.GetPszISOProperty("roomA", "", false);
  string l_szRoomB = _xmlPortal.GetPszISOProperty("roomB", "", false);
  
  Vect3f l_vSize     = _xmlPortal.GetVect3fProperty("size", Vect3f(), true);
  Vect3f l_vPosition = _xmlPortal.GetVect3fProperty("position", Vect3f(), true);
  float  l_fYaw      = _xmlPortal.GetFloatProperty("yaw");
  float  l_fPitch    = _xmlPortal.GetFloatProperty("pitch");
  float  l_fRoll     = _xmlPortal.GetFloatProperty("roll");

  if(l_szRoomA == "")
  {
    LOGGER->AddNewLog(ELL_ERROR, "CPortal::Init Falta la Room A");
    SetOk(false);
  }
  else if(l_szRoomB == "")
  {
    LOGGER->AddNewLog(ELL_ERROR, "CPortal::Init Falta la Room B");
    SetOk(false);
  }
  else if(l_szRoomA == l_szRoomB)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CPortal::Init La Room A i la B són la mateixa!");
    SetOk(false);
  }
  else
  {
    m_pRoomA = _pPortalManager->GetRoom(l_szRoomA);
    m_pRoomB = _pPortalManager->GetRoom(l_szRoomB);

    if(!m_pRoomA)
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortal::Init La Room A \"%s\" no existeix al manager!", l_szRoomA.c_str());
      SetOk(false);
    }
    else if(!m_pRoomB)
    {
      LOGGER->AddNewLog(ELL_ERROR, "CPortal::Init La Room B \"%s\" no existeix al manager!", l_szRoomB.c_str());
      SetOk(false);
    }
    else
    {
      SetYaw(l_fYaw);
      SetPitch(l_fPitch);
      SetRoll(l_fRoll);
      SetPosition(l_vPosition);

      GetBoundingBox()->Init(l_vSize);
    }
  }

  return IsOk();
}

