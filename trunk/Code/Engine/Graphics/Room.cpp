#include "Room.h"
#include "RenderManager.h"
#include "RenderableObjectsManager.h"
#include "Core.h"
#include "Portal.h"
#include "PortalManager.h"
#include "EmiterInstance.h"


bool CRoom::Init()
{
  SetName("undefinded");
  m_bRendered = 
  m_bNeightbour = true;
  SetOk(true);
  return IsOk();
}

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
          float l_fYaw      = l_xmlBounding.GetFloatProperty("yaw")   * FLOAT_PI_VALUE / 180.0f;
          float l_fPitch    = l_xmlBounding.GetFloatProperty("pitch") * FLOAT_PI_VALUE / 180.0f;
          float l_fRoll     = l_xmlBounding.GetFloatProperty("roll")  * FLOAT_PI_VALUE / 180.0f;

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
                _UsedNames.insert(l_szName);
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

  
  m_bRendered = 
  m_bNeightbour = true;
  SetOk(true);
  return IsOk();
}

void CRoom::Render(CRenderManager* _pRM, const CFrustum& _Frustum, TBlendQueue& _BlendQueue, TBlendQueue& _EmiterQueue) const
{
  {
    set<CRenderableObject*>::const_iterator l_it  = m_RenderableObjects.cbegin();
    set<CRenderableObject*>::const_iterator l_end = m_RenderableObjects.cend();

    for(; l_it != l_end; ++l_it)
    {
      CRenderableObject* l_pRenderableObject = *l_it;
      if(l_pRenderableObject->GetVisible())
      {
        Vect3f l_Center = l_pRenderableObject->GetBoundingSphere()->GetMiddlePoint() + l_pRenderableObject->GetPosition();
        D3DXVECTOR3 l_d3Center(l_Center.x,l_Center.y,l_Center.z);

        if(_Frustum.SphereVisible(l_d3Center, l_pRenderableObject->GetBoundingSphere()->GetRadius()))
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
  {
    set<CEmiterInstance*>::const_iterator l_it  = m_Emiters.cbegin();
    set<CEmiterInstance*>::const_iterator l_end = m_Emiters.cend();

    for(; l_it != l_end; ++l_it)
    {
      CEmiterInstance* l_pEmiter = *l_it;
      Vect3f l_Center = l_pEmiter->GetBoundingSphere()->GetMiddlePoint() + l_pEmiter->GetPosition();
      D3DXVECTOR3 l_d3Center(l_Center.x,l_Center.y,l_Center.z);

      if(_Frustum.SphereVisible(l_d3Center, l_pEmiter->GetBoundingSphere()->GetRadius()))
      {
        _EmiterQueue.push(l_pEmiter);
      }
    }
  }
}

void CRoom::Update(CPortalManager* _pPM)
{
  {
    set<CRenderableObject*> l_Remove;
    {
      set<CRenderableObject*>::const_iterator l_it  = m_RenderableObjects.cbegin();
      set<CRenderableObject*>::const_iterator l_end = m_RenderableObjects.cend();
      for(; l_it != l_end; ++l_it)
      {
        CRenderableObject* l_pRenderableObject = *l_it;

        if(m_Boundings.size() != 0)
        {
          //som en una habitació
          if(!IsObject3DSphereInRoom(l_pRenderableObject))
          {
            l_Remove.insert(l_pRenderableObject);

            vector<CPortal*>::const_iterator l_it = m_Portals.cbegin();
            vector<CPortal*>::const_iterator l_end = m_Portals.cend();
            for(; l_it != l_end; ++l_it)
            {
              CPortal* l_pPortal = *l_it;
              CRoom*   l_pOtherRoom = (l_pPortal->GetRoomA() == this) ? l_pPortal->GetRoomB() : l_pPortal->GetRoomA();
              if(l_pOtherRoom->IsObject3DSphereInRoom(l_pRenderableObject))
              {
                l_pOtherRoom->AddRendeableObject(l_pRenderableObject);
                break;
              }
            }
            if(l_it == l_end)
              _pPM->InsertRenderableObject(l_pRenderableObject);
          }
        }
        else
        {
          //Som a undefined
          set<string>::const_iterator l_it  = _pPM->GetRoomNames().cbegin();
          set<string>::const_iterator l_end = _pPM->GetRoomNames().cend();
          for(; l_it != l_end; ++l_it)
          {
            CRoom* l_pOtherRoom = _pPM->GetRoom(*l_it);
            if(l_pOtherRoom->IsObject3DSphereInRoom(l_pRenderableObject))
            {
              l_pOtherRoom->AddRendeableObject(l_pRenderableObject);
              l_Remove.insert(l_pRenderableObject);
              break;
            }
          }
        }
      }
    }
    {
      set<CRenderableObject*>::iterator l_it  = l_Remove.begin();
      set<CRenderableObject*>::iterator l_end = l_Remove.end();
      for(; l_it != l_end; ++l_it)
      {
        m_RenderableObjects.erase(*l_it);
      }
    }
  }


  {
    set<CEmiterInstance*> l_Remove;
    {
      set<CEmiterInstance*>::const_iterator l_it  = m_Emiters.cbegin();
      set<CEmiterInstance*>::const_iterator l_end = m_Emiters.cend();
      for(; l_it != l_end; ++l_it)
      {
        CEmiterInstance* l_pEmiter = *l_it;

        if(m_Boundings.size() != 0)
        {
          //som en una habitació
          if(!IsObject3DSphereInRoom(l_pEmiter))
          {
            l_Remove.insert(l_pEmiter);

            vector<CPortal*>::const_iterator l_it  = m_Portals.cbegin();
            vector<CPortal*>::const_iterator l_end = m_Portals.cend();
            for(; l_it != l_end; ++l_it)
            {
              CPortal* l_pPortal = *l_it;
              CRoom*   l_pOtherRoom = (l_pPortal->GetRoomA() == this) ? l_pPortal->GetRoomB() : l_pPortal->GetRoomA();
              if(l_pOtherRoom->IsObject3DSphereInRoom(l_pEmiter))
              {
                l_pOtherRoom->AddEmiter(l_pEmiter);
                break;
              }
            }
            if(l_it == l_end)
              _pPM->InsertEmiter(l_pEmiter);
          }
        }
        else
        {
          //Som a undefined
          set<string>::const_iterator l_it  = _pPM->GetRoomNames().cbegin();
          set<string>::const_iterator l_end = _pPM->GetRoomNames().cend();
          for(; l_it != l_end; ++l_it)
          {
            CRoom* l_pOtherRoom = _pPM->GetRoom(*l_it);
            if(l_pOtherRoom->IsObject3DSphereInRoom(l_pEmiter))
            {
              l_pOtherRoom->AddEmiter(l_pEmiter);
              l_Remove.insert(l_pEmiter);
              break;
            }
          }
        }
      }
    }
    {
      set<CEmiterInstance*>::iterator l_it  = l_Remove.begin();
      set<CEmiterInstance*>::iterator l_end = l_Remove.end();
      for(; l_it != l_end; ++l_it)
      {
        m_Emiters.erase(*l_it);
      }
    }
  }
}

void CRoom::AddRendeableObject(CRenderableObject* _pRO)
{
  m_RenderableObjects.insert(_pRO);
}

bool CRoom::RemoveRendeableObject(CRenderableObject* _pRO)
{
  set<CRenderableObject*>::iterator l_it = m_RenderableObjects.find(_pRO);
  if(l_it != m_RenderableObjects.end())
  {
    m_RenderableObjects.erase(l_it);
    return true;
  }
  else
  {
    return false;
  }
}

void CRoom::AddEmiter(CEmiterInstance* _pEmiter)
{
  _pEmiter->SetRoom(this);
  m_Emiters.insert(_pEmiter);
}

bool CRoom::RemoveEmiter(CEmiterInstance* _pEmiter)
{
  set<CEmiterInstance*>::iterator l_it = m_Emiters.find(_pEmiter);
  if(l_it != m_Emiters.end())
  {
    m_Emiters.erase(l_it);
    return true;
  }
  else
  {
    return false;
  }
}

bool CRoom::IsObject3DSphereInRoom(const CObject3D* _pObject3D) const
{
  vector<CObject3D>::const_iterator l_it  = m_Boundings.cbegin();
  vector<CObject3D>::const_iterator l_end = m_Boundings.cend();

  Mat44f l_matObject = _pObject3D->GetMat44();
  
  Vect3f l_vCenterObject = _pObject3D->GetBoundingSphere()->GetMiddlePoint();
  float  l_fRadius       = _pObject3D->GetBoundingSphere()->GetRadius();

  Vect4f l_aux = l_matObject * Vect4f(l_vCenterObject.x, l_vCenterObject.y, l_vCenterObject.z, 1);
  l_vCenterObject.x = l_aux.x / l_aux.w;
  l_vCenterObject.y = l_aux.y / l_aux.w;
  l_vCenterObject.z = l_aux.z / l_aux.w;

  for(; l_it != l_end; ++l_it)
  {
    Mat44f l_marRoom         = l_it->GetMat44();
    const CBoundingBox *l_BB = l_it->GetBoundingBox();
    bool l_bInsideTheBox = true;
    for(int i = 0; i < 3; ++i)
    {
      Vect3f l_vBasis;
      float l_fBoxR;
      //agafem el vector que toca segons la iteració.
      switch(i)
      { 
      case 0: 
        l_vBasis = Vect3f(1,0,0);
        l_fBoxR = l_BB->GetDimension().x;
        break;
      case 1: 
        l_vBasis = Vect3f(0,1,0);
        l_fBoxR = l_BB->GetDimension().y;
        break;
      case 2: 
        l_vBasis = Vect3f(0,0,1);
        l_fBoxR = l_BB->GetDimension().z;
        break;
      }
      //movem el vector segons la transformada
      l_aux = l_marRoom.GetInverted().Transpose() * Vect4f(l_vBasis.x,l_vBasis.y,l_vBasis.z,0);
      l_vBasis.x = l_aux.x;
      l_vBasis.y = l_aux.y;
      l_vBasis.z = l_aux.z;

      //calculem el "radi" de la box segon l'eix
      l_fBoxR = l_fBoxR * (l_vBasis * l_vBasis) *.5f;

      //minim i maxim de la box projectat
      Vect3f l_fMiddlePoint = l_BB->GetMiddlePoint();
      l_aux = l_marRoom * Vect4f(l_fMiddlePoint.x,l_fMiddlePoint.y,l_fMiddlePoint.z,1);
      l_fMiddlePoint.x = l_aux.x / l_aux.w;
      l_fMiddlePoint.y = l_aux.y / l_aux.w;
      l_fMiddlePoint.z = l_aux.z / l_aux.w;

      float l_fProjCenter = l_fMiddlePoint * l_vBasis;
      float l_fBoxMin = l_fProjCenter - l_fBoxR;
      float l_fBoxMax = l_fProjCenter + l_fBoxR;


      //centre de la esfera projectat
      float l_fSphereCenter = l_vCenterObject * l_vBasis;

      if(l_fSphereCenter + l_fRadius < l_fBoxMin ||
         l_fSphereCenter - l_fRadius > l_fBoxMax)
      {
        l_bInsideTheBox = false;
        break;
      }
    }
    if(l_bInsideTheBox)
    {
      return true;
    }
  }

  return false;
}



void CRoom::DebugRender(CRenderManager* _pRM) const
{
  
  vector<CObject3D>::const_iterator l_it  = m_Boundings.cbegin();
  vector<CObject3D>::const_iterator l_end = m_Boundings.cend();

  for(; l_it != l_end; ++l_it)
  {
    Mat44f scale;
    scale.SetIdentity();
    scale.SetScale(l_it->GetBoundingBox()->GetDimension().x, l_it->GetBoundingBox()->GetDimension().y, l_it->GetBoundingBox()->GetDimension().z);

    _pRM->SetTransform( l_it->GetMat44() * scale);

    _pRM->DrawCube(1, colBLACK);
  }
}