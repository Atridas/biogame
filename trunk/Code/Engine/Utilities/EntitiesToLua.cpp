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
#include "ComponentDoor.h"
#include "ComponentShield.h"
#include "ComponentNavNode.h"
#include "ComponentEmiter.h"
#include "ComponentSpawner.h"
#include "ComponentExplosive.h"
#include "ComponentEmiter.h"
#include "ComponentBillboard.h"
#include "ComponentRotative.h"
#include "ComponentOmni.h"
#include "ComponentArma.h"
#include "ComponentLifetime.h"
#include "OmniLight.h"
#include "ComponentPhysxSphere.h"


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

void SetGodMode(bool _bGodMode)
{
  CGameEntity* l_pPlayer = ENTITY_MANAGER->GetEntity("Player");
  if(l_pPlayer)
    l_pPlayer->GetComponent<CComponentPlayerController>()->SetGodMode(_bGodMode);
}

// ----------------------------------------------------

void RegisterEntitiesToLua(lua_State* _pLS)
{
  module(_pLS) [
    //globals
    def("god_mode", &SetGodMode)

    //Entities
    ,class_<SEventInfo>("EventInfo")
      .enum_("TYPE")
      [
        value("int"   ,SEventInfo::INT),
        value("float" ,SEventInfo::FLOAT),
        value("vector",SEventInfo::VECTOR),
        value("string",SEventInfo::STRING),
        value("pointer",SEventInfo::PTR)
      ]
      .def_readwrite("Type", &SEventInfo::Type)
      .def_readwrite("i",    &SEventInfo::i)
      .def_readwrite("f",    &SEventInfo::f)
      .def_readwrite("v",    &SEventInfo::v)
      .def_readwrite("str",  &SEventInfo::str)
      .def_readwrite("ptr",  &SEventInfo::ptr)
      .def("set_str",        &SEventInfo::SetStr)
      
    ,class_<SEvent>("Event")
      .enum_("EventType")
      [
        value("rebre_impacte",       SEvent::REBRE_IMPACTE),
        value("rebre_force",         SEvent::REBRE_FORCE),
        value("morir",               SEvent::MORIR),
        value("obrir",               SEvent::OBRIR),
        value("tancar",              SEvent::TANCAR),
        value("pickup",              SEvent::PICKUP),
        value("interaccio",          SEvent::INTERACCIO)
      ]
      .def_readwrite("sender",       &SEvent::Sender)
      .def_readwrite("receiver",     &SEvent::Receiver)
      .def_readwrite("msg",          &SEvent::Msg)
      .def_readwrite("dispatch_time",&SEvent::DispatchTime)
      .def("get_info",               &SEvent::GetInfo)
      .def("set_info",               &SEvent::SetInfo)
      //.property("Info", &GetEventInfo, &SetEventInfo, raw(_2))

    ,class_<CBaseComponent>("BaseComponent")
      .enum_("ComponentType")
      [
          value("object_3d",            CBaseComponent::ECT_OBJECT_3D),
          value("movement",             CBaseComponent::ECT_MOVEMENT),
          value("rotative",             CBaseComponent::ECT_ROTATIVE),
          value("player_controller",    CBaseComponent::ECT_PLAYER_CONTROLLER),
          value("ia_walk_to_player",    CBaseComponent::ECT_IA_WALK_TO_PLAYER),
          value("physx_controller",     CBaseComponent::ECT_PHYSX_CONTROLLER),
          value("physx_actor",          CBaseComponent::ECT_PHYSX_ACTOR),
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
          value("mirilla",              CBaseComponent::ECT_MIRILLA),
          value("door",                 CBaseComponent::ECT_DOOR),
          value("shield",               CBaseComponent::ECT_SHIELD),
          value("interactive",          CBaseComponent::ECT_INTERACTIVE),
          value("destroyable",          CBaseComponent::ECT_DESTROYABLE),
          value("nav_node",             CBaseComponent::ECT_NAV_NODE),
          value("emiter",               CBaseComponent::ECT_EMITER),
          value("spawner",              CBaseComponent::ECT_SPAWNER),
          value("explosive",            CBaseComponent::ECT_EXPLOSIVE),
          value("billboard",            CBaseComponent::ECT_BILLBOARD),
          value("omni",                 CBaseComponent::ECT_OMNI),
          value("life_time",            CBaseComponent::ECT_LIFETIME),
          value("physx_sphere",         CBaseComponent::ECT_PHYSXSPHERE)
      ]
      .def("get_type",     &CBaseComponent::GetType)
      .def("get_entity",   &CBaseComponent::GetEntity)
      .def("is_type",      &CBaseComponent::IsType)
      .def("set_active",   &CBaseComponent::SetActive)
      .def("is_active",    &CBaseComponent::IsActive)
      .def("receive_event",&CBaseComponent::ReceiveEvent)

    ,class_<CGameEntity>("GameEntity")
      .def("get_guid",         &CGameEntity::GetGUID)
      .def("get_component",    (CBaseComponent*(CGameEntity::*)(CBaseComponent::Type)const)&CGameEntity::GetComponent<CBaseComponent>)
      .def("get_name",         &CGameEntity::GetName)
      .def("delete_component", &CGameEntity::DeleteComponent)
      .def("set_active",       &CGameEntity::SetActive)

    ,class_<CEntityManager>("EntityManager")
      .def("create_entity",                     &CEntityManager::CreateEntity)
      .def("set_name_from_id",                  (bool(CEntityManager::*)(const string&, int)               )&CEntityManager::SetName)
      .def("set_name",                          (bool(CEntityManager::*)(const string&, const CGameEntity*))&CEntityManager::SetName)
      .def("get_entity",                        (CGameEntity*(CEntityManager::*)(int)          const)&CEntityManager::GetEntity)
      .def("get_entity",                        (CGameEntity*(CEntityManager::*)(const string&)const)&CEntityManager::GetEntity)
      .def("get_entity_from_name",              (CGameEntity*(CEntityManager::*)(const string&)const)&CEntityManager::GetEntity)
      .def("remove_entity_from_id",             (void(CEntityManager::*)(int)               )&CEntityManager::RemoveEntity)
      .def("remove_entity",                     (void(CEntityManager::*)(int)               )&CEntityManager::RemoveEntity)
      .def("remove_entity",                     (void(CEntityManager::*)(const CGameEntity*))&CEntityManager::RemoveEntity)
      .def("send_event",                        &CEntityManager::SendEvent)
      .def("get_event",                         &CEntityManager::GetEvent)
                                                
      .def("load_entities",                     &CEntityManager::LoadEntitiesFromXML)
      .def("init_player",                       &CEntityManager::InitPlayer)
      .def("init_enemy",                        &CEntityManager::InitEnemy)
      .def("init_miner",                        &CEntityManager::InitMiner)
      .def("init_militar",                      &CEntityManager::InitMilitar)
      .def("init_emiter",                       &CEntityManager::InitParticles)
      .def("init_trigger_with_emiter",          &CEntityManager::InitTriggerWithParticles)
      .def("init_pickup",                       &CEntityManager::InitPickUp)
  ];

  module(_pLS) [
    class_<CComponentObject3D, bases<CObject3D,CBaseComponent>>("ComponentObject3D")
      .scope[def("add_to_entity",  &CComponentObject3D::AddToEntity)]
      .def("get_position",         &CComponentObject3D::GetPosition)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPlayerController, CBaseComponent>("ComponentPlayerController")
      .scope[def("add_to_entity",    (bool(*)(CGameEntity*))&CComponentPlayerController::AddToEntity)]
      .def("shoot",                  &CComponentPlayerController::Shoot)
      .def("shoot_grenade",         &CComponentPlayerController::ShootGrenade)
      .def("force",                  &CComponentPlayerController::Force)
      .def("cover",                  &CComponentPlayerController::Cover)
      .def("use",                    &CComponentPlayerController::Use)
      //.def("die",                    &CComponentPlayerController::Die)
      .def("respawn",                &CComponentPlayerController::Respawn)
      .def("has_pickup",             &CComponentPlayerController::HasPickUp)
      .def("add_pickup",             &CComponentPlayerController::AddPickUp)
      .def("remove_pickup",          &CComponentPlayerController::RemovePickUp)
      .def_readwrite("pos_inicial",  &CComponentPlayerController::m_vPosInicial)
      .def_readwrite("shoot_active", &CComponentPlayerController::m_bShootActive)
      .def_readwrite("force_active", &CComponentPlayerController::m_bForceActive)

      /*.def_readwrite("move_fwd",   &CComponentPlayerController::m_szMoveForward)
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
      .def_readwrite("max_pitch_angle", &CComponentPlayerController::m_fMinPitchAngle)*/

      .def_readwrite("speed", &CComponentPlayerController::m_fSpeed)
      .def_readwrite("current_animation", &CComponentPlayerController::m_szCurrentAnimation)

      .def_readwrite("time", &CComponentPlayerController::m_fTime)

      .def_readwrite("cover_normal", &CComponentPlayerController::m_vCoverNormal)
      .def_readwrite("cover_position", &CComponentPlayerController::m_vCoverPosition)
      .def_readwrite("cover_entity",&CComponentPlayerController::m_pCoverEntity)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponent3rdPSCamera, CBaseComponent>("Component3rdPSCamera")
      .scope[def("add_to_entity",     &CComponent3rdPSCamera::AddToEntity)]
      .def_readwrite("camera_height", &CComponent3rdPSCamera::m_fCameraHeight)
      .def_readwrite("camera_right",  &CComponent3rdPSCamera::m_fCameraRight)
      .def("set_zoom", &CComponent3rdPSCamera::SetZoom)
      .def("set_fov", &CComponent3rdPSCamera::SetFOV)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXController,CBaseComponent>("ComponentPhysXController")
      .scope[def("add_to_entity",      &CComponentPhysXController::AddToEntity)]
      .def("set_height",               &CComponentPhysXController::SetHeight)
      .def("set_position_from_ragdoll",&CComponentPhysXController::SetPositionFromRagdoll)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXBox, CBaseComponent>("ComponentPhysXBox")
      .scope[def("add_to_entity", (CComponentPhysXBox*(*)(CGameEntity*,float,float,float,float,float,float,float,int))&CComponentPhysXBox::AddToEntity)]
      .scope[def("add_to_entity", (CComponentPhysXBox*(*)(CGameEntity*,float,int))                                    &CComponentPhysXBox::AddToEntity)]
      .def("set_position",        &CComponentPhysXBox::SetPosition)
      .def("get_position",        &CComponentPhysXBox::GetPosition)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRenderableObject, CBaseComponent>("ComponentRenderableObject")
      .scope[def("add_to_entity",                &CComponentRenderableObject::AddToEntity)]
      .def("add_to_entity_with_animated_model",  &CComponentRenderableObject::AddToEntityWithAnimatedModel)
      .def_readwrite("remove_renderable_object", &CComponentRenderableObject::m_bRemoveRenderableObject)
      .def_readwrite("block_yaw",                &CComponentRenderableObject::m_bBlockYaw)
      .def("set_yaw",                            &CComponentRenderableObject::SetYaw)
      .def("set_active",                         &CComponentRenderableObject::SetActive)

      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentMovement, CBaseComponent>("ComponentMovement")
      .scope[def("add_to_entity",            &CComponentMovement::AddToEntity)]
      .def_readwrite("movement",             &CComponentMovement::m_vMovement)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentAnimation, CBaseComponent>("ComponentAnimation")
      .scope[def("add_to_entity",      &CComponentAnimation::AddToEntity)]
      .def("play",                     &CComponentAnimation::Play)
      .def("stop",                     &CComponentAnimation::Stop)
      //.def("play_cycle",               &CComponentAnimation::PlayCycle)
      .def("play_cycle",              (void(CComponentAnimation::*)(const string&, float))&CComponentAnimation::PlayCycle)
      .def("play_cycle",              (void(CComponentAnimation::*)(const string&, float, float))&CComponentAnimation::PlayCycle)
      .def("stop_cycle",               &CComponentAnimation::StopCycle)
      .def("clear_cycle",              (void(CComponentAnimation::*)(float))&CComponentAnimation::ClearCycle)
      .def("clear_cycle",              (void(CComponentAnimation::*)(const string&, float))&CComponentAnimation::ClearCycle)
      .def("clear_all_cycles",         &CComponentAnimation::ClearAllCycles)


    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentStateMachine, CBaseComponent>("ComponentStateMachine")
      .scope[def("add_to_entity",      &CComponentStateMachine::AddToEntity)]
      .def("get_state_machine",        &CComponentStateMachine::GetStateMachine)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentIABrain, CBaseComponent>("ComponentIABrain")
      .scope[def("add_to_entity",      &CComponentIABrain::AddToEntity)]
      .def("shoot",                    &CComponentIABrain::Shoot)
      //.def("die",                      &CComponentIABrain::Die)
      .def("plan_path_to_cobertura",   (bool(CComponentIABrain::*)(void))&CComponentIABrain::PlanPathToCobertura)
      .def("plan_path_to_cobertura",   (bool(CComponentIABrain::*)(int))&CComponentIABrain::PlanPathToCobertura)
      .def("get_next_node_position",   &CComponentIABrain::GetNextNodePosition)
      .def("arrived_at_destination",   &CComponentIABrain::ArrivedAtDestination) 
      .def("arrived_at_node",          &CComponentIABrain::ArrivedAtNode)
      .def("set_next_node",            &CComponentIABrain::SetNextNode) 
      .def_readonly("player",          &CComponentIABrain::m_pPlayer)
      .def_readwrite("time",           &CComponentIABrain::m_fTime)
      .def_readwrite("shooted",        &CComponentIABrain::m_bShooted)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentVida, CBaseComponent>("ComponentVida")
      .scope[def("add_to_entity",      &CComponentVida::AddToEntity)]
      .def("get_hp",                   &CComponentVida::GetHP)
      .def("increase",                 &CComponentVida::Increase)
      .def("decrease",                 &CComponentVida::Decrease)
      .def("set",                      &CComponentVida::Set)
      .def("set_regen",                &CComponentVida::SetRegen)
      .def("add_dot",                  &CComponentVida::AddDoT)
      .def("add_hot",                  &CComponentVida::AddHoT)

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentShield, CBaseComponent>("ComponentShield")
      .scope[def("add_to_entity",      &CComponentShield::AddToEntity)]
      .def("get_hp",                   &CComponentShield::GetHP)
      .def("increase",                 &CComponentShield::Increase)
      .def("decrease",                 &CComponentShield::Decrease)
      .def("set",                      &CComponentShield::Set)
      .def("set_regen",                &CComponentShield::SetRegen)
      //.def("activate",                 &CComponentShield::Activate)
      //.def("deactivate",               &CComponentShield::Deactivate)
      //.def("is_active",                &CComponentShield::IsActive)
      .def("is_ready",                 &CComponentShield::IsReady)
      
    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRagdoll, CBaseComponent>("ComponentRagdoll")
      .scope[def("add_to_entity",      &CComponentRagdoll::AddToEntity)]
      .def("apply_physics",            &CComponentRagdoll::ApplyPhysics)
      .def("get_position",             &CComponentRagdoll::GetPosition)

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentMirilla, CBaseComponent>("ComponentMirilla")

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentCover, CBaseComponent>("ComponentCover")
      .def("get_cover_type",           &CComponentCover::GetCoverType)
      .enum_("CoverType")
      [
          value("cover_low",           CComponentCover::COVER_LOW),
          value("cover_high",          CComponentCover::COVER_HIGH)
      ]

    ,class_<CComponentHighCover, CComponentCover>("ComponentHighCover")
      .scope[def("add_to_entity",      &CComponentHighCover::AddToEntity)]
      .def("get_cover_type",           &CComponentHighCover::GetCoverType)

    ,class_<CComponentLowCover, CComponentCover>("ComponentLowCover")
      .scope[def("add_to_entity",      &CComponentLowCover::AddToEntity)]
      .def("get_cover_type",           &CComponentLowCover::GetCoverType)
    // ----------------------------------------------------------------------------------------------------

    ,class_<CComponentDoor, CBaseComponent>("ComponentDoor")
      .scope[def("add_to_entity",      &CComponentDoor::AddToEntity)]
      .def("block",                    &CComponentDoor::Block)
      .def("is_blocked",               &CComponentDoor::IsBlocked)
      .def("get_open_time",            &CComponentDoor::GetOpenTime)
      .def("get_close_time",           &CComponentDoor::GetCloseTime)
      .def_readwrite("time",           &CComponentDoor::m_fTime)
    // ----------------------------------------------------------------------------------------------------

    ,class_<CComponentNavNode, CBaseComponent>("ComponentNavNode")
      .scope[def("add_to_entity",      &CComponentNavNode::AddToEntity)]
      .def("get_graph_node_index",     &CComponentNavNode::GetGraphNodeIndex)

    // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentSpawner, CBaseComponent>("ComponentSpawner")
      .enum_("EEnemyType")
      [
        value("enemy_militar",         CComponentSpawner::EE_MILITAR),
        value("enemy_miner",           CComponentSpawner::EE_MINER)
      ]
      .scope[def("add_to_entity",      &CComponentSpawner::AddToEntity)]
      .def("set_enemy_type",           &CComponentSpawner::SetEnemyType)
      .def("get_enemy_type",           &CComponentSpawner::GetEnemyType)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentExplosive, CBaseComponent>("ComponentExplosive")
      .scope[def("add_to_entity",      &CComponentExplosive::AddToEntity)]
      .def("explode",                  &CComponentExplosive::Explode)
      .def("set_active",               &CComponentExplosive::SetActive)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentEmiter, CBaseComponent>("ComponentEmiter")
      .scope[def("add_to_entity",  &CComponentEmiter::AddToEntity)]
      .def("change_core",          &CComponentEmiter::ChangeCore)
      .def("change_volume",        &CComponentEmiter::ChangeVolume)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentBillboard, CBaseComponent>("ComponentBillboard")
      .scope[def("add_to_entity",  &CComponentBillboard::AddToEntity)]
      .def("change_core",          &CComponentBillboard::ChangeCore)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentRotative, CBaseComponent>("ComponentRotative")
      .scope[def("add_to_entity",      &CComponentRotative::AddToEntity)]
      .def_readwrite("yaw_rotation",   &CComponentRotative::m_fYawRotation)
      .def_readwrite("pitch_rotation", &CComponentRotative::m_fPitchRotation)
      .def_readwrite("roll_rotation",  &CComponentRotative::m_fRollRotation)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentOmni, CBaseComponent>("ComponentOmni")
      .scope[def("add_to_entity",            &CComponentOmni::AddToEntity)]
      .def("get_omni_light",                 &CComponentOmni::GetOmniLight)
      .def("set_active",                     &CComponentOmni::SetActive)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentArma, CBaseComponent>("ComponentArma")
      .scope[def("add_to_entity",            &CComponentArma::AddToEntity)]

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentLifetime, CBaseComponent>("ComponentLifetime")
      .def_readwrite("time", &CComponentLifetime::m_fTime)
      .def_readwrite("target_time", &CComponentLifetime::m_fTargetTime)
      .def_readwrite("kill_entity", &CComponentLifetime::m_bKillEntity)

      // ----------------------------------------------------------------------------------------------------
    ,class_<CComponentPhysXSphere, CBaseComponent>("ComponentPhysXSphere")
      .def("set_active",        &CComponentPhysXSphere::SetActive)

  ];
}


void RegisterCore_Entities(class_<CCore>& _Core)
{
  _Core.def("get_entity_manager",              &CCore::GetEntityManager);
}