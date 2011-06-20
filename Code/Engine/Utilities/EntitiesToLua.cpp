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

#include "Core.h"
#include "EntityDefines.h"
#include "EntitiesToLua.h"
#include "EntityManager.h"

#include "ComponentObject3D.h"
#include "Component3rdPSCamera.h"
#include "ComponentPlayerController.h"
#include "ComponentPhysXController.h"
#include "ComponentPhysXBox.h"
#include "ComponentRenderableObject.h"
#include "ComponentMovement.h"
#include "ComponentAnimation.h"
#include "ComponentStateMachine.h"
#include "ComponentIABrain.h"
#include "ScriptedStateMachine.h"
#include "ComponentVida.h"
#include "ComponentRagdoll.h"
#include "ComponentMirilla.h"
#include "ComponentCover.h"
#include "ComponentHighCover.h"
#include "ComponentLowCover.h"


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
        value("int"   ,SEventInfo::INT),
        value("float" ,SEventInfo::FLOAT),
        value("vector",SEventInfo::VECTOR),
        value("string",SEventInfo::STRING)
      ]
      .def_readwrite("i",    &SEventInfo::i)
      .def_readwrite("f",    &SEventInfo::f)
      .def_readwrite("v",    &SEventInfo::v)
      .def_readwrite("str",  &SEventInfo::str)
      
    ,class_<SEvent>("Event")
      .enum_("EventType")
      [
        value("rebre_impacte",       SEvent::REBRE_IMPACTE),
        value("morir",               SEvent::MORIR)
      ]
      .def_readwrite("sender",       &SEvent::Sender)
      .def_readwrite("receiver",     &SEvent::Receiver)
      .def_readwrite("msg",          &SEvent::Msg)
      .def_readwrite("dispatch_time",&SEvent::DispatchTime)
      .property("Info", GetEventInfo, SetEventInfo, raw(_2)) 
      //.def_readwrite("Info",        &SEvent::Info)

    ,class_<CBaseComponent>("BaseComponent")
      .enum_("ComponentType")
      [
          value("object_3d",            CBaseComponent::ECT_OBJECT_3D),
          value("movement",             CBaseComponent::ECT_MOVEMENT),
          value("player_controller",    CBaseComponent::ECT_PLAYER_CONTROLLER),
          value("ia_walk_to_player",    CBaseComponent::ECT_IA_WALK_TO_PLAYER),
          value("physx_controller",     CBaseComponent::ECT_PHYSX_CONTROLLER),
          value("physx_controller",     CBaseComponent::ECT_PHYSX_ACTOR),
          value("renderable_object",    CBaseComponent::ECT_RENDERABLE_OBJECT),
          value("thps_camera",          CBaseComponent::ECT_3RD_PERSON_SHOOTER_CAMERA),
          value("trigger",              CBaseComponent::ECT_TRIGGER),
          value("vida",                 CBaseComponent::ECT_VIDA),
          value("laser",                CBaseComponent::ECT_LASER),
          value("state_machine",        CBaseComponent::ECT_STATE_MACHINE),
          value("animation",            CBaseComponent::ECT_ANIMATION),
          value("ia_brain",             CBaseComponent::ECT_IA_BRAIN),
          value("ragdoll",              CBaseComponent::ECT_RAGDOLL),
          value("cover",                CBaseComponent::ECT_COVER),
          value("mirilla",              CBaseComponent::ECT_MIRILLA)
      ]
      .def("get_type",     &CBaseComponent::GetType)
      .def("get_entity",   &CBaseComponent::GetEntity)
      .def("is_type",      &CBaseComponent::IsType)
      .def("receive_event",&CBaseComponent::ReceiveEvent)

    ,class_<CGameEntity>("GameEntity")
      .def("get_guid",         &CGameEntity::GetGUID)
      .def("get_component",    (CBaseComponent*(CGameEntity::*)(CBaseComponent::Type)const)&CGameEntity::GetComponent<CBaseComponent>)
      .def("get_name",         &CGameEntity::GetName)
      .def("delete_component", &CGameEntity::DeleteComponent)

    ,class_<CEntityManager>("EntityManager")
      .def("create_entity",         &CEntityManager::CreateEntity)
      .def("set_name_from_id",      (bool(CEntityManager::*)(const string&, int)               )&CEntityManager::SetName)
      .def("set_name",              (bool(CEntityManager::*)(const string&, const CGameEntity*))&CEntityManager::SetName)
      .def("get_entity",            (CGameEntity*(CEntityManager::*)(int)          const)&CEntityManager::GetEntity)
      .def("get_entity_from_name",  (CGameEntity*(CEntityManager::*)(const string&)const)&CEntityManager::GetEntity)
      .def("remove_entity_from_id", (void(CEntityManager::*)(int)               )&CEntityManager::RemoveEntity)
      .def("remove_entity",         (void(CEntityManager::*)(const CGameEntity*))&CEntityManager::RemoveEntity)

      .def("load_entities",         &CEntityManager::LoadEntitiesFromXML)
      .def("init_player",           &CEntityManager::InitPlayer)
      .def("init_enemy",            &CEntityManager::InitEnemy)
      .def("init_miner",            &CEntityManager::InitMiner)
      .def("init_militar",          &CEntityManager::InitMilitar)
  ];

  module(_pLS) [
    class_<CComponentObject3D, bases<CObject3D,CBaseComponent>>("ComponentObject3D")
      .def("init", &CComponentObject3D::Init)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPlayerController, CBaseComponent>("ComponentPlayerController")
      .def("init",        (bool(CComponentPlayerController::*)(CGameEntity*))&CComponentPlayerController::Init)
      .def("shoot",       &CComponentPlayerController::Shoot)
      .def("cover",       &CComponentPlayerController::Cover)
      .def("die",         &CComponentPlayerController::Die)
      .def("respawn",     &CComponentPlayerController::Respawn)
      .def_readwrite("pos_inicial", &CComponentPlayerController::m_vPosInicial)

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

      .def_readwrite("speed", &CComponentPlayerController::m_fSpeed)
      .def_readwrite("current_animation", &CComponentPlayerController::m_szCurrentAnimation)

      .def_readwrite("time", &CComponentPlayerController::m_fTime)

      .def_readwrite("cover_normal", &CComponentPlayerController::m_vCoverNormal)
      .def_readwrite("cover_position", &CComponentPlayerController::m_vCoverPosition)
      .def_readwrite("cover_entity",&CComponentPlayerController::m_pCoverEntity)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponent3rdPSCamera, CBaseComponent>("Component3rdPSCamera")
      .def("init", &CComponent3rdPSCamera::Init)
      .def_readwrite("camera_height", &CComponent3rdPSCamera::m_fCameraHeight)
      .def_readwrite("camera_right", &CComponent3rdPSCamera::m_fCameraRight)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXController,CBaseComponent>("ComponentPhysXController")
      .def("init", &CComponentPhysXController::Init)
      .def("set_height", &CComponentPhysXController::SetHeight)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXBox, CBaseComponent>("ComponentPhysXBox")
      .def("init",     (bool(CComponentPhysXBox::*)(CGameEntity*,float,float,float,float,float,float,float,int))&CComponentPhysXBox::Init)
      .def("init",     (bool(CComponentPhysXBox::*)(CGameEntity*,float,int))&CComponentPhysXBox::Init)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRenderableObject, CBaseComponent>("ComponentRenderableObject")
      .def("init",                               &CComponentRenderableObject::Init)
      .def("init_animated_model",                &CComponentRenderableObject::InitAnimatedModel)
      .def_readwrite("remove_renderable_object", &CComponentRenderableObject::m_bRemoveRenderableObject)
      .def_readwrite("active",                   &CComponentRenderableObject::m_bActive)
      .def_readwrite("block_yaw",                &CComponentRenderableObject::m_bBlockYaw)
      .def("set_yaw",                            &CComponentRenderableObject::SetYaw)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentMovement, CBaseComponent>("ComponentMovement")
      .def("init",                               &CComponentMovement::Init)
      .def_readwrite("movement",                 &CComponentMovement::m_vMovement)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentAnimation, CBaseComponent>("ComponentAnimation")
      .def("init",                     &CComponentAnimation::Init)
      .def("clear_cycle",              (void(CComponentAnimation::*)(float))&CComponentAnimation::ClearCycle)
      .def("clear_cycle",              (void(CComponentAnimation::*)(const string&, float))&CComponentAnimation::ClearCycle)
      .def("set_cycle",                &CComponentAnimation::SetCycle)
      .def("set_animation",            &CComponentAnimation::SetAnimation)
      .def("get_current_cycle",        &CComponentAnimation::GetCurrentCycle)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentStateMachine, CBaseComponent>("ComponentStateMachine")
      .def("init",                     &CComponentStateMachine::Init)
      .def("get_state_machine",        &CComponentStateMachine::GetStateMachine)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentIABrain, CBaseComponent>("ComponentIABrain")
      .def("init",                     &CComponentIABrain::Init)
      .def("shoot",                    &CComponentIABrain::Shoot)
      .def("die",                      &CComponentIABrain::Die)
      .def_readonly("player",          &CComponentIABrain::m_pPlayer)
      .def_readwrite("time",           &CComponentIABrain::m_fTime)
      .def_readwrite("shooted",        &CComponentIABrain::m_bShooted)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentVida, CBaseComponent>("ComponentIABrain")
      .def("init",                     &CComponentVida::Init)
      .def_readwrite("vida",           &CComponentVida::m_fVida)
      .def_readwrite("immortal",       &CComponentVida::m_bImmortal)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRagdoll, CBaseComponent>("ComponentRagdoll")
      .def("init",                     &CComponentRagdoll::Init)
      .def("set_active",               &CComponentRagdoll::SetActive)

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentMirilla, CBaseComponent>("ComponentMirilla")
      .def("set_active",               &CComponentMirilla::SetActive)

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentCover, CBaseComponent>("ComponentCover")
      .def("get_cover_type",               &CComponentCover::GetCoverType)
      .enum_("CoverType")
      [
          value("cover_low",            CComponentCover::COVER_LOW),
          value("cover_high",             CComponentCover::COVER_HIGH)
      ]
    ,class_<CComponentHighCover, CBaseComponent>("ComponentHighCover")
      .def("get_cover_type",               &CComponentCover::GetCoverType)
    ,class_<CComponentLowCover, CBaseComponent>("ComponentLowCover")
      .def("get_cover_type",               &CComponentCover::GetCoverType)
  ];
  


  module(_pLS) [
   
    class_<CScriptedStateMachine>("ScriptedStateMachine")
      .def("set_current_state",    &CScriptedStateMachine::SetCurrentState)
      .def("change_state",         &CScriptedStateMachine::ChangeState)
      .def("current_state",        &CScriptedStateMachine::CurrentState)
      .def("revert_state",         &CScriptedStateMachine::RevertState)
      .def("receive_event",        &CScriptedStateMachine::ReceiveEvent)
  ];
}


void RegisterCore(class_<CCore>& _Core)
{
  _Core.def("get_entity_manager",              &CCore::GetEntityManager);
}