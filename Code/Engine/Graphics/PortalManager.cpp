#include "PortalManager.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Core.h"
#include "EffectManager.h"
#include "Portal.h"

#include <XML\XMLTreeNode.h>


bool CPortalManager::Init(const string& _szFileName)
{
  CXMLTreeNode l_xmlLevel;

  if(l_xmlLevel.LoadFile(_szFileName.c_str()))
  {
    Init(l_xmlLevel);
  }
  else
  {
    SetOk(false);
    LOGGER->AddNewLog(ELL_ERROR, "CPortalManager::Init file \"%s\" not found or incorrect.");
  }

  return IsOk();
}

bool CPortalManager::Init(CXMLTreeNode& _xmlLevel)
{
  assert(strcmp(_xmlLevel.GetName(), "Level") == 0);
  SetOk(true);

  LOGGER->AddNewLog(ELL_INFORMATION, "CPortalManager::Init");

  {
    CXMLTreeNode l_xmlRooms = _xmlLevel["Rooms"];

    set<string> _UsedGameObjects;

    if(l_xmlRooms.Exists())
    {
      LOGGER->AddNewLog(ELL_INFORMATION, "CPortalManager::Init Recorrent Rooms");

      int l_iRooms = l_xmlRooms.GetNumChildren();
      for(int i = 0; i < l_iRooms; ++i)
      {
        CXMLTreeNode l_xmlRoom = l_xmlRooms(i);
        if(strcmp(l_xmlRoom.GetName(), "Room") == 0)
        {
          bool l_bUndefined = l_xmlRoom.GetBoolProperty("undefined",false,false);
          string l_szName = l_xmlRoom.GetPszISOProperty("name", "undefined", !l_bUndefined);

          if(m_Rooms.find(l_szName) == m_Rooms.end())
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
      LOGGER->AddNewLog(ELL_INFORMATION, "CPortalManager::Init Recorrent Portals");

      int l_iPortals = l_xmlPortals.GetNumChildren();
      for(int i = 0; i < l_iPortals; ++i)
      {
        CXMLTreeNode l_xmlPortal = l_xmlPortals(i);
        if(strcmp(l_xmlPortal.GetName(), "Portal") == 0)
        {
          string l_szName = l_xmlPortal.GetPszISOProperty("name", "", true);

          if(m_Portals.find(l_szName) == m_Portals.end())
          {
            CPortal l_Portal;
            m_Portals[l_szName] = l_Portal;
            if(!m_Portals[l_szName].Init(l_xmlPortal, this))
            {
              m_Portals.erase(l_szName);
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


struct SRP { 
  CRoom* r; 
  CPortal* p;
  SRP(CRoom* _r, CPortal* _p):r(_r),p(_p) {};
};

void RenderPortals(CRoom *_pRoom, CRenderManager* _pRM, const Vect3f& _vCameraEye, const CFrustum& _Frustum, set<CPortal*> _PrevPortals, CRoom::TBlendQueue& _BlendQueue)
{
  _pRoom->Render(_pRM,_Frustum,_BlendQueue);
  const vector<CPortal*>& l_Portals = _pRoom->GetPortals();
    
  vector<SRP> l_NextRooms;

  {
    vector<CPortal*>::const_iterator l_it  = l_Portals.cbegin();
    vector<CPortal*>::const_iterator l_end = l_Portals.cend();
    for(; l_it != l_end; ++l_it)
    {
      CPortal* l_pPortal = *l_it;
      if(_PrevPortals.find(l_pPortal) == _PrevPortals.cend())
      {
        _PrevPortals.insert(l_pPortal);
        if(l_pPortal->GetRoomA() == _pRoom)
        {
          assert(l_pPortal->GetRoomB() != _pRoom);
          l_NextRooms.push_back( SRP(l_pPortal->GetRoomB(),l_pPortal) );
        }
        else
        {
          assert(l_pPortal->GetRoomB() == _pRoom);
          l_NextRooms.push_back( SRP(l_pPortal->GetRoomA(),l_pPortal) );
        }
      }
    }
  }
  {
    vector<SRP>::const_iterator l_it  = l_NextRooms.cbegin();
    vector<SRP>::const_iterator l_end = l_NextRooms.cend();
    for(; l_it != l_end; ++l_it)
    {
      const Vect3f * l_PortalPoints = l_it->p->GetBoundingBox()->GetBox();
      Vect3f l_TransformedPoints[8];
      Mat44f l_matPortal = l_it->p->GetMat44();

      for(int i = 0; i < 8; ++i)
      {
        l_TransformedPoints[i] = l_matPortal * l_PortalPoints[i];
      }

      if(_Frustum.BoxVisibleByVertexs(l_TransformedPoints))
      {
        CFrustum l_Frustum(_Frustum);
        l_Frustum.Update(_vCameraEye, l_TransformedPoints, 8);

        {//TODO moure-ho a debug-render
          Mat44f i;
          i.SetIdentity();
          _pRM->SetTransform(i);
          _pRM->DrawFrustum(&l_Frustum, colYELLOW);
        }

        RenderPortals(l_it->r,_pRM,_vCameraEye,l_Frustum,_PrevPortals,_BlendQueue);
      }
    }
  }
}


void CPortalManager::Render(CRenderManager* _pRM)
{
  // 1 busquem en quina habitació està la camera.

  CCamera *l_pCamera = _pRM->GetCamera();
  Vect3f l_vCameraEye = l_pCamera->GetEye();

  map<string,CRoom>::iterator l_it = m_Rooms.find(m_szCameraLastRoom);
  CRoom *l_pCameraRoom = 0;

  CObject3D l_CameraObject;
  l_CameraObject.SetPosition( l_vCameraEye );
  l_CameraObject.GetBoundingSphere()->Init(Vect3f(0,0,0),0.2f);

  if(l_it != m_Rooms.end())
  {
    l_pCameraRoom = &(l_it->second);
    if(!l_pCameraRoom->IsObject3DSphereInRoom(l_CameraObject))
    {
      l_pCameraRoom = 0;
    }
  }
  
  if(!l_pCameraRoom)
  {
    map<string,CRoom>::iterator l_end = m_Rooms.end();
    for(l_it = m_Rooms.begin(); l_it != l_end; ++l_it)
    {
      if(l_it->second.IsObject3DSphereInRoom(l_CameraObject))
      {
        m_szCameraLastRoom = l_it->first;
        l_pCameraRoom = &(l_it->second);
        break;
      }
    }
  }

  CFrustum l_Frustum = _pRM->GetFrustum();

  Vect3f   l_vEye    = (l_pCamera)? l_pCamera->GetEye() : Vect3f(0,0,-1);
  CRenderableObjectOrdering l_Ordering(l_vEye);

  CRoom::TBlendQueue l_BlendQueue(l_Ordering);

  if(!l_pCameraRoom)
  {
    map<string,CRoom>::iterator l_end = m_Rooms.end();
    for(l_it = m_Rooms.begin(); l_it != l_end; ++l_it)
    {
      l_it->second.Render(_pRM,l_Frustum,l_BlendQueue);
    }
  }
  else
  {
    set<CPortal*> l_PrevPortals;
    RenderPortals(l_pCameraRoom, _pRM, l_vCameraEye, l_Frustum, l_PrevPortals, l_BlendQueue);
  }


  CORE->GetEffectManager()->ActivateAlphaRendering();

  while(!l_BlendQueue.empty())
  {
    CRenderableObject* l_pRenderableObject = l_BlendQueue.top();
    l_pRenderableObject->Render(_pRM);
    l_BlendQueue.pop();
  }
}

void CPortalManager::DebugRender(CRenderManager* _pRM) const
{
  
  map<string,CPortal>::const_iterator l_itP  = m_Portals.cbegin();
  map<string,CPortal>::const_iterator l_endP = m_Portals.cend();

  for(; l_itP != l_endP; ++l_itP)
  {
    l_itP->second.DebugRender(_pRM);
  }

  map<string,CRoom>::const_iterator l_itR  = m_Rooms.cbegin();
  map<string,CRoom>::const_iterator l_endR = m_Rooms.cend();

  for(; l_itR != l_endR; ++l_itR)
  {
    l_itR->second.DebugRender(_pRM);
  }
}
