#include "Room.h"
#include "RenderManager.h"
#include "RenderableObjectsManager.h"
#include "Core.h"


bool CRoom::Init(CXMLTreeNode& _xmlRoom, set<string>& _UsedNames)
{
  assert(strcmp(_xmlRoom.GetName(), "Room") == 0);

  bool l_bUndefined = _xmlRoom.GetBoolProperty("undefined",false,false);
  string name = _xmlRoom.GetPszISOProperty("name", "undefined", !l_bUndefined);

  SetName(name);

  LOGGER->AddNewLog(ELL_INFORMATION, "CRoom::Init loading room \"%s\"", name.c_str());

  {
    CXMLTreeNode l_xmlBoundings = _xmlRoom["Boundings"];

    if(l_xmlBoundings.Exists())
    {
      int l_iBoundings = l_xmlBoundings.GetNumChildren();
      for(int i = 0; i < l_iBoundings; ++i)
      {
        CXMLTreeNode l_xmlBounding = l_xmlBoundings(i);
        if(strcmp( l_xmlBounding.GetName(), "Box" ) == 0)
        {
          Vect3f l_Size     = l_xmlBounding.GetVect3fProperty("size", Vect3f(1));
          Vect3f l_Position = l_xmlBounding.GetVect3fProperty("position", Vect3f());
          float l_fYaw      = l_xmlBounding.GetFloatProperty("yaw");
          float l_fPitch    = l_xmlBounding.GetFloatProperty("pitch");
          float l_fRoll     = l_xmlBounding.GetFloatProperty("roll");

          CObject3D l_O3D(l_Position, l_fYaw, l_fPitch, l_fRoll);
          l_O3D.GetBoundingBox()->Init(l_Size);

          m_Boundings.push_back(l_O3D);
        }
        else if( !l_xmlBounding.IsComment() )
        {
          LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init Room \"%s\" unknown bounding \"%s\"", name.c_str(), l_xmlBounding.GetName());
        }
      }
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init Room \"%s\" without boundings", name.c_str());
    }
  }

  {
    CXMLTreeNode l_xmlObjects = _xmlRoom["Objects"];
    CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

    if(l_xmlObjects.Exists())
    {
      int l_iObjects = l_xmlObjects.GetNumChildren();
      for(int i = 0; i < l_iObjects; ++i)
      {
        CXMLTreeNode l_xmlGameObject = l_xmlObjects(i);
        if(strcmp( l_xmlGameObject.GetName(), "GameObject" ) == 0)
        {
          string l_szName = l_xmlGameObject.GetPszISOProperty("name", "", false);

          if(l_szName != "")
          {
            CRenderableObject* l_pRO = l_pROM->GetResource(l_szName);
            if(l_pRO && l_pRO->IsOk())
            {
              if(_UsedNames.find(l_szName) == _UsedNames.end())
              {
                m_RenderableObjects.insert(l_pRO);
              }
              else
              {
                LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init GameObject \"%s\" repetit!", l_szName.c_str());
              }
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init GameObject \"%s\" is not in the RO Manager!", l_szName.c_str());
            }
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init GameObject without name!");
          }
        }
        else if( !l_xmlGameObject.IsComment() )
        {
          LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init Room \"%s\" unknown object \"%s\"", name.c_str(), l_xmlGameObject.GetName());
        }
      }
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING, "CRoom::Init Room \"%s\" without objects", name.c_str());
    }
  }

  SetOk(true);
  return IsOk();
}

void CRoom::Render(CRenderManager* _pRM, priority_queue<CRenderableObject*,vector< CRenderableObject*>, CRenderableObjectOrdering>& _BlendQueue) const
{
  const CFrustum& l_Frustum = _pRM->GetFrustum();
  
  set<CRenderableObject*>::const_iterator l_it  = m_RenderableObjects.cbegin();
  set<CRenderableObject*>::const_iterator l_end = m_RenderableObjects.cend();

  for(; l_it != l_end; ++l_it)
  {
    CRenderableObject* l_pRenderableObject = *l_it;
    if(l_pRenderableObject->GetVisible())
    {
      Vect3f l_Center = l_pRenderableObject->GetBoundingSphere()->GetMiddlePoint() + l_pRenderableObject->GetPosition();
      D3DXVECTOR3 l_d3Center(l_Center.x,l_Center.y,l_Center.z);

      if(l_Frustum.SphereVisible(l_d3Center, l_pRenderableObject->GetBoundingSphere()->GetRadius()))
      {
        if(l_pRenderableObject->IsAlphaBlended())
        {
          _BlendQueue.push(l_pRenderableObject);
        }
        else
        {
          l_pRenderableObject->Render(_pRM);
        }
      }
    }
  }
}

void CRoom::AddRendeableObject(CRenderableObject* _pRO)
{
  m_RenderableObjects.insert(_pRO);
}

void CRoom::RemoveRendeableObject(CRenderableObject* _pRO)
{
  m_RenderableObjects.erase(_pRO);
}
