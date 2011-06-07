#define __DONT_INCLUDE_MEM_LEAKS__
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "EntityDefines.h"
#include "EntitiesToLua.h"
#include "EntityManager.h"

#include "ComponentObject3D.h"
#include "Component3rdPSCamera.h"
#include "ComponentPlayerController.h"
#include "ComponentPhysXController.h"
#include "ComponentPhysXBox.h"
#include "ComponentRenderableObject.h"


#include "Utils/MemLeaks.h"

using namespace luabind;

// Entity System functions ----------------------------

luabind::object GetEventInfo(SEvent const& x, lua_State* L)
{
    luabind::object result = luabind::newtable(L);

    for (int i = 0; i < EVENT_INFO_SIZE; ++i)
        result[i + 1] = x.Info[i];

    return result;
}

void SetEventInfo(SEvent& x, luabind::argument const& table)
{
    for (int i = 0; i < EVENT_INFO_SIZE; ++i)
        x.Info[i] = luabind::object_cast<SEventInfo>(table[i + 1]);
} 

// ----------------------------------------------------

void RegisterEntitiesToLua(lua_State* _pLS)
{
  module(_pLS) [
    class_<SEventInfo>("EventInfo")
      .enum_("Type")
      [
        value("sti_int"   ,SEventInfo::STI_INT),
        value("sti_float" ,SEventInfo::STI_FLOAT),
        value("sti_vector",SEventInfo::STI_VECTOR),
        value("sti_string",SEventInfo::STI_STRING)
      ]
      .def_readwrite("i",    &SEventInfo::i)
      .def_readwrite("f",    &SEventInfo::f)
      .def_readwrite("v",    &SEventInfo::v)
      .def_readwrite("str",  &SEventInfo::str)
      
    ,class_<SEvent>("Event")
      //.enum_("EventType")
      //[
      //]
      .def_readwrite("Sender",      &SEvent::Sender)
      .def_readwrite("Receiver",    &SEvent::Receiver)
      .def_readwrite("Msg",         &SEvent::Msg)
      .def_readwrite("DispatchTime",&SEvent::DispatchTime)
      .property("Info", GetEventInfo, SetEventInfo, raw(_2)) 
      //.def_readwrite("Info",        &SEvent::Info)
      
    

    ,class_<CBaseComponent>("BaseComponent")
      .enum_("ComponentType")
      [
          value("object_3d",         CBaseComponent::ECT_OBJECT_3D),
          value("phisX_controller",  CBaseComponent::ECT_PHYSX_CONTROLLER),
          value("player_controller", CBaseComponent::ECT_PLAYER_CONTROLLER),
          value("thps_camera",       CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA)
      ]
      .def("get_type",     &CBaseComponent::GetType)
      .def("get_entity",   &CBaseComponent::GetEntity)
      .def("is_type",      &CBaseComponent::IsType)
      .def("receive_event",&CBaseComponent::ReceiveEvent)

    ,class_<CGameEntity>("GameEntity")
      .def("get_guid",      &CGameEntity::GetGUID)
      .def("get_component", (CBaseComponent*(CGameEntity::*)(CBaseComponent::Type)const)&CGameEntity::GetComponent<CBaseComponent>)
      .def("get_name",      &CGameEntity::GetName)

    ,class_<CEntityManager>("EntityManager")
      .def("create_entity",         &CEntityManager::CreateEntity)
      .def("set_name_from_id",      (void(CEntityManager::*)(const string&, int)               )&CEntityManager::SetName)
      .def("set_name",              (void(CEntityManager::*)(const string&, const CGameEntity*))&CEntityManager::SetName)
      .def("get_entity",            (CGameEntity*(CEntityManager::*)(int)          const)&CEntityManager::GetEntity)
      .def("get_entity_from_name",  (CGameEntity*(CEntityManager::*)(const string&)const)&CEntityManager::GetEntity)
      .def("remove_entity_from_id", (void(CEntityManager::*)(int)               )&CEntityManager::RemoveEntity)
      .def("remove_entity",         (void(CEntityManager::*)(const CGameEntity*))&CEntityManager::RemoveEntity)
  ];

  module(_pLS) [
    class_<CComponentObject3D, bases<CObject3D,CBaseComponent>>("ComponentObject3D")
      .def("init", &CComponentObject3D::Init)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPlayerController, CBaseComponent>("ComponentPlayerController")
      .def("init", (bool(CComponentPlayerController::*)(CGameEntity*))&CComponentPlayerController::Init)

      .def_readwrite("move_fwd",   &CComponentPlayerController::m_szMoveForward)
      .def_readwrite("move_back",  &CComponentPlayerController::m_szMoveBack)
      .def_readwrite("move_left",  &CComponentPlayerController::m_szMoveLeft)
      .def_readwrite("move_right", &CComponentPlayerController::m_szMoveRight)
      
      .def_readwrite("run",  &CComponentPlayerController::m_szRun)
      .def_readwrite("walk", &CComponentPlayerController::m_szWalk)
      
      .def_readwrite("walk_speed", &CComponentPlayerController::m_fWalkSpeed)
      .def_readwrite("run_speed",  &CComponentPlayerController::m_fRunSpeed)
      .def_readwrite("yaw_speed",  &CComponentPlayerController::m_fYawSpeed)
      .def_readwrite("pitch_speed", &CComponentPlayerController::m_fPitchSpeed)

      .def_readwrite("min_pitch_angle", &CComponentPlayerController::m_fMaxPitchAngle)
      .def_readwrite("max_pitch_angle", &CComponentPlayerController::m_fMinPitchAngle)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponent3rdPSCamera, CBaseComponent>("Component3rdPSCamera")
      .def("init", &CComponent3rdPSCamera::Init)
      .def_readwrite("camera_height", &CComponent3rdPSCamera::m_fCameraHeight)
      .def_readwrite("camera_right", &CComponent3rdPSCamera::m_fCameraRight)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXController,CBaseComponent>("ComponentPhysXController")
      .def("init", &CComponentPhysXController::Init)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXBox, CBaseComponent>("ComponentPhysXBox")
      .def("init",                        (bool(CComponentPhysXBox::*)(CGameEntity*,float,float,float,float,float,float,float,int))&CComponentPhysXBox::Init)
      .def("init_with_renderable_object", (bool(CComponentPhysXBox::*)(CGameEntity*,float,int))&CComponentPhysXBox::Init)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRenderableObject, CBaseComponent>("ComponentRenderableObject")
      .def("init",                               &CComponentRenderableObject::Init)
      .def("init_animated_model",                &CComponentRenderableObject::InitAnimatedModel)
      .def_readwrite("remove_renderable_object", &CComponentRenderableObject::m_bRemoveRenderableObject)
  
  ];
}
