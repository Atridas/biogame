#include <algorithm>
#include <sstream>
#include "Viewer.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "ShoulderCamera.h"
#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"
#include "InstanceMesh.h"
#include "StaticMesh.h"
#include "AnimatedCoreModel.h"
#include <LightManager.h>
#include "SpotLight.h"
#include "OmniLight.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "ActionManager.h"
#include "Material.h"
#include "Renderer.h"
#include "Utils/MemLeaks.h"


bool SortRenderableObjectByName(CRenderableObject* _pRO1, CRenderableObject* _pRO2)
{
  string l_szName1 = _pRO1->GetName();
  string l_szName2 = _pRO2->GetName();

  transform(l_szName1.begin(), l_szName1.end(), l_szName1.begin(), tolower);
  transform(l_szName2.begin(), l_szName2.end(), l_szName2.begin(), tolower);

  return (l_szName1.compare(l_szName2) < 0);
}

CViewer::CViewer(void)
{
  //--------------------
}

void CViewer::Init() 
{
  m_vMeshes = CORE->GetRenderableObjectsManager()->GetMeshes();
  m_vAnimatedModels = CORE->GetRenderableObjectsManager()->GetAnimatedModels();

  sort(m_vMeshes.begin(),m_vMeshes.end(),SortRenderableObjectByName);
  sort(m_vAnimatedModels.begin(),m_vAnimatedModels.end(),SortRenderableObjectByName);

  m_itCurrentMesh = m_vMeshes.begin();
  m_itCurrentAnimated = m_vAnimatedModels.begin();

  m_pCharacter = 0;
  m_fInitialCharacterYaw = 0.0f;

  if(m_vAnimatedModels.size() > 0)
  {
    m_pCharacter = (CRenderableAnimatedInstanceModel*)m_vAnimatedModels[0];
    m_fInitialCharacterYaw = m_pCharacter->GetYaw();
	  m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0);
	  m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(0,0);
  }

  m_pTargetObject = new CObject3D();
  m_pTargetObject->SetPosition(Vect3f(0.0f,0.0f,0.0f));

  m_pObjectCamera = new CShoulderCamera(
      0.1f,
      100.0f,
      55.0f * FLOAT_PI_VALUE/180.0f,
      ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
      m_pTargetObject,
      2.0f,0.6f,1.5f);

  m_pObjectModeLight = 0;
  //m_pSpotLight = 0;

  m_pObjectModeLight = CORE->GetLightManager()->CreateDirectionalLight("ObjectModeLight",
                                                                        Vect3f(0.0f),
                                                                        Vect3f(1.0f,1.0f,1.0f),
                                                                        CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                                        50.0f,
                                                                        80.0f,
                                                                        false);

  //m_pObjectModeLight->SetDynamicObjectsOnly(true);

  //m_pSpotLight = CORE->GetLightManager()->CreateSpotLight("FreeModeLight",
  //                                                        Vect3f(-2.15715f,0.0f,-7.32758f),
  //                                                        Vect3f(-5.4188f,0.0f,3.75613f),
  //                                                        CColor(Vect3f(1.0f,1.0f,1.0f)),
  //                                                        20.0f,
  //                                                        80.0f,
  //                                                        10.0f,
  //                                                        45.0f,
  //                                                        false );

  //m_vOmniColor = Vect3f(1.0f,1.0f,1.0f);
  //m_pOmniLight = CORE->GetLightManager()->CreateOmniLight("OmniViewerLight",Vect3f(0.0f),CColor(m_vOmniColor),0.1f,17.0f);

  //CSceneEffectManager* l_pSceneEffectManager = CORE->GetSceneEffectManager();
  //if(l_pSceneEffectManager)
  //{
  //  l_pSceneEffectManager->SetShadowMapLightCast(m_pSpotLight);
  //}

  //m_vAmbientLight = CORE->GetLightManager()->GetAmbientLight();

  m_bEnableLights = true;
  //m_bGuiActive = false;
  m_bShowInfo = true;
  //m_eNormalRendering = NO_NORMALS;
  m_bShowBoxes = false;
  m_bShowSpheres = false;

  //materials
  //m_eLightmapMode  = CMaterial::RADIOSITY_NORMAL;
  m_fGlowIntensity = 0.0f;
  m_fGlowIntensity = 0.0f;
  m_fGlossiness = 0.0f;

  m_iMode = FREE_MODE;
  m_eCurrentMaterialProperty = CViewer::SPECULAR;

  //SOUND_MANAGER->PlayMusic("bgm",true);

  m_vMouseDelta = 0;

  ResetActions();

  InitMode();
}



CViewer::~CViewer(void)
{
  Release();
}

void CViewer::Release()
{
  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pTargetObject)
}

void CViewer::SetMode(EModes _eMode)
{
  int l_iPreviousMode = m_iMode;
  m_iMode = _eMode;

  if(l_iPreviousMode != m_iMode)
    InitMode();
}

void CViewer::InitMode()
{
  m_bShowBoxes = false;
  m_bShowSpheres = false;

  switch(m_iMode) {
  case FREE_MODE:
    InitFreeMode();
    break;
  case MESH_MODE:
    InitMeshMode();
    break;
  case ANIMATED_MODE:
    InitAnimatedMode();
    break;
  default:
    break;
  }
}

void CViewer::InitFreeMode()
{
  CORE->GetRenderer()->SetUniqueRenderPath("HDR");

  m_fVelocity = 5.0f;

  CORE->GetRenderableObjectsManager()->SetAllVisibility(true);

  if(m_pCharacter)
  {
    m_pTargetObject->SetYaw(m_pCharacter->GetYaw()-m_fInitialCharacterYaw);
    m_pTargetObject->SetPosition(m_pCharacter->GetPosition());
    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderDistance(0.85f);
    ((CShoulderCamera*)m_pObjectCamera)->SetZoom(1.8f);
  }else{
    m_pTargetObject->SetYaw(0.0f);
    m_pTargetObject->SetPosition(Vect3f(m_pTargetObject->GetPosition().x,0.0f,m_pTargetObject->GetPosition().z));
    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderDistance(0.0f);
    ((CShoulderCamera*)m_pObjectCamera)->SetZoom(0.0f);
  }

  m_pTargetObject->SetPitch(0.0f);

  ((CShoulderCamera*)m_pObjectCamera)->SetShoulderHeight(1.55f);

  if(m_pObjectModeLight)
  {
    m_pObjectModeLight->SetActive(false);
  }

  UpdatePosition(Vect3f(0.0f),0.0f,0.0f);
  UpdateCamera(0.0f,0.0f);
}

void CViewer::InitMeshMode()
{
  if(m_vMeshes.size() != 0)
  {
    CORE->GetRenderer()->SetUniqueRenderPath("forward");

    CORE->GetLightManager()->SetLightsEnabled(false);

    m_pTargetObject->SetPitch(0.0f);
    m_pTargetObject->SetYaw(0.0f);

    if(m_pObjectModeLight)
    {
      CORE->GetLightManager()->SetAmbientLight(Vect3f(0.0f,0.0f,0.0f));
      m_pObjectModeLight->SetActive(true);
    }else{

    }

    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderDistance(0.0f);
    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderHeight(0.0f);
  
    FocusCurrentMesh();
  }else
    SetNextMode();
}

void CViewer::InitAnimatedMode()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CORE->GetRenderer()->SetUniqueRenderPath("forward");

    CORE->GetLightManager()->SetLightsEnabled(false);

    if(m_pObjectModeLight)
    {
      CORE->GetLightManager()->SetAmbientLight(Vect3f(0.0f,0.0f,0.0f));
      m_pObjectModeLight->SetActive(true);
    }else{

    }

    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderDistance(0.0f);
    ((CShoulderCamera*)m_pObjectCamera)->SetShoulderHeight(1.0f);

    m_pTargetObject->SetPitch(0.0f);
    m_pTargetObject->SetYaw(0.0f);

    FocusCurrentAnimatedModel();

  }else
    SetNextMode();
}

void CViewer::ProcessFreeMode(const float _fElapsedTime)
{
  Vect3f l_vPosDelta = Vect3f(0.0f);
  float l_fDeltaYaw = m_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = m_vMouseDelta.y * _fElapsedTime;

  CORE->GetLightManager()->SetAmbientLight(Vect3f(0.3f));
  
  CORE->GetLightManager()->SetLightsEnabled(m_bEnableLights);
  
  if(!CORE->GetActionManager()->IsActionActive("Run"))
  {
    SetWalking();
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveFwd"))
  {
    m_bMoveFwd = false;
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveBack"))
  {
    m_bMoveBack = false;
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveLeft"))
  {
    m_bMoveLeft = false;
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveRight"))
  {
    m_bMoveRight = false;
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveUp"))
  {
    m_bMoveUp = false;
  }

  if(!CORE->GetActionManager()->IsActionActive("MoveDown"))
  {
    m_bMoveDown = false;
  }

  if(m_bMoveFwd)
  {
    l_vPosDelta.x += 1.0f;
  }

  if(m_bMoveBack)
  {
    l_vPosDelta.x -= 1.0f;
  }

  if(m_bMoveLeft)
  {
    l_vPosDelta.z += 1.0f;
  }

  if(m_bMoveRight)
  {
    l_vPosDelta.z -= 1.0f;
  }

  if(m_bMoveUp)
  {
    l_vPosDelta.y += 1.0f;
  }

  if(m_bMoveDown)
  {
    l_vPosDelta.y -= 1.0f;
  }

  if(l_vPosDelta != Vect3f(0.0f))
    l_vPosDelta.Normalize();

  l_vPosDelta *= m_fVelocity * _fElapsedTime;

  UpdatePosition(l_vPosDelta,l_fDeltaPitch,l_fDeltaYaw);
  UpdateCamera(l_fDeltaPitch, l_fDeltaYaw);

  if (!m_bMoveFwd && !m_bMoveBack && !m_bMoveLeft && !m_bMoveLeft && !m_bMoveRight)
  {
    if(m_pCharacter)
    {
	    if(m_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() == 1)
	    {
		    m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
        m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(0,0.3f);
	    }
    }
  }
}

void CViewer::ProcessMeshMode(const float _fElapsedTime)
{  
  float l_fDeltaYaw = m_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = m_vMouseDelta.y * _fElapsedTime;

  UpdateCamera(l_fDeltaPitch,l_fDeltaYaw);

  if(m_vMeshes.size() != 0 && m_pObjectModeLight)
  {
    m_pObjectModeLight->SetPosition(m_pObjectCamera->GetEye());
    m_pObjectModeLight->SetDirection(m_pObjectCamera->GetDirection());
  }
}

void CViewer::ProcessAnimatedMode(const float _fElapsedTime)
{
  float l_fDeltaYaw = m_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = m_vMouseDelta.y * _fElapsedTime;

  UpdateCamera(l_fDeltaPitch,l_fDeltaYaw);

  if(m_vAnimatedModels.size() != 0 && m_pObjectModeLight)
  {
    m_pObjectModeLight->SetPosition(m_pObjectCamera->GetEye());
    m_pObjectModeLight->SetDirection(m_pObjectCamera->GetDirection());
  }
}

void CViewer::Update(const float _fElapsedTime)
{
  switch(m_iMode) {
  case FREE_MODE:
    ProcessFreeMode(_fElapsedTime);
    break;
  case MESH_MODE:
    ProcessMeshMode(_fElapsedTime);
    break;
  case ANIMATED_MODE:
    ProcessAnimatedMode(_fElapsedTime);
    break;
  default:
    break;
  }

  CORE->GetRenderableObjectsManager()->Update(_fElapsedTime);

  m_vMouseDelta = 0;
}

void CViewer::UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fYaw = 0.0f;

  if(m_pCharacter)
  {
    l_fYaw = m_pCharacter->GetYaw() - _fDeltaYaw;
    _PosDelta.RotateY(-l_fYaw+m_fInitialCharacterYaw);
    m_pCharacter->SetPosition(m_pCharacter->GetPosition()+_PosDelta);
    m_pCharacter->SetYaw(l_fYaw);
    m_pTargetObject->SetPosition(m_pCharacter->GetPosition()+Vect3f(0.0f,0.0f,0.0f));
  }else{
    l_fYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;
    _PosDelta.RotateY(-l_fYaw);
    m_pTargetObject->SetPosition(m_pTargetObject->GetPosition()+_PosDelta);
  }
}

void CViewer::UpdateCamera(float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fTargetPitch = m_pTargetObject->GetPitch() - _fDeltaPitch;
  float l_fTargetYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;

  if(l_fTargetPitch < -FLOAT_PI_VALUE/3) l_fTargetPitch = -FLOAT_PI_VALUE/3;
  if(l_fTargetPitch > FLOAT_PI_VALUE/3) l_fTargetPitch = FLOAT_PI_VALUE/3;
 
  m_pTargetObject->SetYaw(l_fTargetYaw);
  m_pTargetObject->SetPitch(l_fTargetPitch);
}

void CViewer::FocusCurrentMesh()
{
  CBoundingBox* l_BoundingBox = (*m_itCurrentMesh)->GetBoundingBox();
  float l_vZoom = l_BoundingBox->GetMaxSideLength() * 1.5f;

  CORE->GetRenderableObjectsManager()->SetAllVisibility(false);
  (*m_itCurrentMesh)->SetVisible(true);

  m_pTargetObject->SetPosition((*m_itCurrentMesh)->GetCenterPosition());

  ((CShoulderCamera*)m_pObjectCamera)->SetZoom(l_vZoom);

  //reset material values
  CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
  const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();

  ReloadMaterialValues(l_vMaterials);
}

void CViewer::FocusCurrentAnimatedModel()
{
  Vect3f l_vPosition = (*m_itCurrentAnimated)->GetPosition();
  float l_vZoom = 3.0f;

  CORE->GetRenderableObjectsManager()->SetAllVisibility(false);
  (*m_itCurrentAnimated)->SetVisible(true);

  m_pTargetObject->SetPosition(l_vPosition);
  ((CShoulderCamera*)m_pObjectCamera)->SetZoom(l_vZoom);

  //reset material values
  CRenderableAnimatedInstanceModel* l_pRenderMesh = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
  const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();

  ReloadMaterialValues(l_vMaterials);
}

void CViewer::SelectNextMesh()
{
  if(m_vMeshes.size() == 0)
    return;

  m_itCurrentMesh++;

  if(m_itCurrentMesh == m_vMeshes.end())
    m_itCurrentMesh = m_vMeshes.begin();

  FocusCurrentMesh();
}

void CViewer::SelectPrevMesh()
{
  m_fGlowIntensity = 0.0f;

  if(m_vMeshes.size() == 0)
    return;

  if(m_itCurrentMesh == m_vMeshes.begin())
  {
    m_itCurrentMesh = m_vMeshes.end()-1;
  }else{
    m_itCurrentMesh--;
  }

  FocusCurrentMesh();
}

void CViewer::SelectNextAnimatedModel()
{
  m_fGlowIntensity = 0.0f;

  if(m_vAnimatedModels.size() == 0)
    return;

  m_itCurrentAnimated++;

  if(m_itCurrentAnimated == m_vAnimatedModels.end())
    m_itCurrentAnimated = m_vAnimatedModels.begin();

  FocusCurrentAnimatedModel();
}

void CViewer::SelectPrevAnimatedModel()
{
  m_fGlowIntensity = 0.0f;

  if(m_vAnimatedModels.size() == 0)
    return;

  if(m_itCurrentAnimated == m_vAnimatedModels.begin())
  {
    m_itCurrentAnimated = m_vAnimatedModels.end()-1;
  }else{
    m_itCurrentAnimated--;
  }

  FocusCurrentAnimatedModel();
}

void CViewer::ToggleLights()
{
  m_bEnableLights = !m_bEnableLights;
}

//void CViewer::ToggleHelp()
//{
//  m_bShowHelp = !m_bShowHelp;
//}

void CViewer::IncreaseZoom()
{
  ((CThPSCamera*)m_pObjectCamera)->AddZoom(0.3f);
}

void CViewer::DecreaseZoom()
{
  ((CThPSCamera*)m_pObjectCamera)->AddZoom(-0.3f);
}

void CViewer::SetNextAnimation()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    int l_iNumAnimations = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimationCount();
    int l_iCurrentCycle = l_pRenderModel->GetAnimatedInstanceModel()->GetCurrentCycle();

    l_iCurrentCycle++;

    if(l_iCurrentCycle >= l_iNumAnimations)
    {
      l_iCurrentCycle = 0;
    }

    l_pRenderModel->GetAnimatedInstanceModel()->ClearCycle(0.0f);
    l_pRenderModel->GetAnimatedInstanceModel()->BlendCycle(l_iCurrentCycle,0.0f);
  }
}

void CViewer::SetPrevAnimation()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    int l_iNumAnimations = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimationCount();
    int l_iCurrentCycle = l_pRenderModel->GetAnimatedInstanceModel()->GetCurrentCycle();

    l_iCurrentCycle--;

    if(l_iCurrentCycle < 0)
    {
      l_iCurrentCycle = l_iNumAnimations-1;
    }

    l_pRenderModel->GetAnimatedInstanceModel()->ClearCycle(0.0f);
    l_pRenderModel->GetAnimatedInstanceModel()->BlendCycle(l_iCurrentCycle,0.0f);
  }
}

//void CViewer::ToggleNormalRendering()
//{
//  int l_iNormalRendering = m_eNormalRendering;
//  l_iNormalRendering++;
//
//  if(l_iNormalRendering == MAX_NORMAL_MODE)
//  {
//    l_iNormalRendering = 0;
//  }
//  m_eNormalRendering = (ENormalModes)l_iNormalRendering;
//
//  switch(m_eNormalRendering)
//  {
//  case CViewer::NORMALS:
//    CORE->GetRenderer()->SetSceneRenderer("show_normals_renderer");
//    break;
//  case CViewer::NORMALMAP:
//    CORE->GetRenderer()->SetSceneRenderer("show_normalmap_renderer");
//    break;
//  case CViewer::FLAT_NORMALMAP:
//    CORE->GetRenderer()->SetSceneRenderer("show_flat_normalmap_renderer");
//    break;
//  case CViewer::TANGENT:
//    CORE->GetRenderer()->SetSceneRenderer("show_tangent_renderer");
//    break;
//  case CViewer::COTANGENT:
//    CORE->GetRenderer()->SetSceneRenderer("show_bitangent_renderer");
//    break;
//  case CViewer::UV_COORDS:
//    CORE->GetRenderer()->SetSceneRenderer("show_uv_coords_renderer");
//    break;
//  case CViewer::NO_NORMALS:
//  default:
//    CORE->GetRenderer()->SetSceneRenderer("forward_renderer");
//    break;
//  }
//}

//void CViewer::ToggleShowBoxes()
//{
//  m_bShowBoxes = !m_bShowBoxes;
//
//  CORE->GetRenderableObjectsManager()->SetAllRenderBoundingBox(m_bShowBoxes);
//}
//
//void CViewer::ToggleShowSpheres()
//{
//  m_bShowSpheres = !m_bShowSpheres;
//
//  CORE->GetRenderableObjectsManager()->SetAllRenderBoundingSphere(m_bShowSpheres);
//}

void CViewer::IncrementGlowMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlowIntensity += 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlowIntensity(m_fGlowIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::DecrementGlowMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlowIntensity -= 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlowIntensity(m_fGlowIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::IncrementGlowAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlowIntensity += 0.1f;
    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlowIntensity(m_fGlowIntensity);

      ++l_itMaterial;
    }
    
  }
}

void CViewer::DecrementGlowAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlowIntensity -= 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlowIntensity(m_fGlowIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::IncrementSpecMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fSpecIntensity += 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetSpecularFactor(m_fSpecIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::DecrementSpecMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fSpecIntensity -= 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetSpecularFactor(m_fSpecIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::IncrementSpecAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fSpecIntensity += 0.1f;
    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetSpecularFactor(m_fSpecIntensity);

      ++l_itMaterial;
    }
    
  }
}

void CViewer::DecrementSpecAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fSpecIntensity -= 0.1f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetSpecularFactor(m_fSpecIntensity);

      ++l_itMaterial;
    }
  }
}

void CViewer::IncrementGlossMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlossiness += 1.0f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlossines(m_fGlossiness);

      ++l_itMaterial;
    }
  }
}

void CViewer::DecrementGlossMesh()
{
  if(m_vMeshes.size() != 0)
  {
    CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*m_itCurrentMesh);
    const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlossiness -= 1.0f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlossines(m_fGlossiness);

      ++l_itMaterial;
    }
  }
}

void CViewer::IncrementGlossAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlossiness += 1.0f;
    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlossines(m_fGlossiness);

      ++l_itMaterial;
    }
    
  }
}

void CViewer::DecrementGlossAnimated()
{
  if(m_vAnimatedModels.size() != 0)
  {
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    const vector<CMaterial*>& l_vMaterials = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    vector<CMaterial*>::const_iterator l_itMaterial = l_vMaterials.begin();

    m_fGlossiness -= 1.0f;

    while(l_itMaterial != l_vMaterials.end())
    {
      CMaterial* l_pMaterial = *l_itMaterial;

      l_pMaterial->SetGlossines(m_fGlossiness);

      ++l_itMaterial;
    }
  }
}

bool CViewer::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "setNextMode") == 0)
  {
    SetNextMode(); 
    return true;
  }

  if(strcmp(_pcAction, "showNormals") == 0)
  {
    //ToggleNormalRendering(); 
    return true;
  }

  if(strcmp(_pcAction, "toggleMENU") == 0)
  {
    ToggleInfo();
    return true;
  }

  if(strcmp(_pcAction, "SetLightsONOFF") == 0)
  {
    ToggleLights();
    return true;
  }

  if(strcmp(_pcAction, "ReloadLights") == 0)
  {
    CORE->GetLightManager()->Reload();
    m_pObjectModeLight = CORE->GetLightManager()->CreateDirectionalLight("ObjectModeLight",
                                                                        Vect3f(0.0f),
                                                                        Vect3f(1.0f,1.0f,1.0f),
                                                                        CColor(Vect3f(1.0f,1.0f,1.0f)),
                                                                        50.0f,
                                                                        80.0f,
                                                                        false);
    InitMode();
    return true;
  }

  if(strcmp(_pcAction, "Yaw") == 0)
  {
    m_vMouseDelta.x = (int)_fDelta;
    return true;
  }

  if(strcmp(_pcAction, "Pitch") == 0)
  {
    m_vMouseDelta.y = (int)_fDelta;
    return true;
  }

  if(strcmp(_pcAction, "SetNextMaterial") == 0)
  {
    NextMaterialProperty();
    return true;
  }
  
  if(strcmp(_pcAction, "Change Lightmap Mode") == 0)
  {
    /*switch(m_eLightmapMode)
    {
    case CMaterial::FLAT            : m_eLightmapMode = CMaterial::RADIOSITY_NORMAL ; break;
    case CMaterial::RADIOSITY_NORMAL: m_eLightmapMode = CMaterial::SHOW_R           ; break;
    case CMaterial::SHOW_R          : m_eLightmapMode = CMaterial::SHOW_G           ; break;
    case CMaterial::SHOW_G          : m_eLightmapMode = CMaterial::SHOW_B           ; break;
    case CMaterial::SHOW_B          : m_eLightmapMode = CMaterial::FLAT             ; break;
    };*/
    /*switch(m_eLightmapMode)
    {
    case CMaterial::FLAT            : m_eLightmapMode = CMaterial::RADIOSITY_NORMAL ; break;
    case CMaterial::RADIOSITY_NORMAL: m_eLightmapMode = CMaterial::FLAT             ; break;
    };


    vector<CRenderableObject*>::iterator l_it  = m_vMeshes.begin();
    vector<CRenderableObject*>::iterator l_end = m_vMeshes.end();

    for(; l_it != l_end; ++l_it)
    {
      CInstanceMesh* l_pRenderMesh = (CInstanceMesh*)(*l_it);
      const vector<CMaterial*>& l_vMaterials = l_pRenderMesh->GetStaticMesh()->GetMaterials();

      vector<CMaterial*>::const_iterator l_itMat  = l_vMaterials.cbegin();
      vector<CMaterial*>::const_iterator l_endMat = l_vMaterials.cend();

      for(; l_itMat != l_endMat; ++l_itMat)
      {
        (*l_itMat)->ActivateRadiosityNormal(m_eLightmapMode);
      }
    }*/
    return true;
  }

  switch(m_iMode) {
  case FREE_MODE:
    return ExecuteFreeModeAction(_fDeltaSeconds,_fDelta,_pcAction);
    break;
  case MESH_MODE:
    return ExecuteMeshModeAction(_fDeltaSeconds,_fDelta,_pcAction);
    break;
  case ANIMATED_MODE:
    return ExecuteAnimatedModeAction(_fDeltaSeconds,_fDelta,_pcAction);
    break;
  default:
    break;
  }

  return false;
}

bool CViewer::ExecuteFreeModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "Run") == 0)
  {
    SetRunning();
    return true;
  }

  if(strcmp(_pcAction, "MoveFwd") == 0)
  {
    MoveFwd();
	
    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {
    MoveBack();
	
    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    MoveLeft();
	
    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    MoveRight();
	
    return true;
  }

  if(strcmp(_pcAction, "MoveUp") == 0)
  {
    m_bMoveUp = true;
	
    return true;
  }

  if(strcmp(_pcAction, "MoveDown") == 0)
  {
    m_bMoveDown = true;
	
    return true;
  }

  //if(strcmp(_pcAction, "AugmentaAmbient") == 0)
  //{
  //  IncrementAmbientLight();
  //  return true;
  //}

  //if(strcmp(_pcAction, "DisminueixAmbient") == 0)
  //{
  //  DecrementAmbientLight();
  //  return true;
  //}

  //if(strcmp(_pcAction, "ShootBOT") == 0)
  //{
  //  if(m_pCharacter)
  //  {
  //    CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)m_pCharacter;
  //    l_pAnim->GetAnimatedInstanceModel()->ExecuteAction(3,1);
  //  }

  //  return true;
  //}

  //if(strcmp(_pcAction, "AimBot") == 0)
  //{
  //  if(m_pCharacter)
  //  {
  //    CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)m_pCharacter;
  //    if ((l_pAnim->GetAnimatedInstanceModel()->GetCurrentCycle() == 1) ||
		//  (l_pAnim->GetAnimatedInstanceModel()->GetCurrentCycle() == 0))
  //    {
  //      l_pAnim->GetAnimatedInstanceModel()->ClearCycle(0.5f);
  //      l_pAnim->GetAnimatedInstanceModel()->BlendCycle(2,0.5f);
  //    }
  //    else if (l_pAnim->GetAnimatedInstanceModel()->GetCurrentCycle() == 2)
  //    {
  //      l_pAnim->GetAnimatedInstanceModel()->ClearCycle(0.5f);
  //      l_pAnim->GetAnimatedInstanceModel()->BlendCycle(1,0.5f);
  //    }
  //  }

  //  return true;
  //}

  return false;
}

bool CViewer::ExecuteMeshModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    if (_fDelta < 0)
    {
      IncreaseZoom();
    }else{
      DecreaseZoom();
    }

    return true;
  }

  if(strcmp(_pcAction, "CanviObjecteDRETA") == 0)
  {
    SelectNextMesh();
    return true;
  }

  if(strcmp(_pcAction, "CanviObjecteESQUERRA") == 0)
  {
    SelectPrevMesh();
    return true;
  }

  if(strcmp(_pcAction, "ReloadMeshActual") == 0)
  {
    ReloadCurrentMesh();
    return true;
  }

  return false;
}

bool CViewer::ExecuteAnimatedModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    if (_fDelta < 0)
    {
      IncreaseZoom();
    }else{
      DecreaseZoom();
    }

    return true;
  }

  if(strcmp(_pcAction, "ReloadMeshActual") == 0)
  {
    ReloadCurrentAnimatedMesh();
    return true;
  }

  return false;
}

void CViewer::ResetActions()
{
  m_bMoveFwd = false;
  m_bMoveBack = false;
  m_bMoveLeft = false;
  m_bMoveRight = false;
  m_bMoveUp = false;
  m_bMoveDown = false;
}

void CViewer::SetRunning()
{
  m_fVelocity = 10.0f;
}

void CViewer::SetWalking()
{
  m_fVelocity = 5.0f;
}

void CViewer::MoveFwd()
{
  if(m_pCharacter)
  {
	  if(m_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	  {
		  m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
		  m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	  }
  }
	m_bMoveFwd = true;
}

void CViewer::MoveBack()
{
  if(m_pCharacter)
  {
	  if (m_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	  {
		  m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
          m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	  }
  }
  m_bMoveBack = true;
}

void CViewer::MoveLeft()
{
  if(m_pCharacter)
  {
	  if (m_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	  {
		  m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
          m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	  }
  }
	m_bMoveLeft = true;
}

void CViewer::MoveRight()
{
  if(m_pCharacter)
  {
	  if (m_pCharacter->GetAnimatedInstanceModel()->GetCurrentCycle() != 1)
	  {
		  m_pCharacter->GetAnimatedInstanceModel()->ClearCycle(0.3f);
          m_pCharacter->GetAnimatedInstanceModel()->BlendCycle(1,0.3f);
	  }
  }
	m_bMoveRight = true;
}

void CViewer::SetNextMode()
{
  m_iMode++;
  m_iMode %= 3;

  InitMode();
}

void CViewer::NextMaterialProperty()
{
  switch(m_eCurrentMaterialProperty)
  {
    case (CViewer::SPECULAR):
      m_eCurrentMaterialProperty = CViewer::GLOSSINESS;
      break;
    case (CViewer::GLOSSINESS):
      m_eCurrentMaterialProperty = CViewer::GLOW;
      break;
    case (CViewer::GLOW):
      m_eCurrentMaterialProperty = CViewer::SPECULAR;
      break;
    default:
      break;
  }

  InitMode();
}

void CViewer::ReloadCurrentMesh()
{
  CInstanceMesh* l_pCurrentMesh = (CInstanceMesh*)(*m_itCurrentMesh);

  if(l_pCurrentMesh)
  {
    l_pCurrentMesh->GetStaticMesh()->Reload();
    FocusCurrentMesh();
  }
}

void CViewer::ReloadCurrentAnimatedMesh()
{
  //TODO: Peta al recaregar els animats al recalcular les binormals...
  CRenderableAnimatedInstanceModel* l_pCurrentAnimatedMesh = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);

  if(l_pCurrentAnimatedMesh)
  {
    l_pCurrentAnimatedMesh->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->Reload();
    FocusCurrentAnimatedModel();
  }
}

void CViewer::ReloadMaterialValues(const vector<CMaterial*>& _vMaterials)
{
  m_fGlowIntensity = 0.0f;
  m_fSpecIntensity = 0.0f;
  m_fGlossiness = 0.0f;
  //m_fBump = 0.0f;

  vector<CMaterial*>::const_iterator l_itMaterial = _vMaterials.begin();

  while(l_itMaterial != _vMaterials.end())
  {
    CMaterial* l_pMaterial = *l_itMaterial;
    
    //glow intensity
    float l_fValue = l_pMaterial->GetGlowIntensity();
    
    if(l_fValue > m_fGlowIntensity)
      m_fGlowIntensity = l_fValue;

    //specular factor
    l_fValue = l_pMaterial->GetSpecularFactor();

    if(l_fValue > m_fSpecIntensity)
      m_fSpecIntensity = l_fValue;

    //glossiness
    l_fValue = l_pMaterial->GetGlossiness();

    if(l_fValue > m_fGlossiness)
      m_fGlossiness = l_fValue;

    //bump
    //l_fValue = l_pMaterial->GetBump();
    //
    //if(l_fValue > m_fBump)
    //  m_fBump = l_fValue;

    ++l_itMaterial;
  }
}

void CViewer::ShowInfo()
{
  if(m_bShowInfo)
  {
    switch(m_iMode) {
    case FREE_MODE:
      ShowFreeModeInfo();
      break;
    case MESH_MODE:
      ShowMeshModeInfo();
      break;
    case ANIMATED_MODE:
      ShowAnimatedModeInfo();
      break;
    default:
      break;
    }
  }
}

void CViewer::ShowFreeModeInfo()
{
  //uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  //int l_iPosicio = 420;
  //int l_iPosicio2 = 400;
  //string l_szMsg3("Mode Escena");
  //stringstream l_SStream;

  
  //l_SStream << "  Mode Lights -> ";
  //switch(m_eLightmapMode)
  //{
  //case CMaterial::FLAT            : l_SStream << "Flat" ; break;
  //case CMaterial::RADIOSITY_NORMAL: l_SStream << "Radiosity Normal" ; break;
  //case CMaterial::SHOW_R          : l_SStream << "Show R" ; break;
  //case CMaterial::SHOW_G          : l_SStream << "Show G" ; break;
  //case CMaterial::SHOW_B          : l_SStream << "Show B" ; break;
  //}

  //FONT_MANAGER->DrawText(550,40,colGREEN,l_uiFontType,l_SStream.str().c_str());

  //FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg3.c_str());

  
  //if(m_bShowHelp)
  //{
  //  l_SStreamHelp << "---<Ajuda>---" <<  endl;
  //  l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
  //  l_SStreamHelp << "[Mov. Endavant] W" <<  endl;
  //  l_SStreamHelp << "[Mov. Endarrera] S" <<  endl;
  //  l_SStreamHelp << "[Mov. Dreta] D" <<  endl;
  //  l_SStreamHelp << "[Mov. Esquerra] A" <<  endl;
  //  l_SStreamHelp << "[Vista Càmera] Ratolí" <<  endl;
  //  l_SStreamHelp << "[Correr] Mantenir L_Shift" <<  endl;
  //  l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
  //
  //  FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  //}

}

void CViewer::ShowMeshModeInfo()
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 100;
  int l_iPosicio2 = 20;
  string l_szMsg("Mode Meshes");
  stringstream l_SStream;
  //stringstream l_SStreamHelp;
  CInstanceMesh* l_pMeshInstance;

  if (m_vMeshes.size() > 0)
  {
    l_pMeshInstance = (CInstanceMesh*)(*m_itCurrentMesh);
    Vect3f l_vDimension = l_pMeshInstance->GetBoundingBox()->GetDimension();

    l_SStream << "Nom: " << l_pMeshInstance->GetName() << endl;
    l_SStream << "Core: " << l_pMeshInstance->GetStaticMesh()->GetFilename() << endl;
    l_SStream << "Dimensions(XYZ): " << (float)l_vDimension.x << " "
      << (float)l_vDimension.y << " "
      << (float)l_vDimension.z << endl;
    l_SStream << "Posicio: " << (float)l_pMeshInstance->GetPosition().x << " "
      << (float)l_pMeshInstance->GetPosition().y << " "
      << (float)l_pMeshInstance->GetPosition().z << endl;
    l_SStream << "Yaw: " << (float)l_pMeshInstance->GetYaw() << endl;
    l_SStream << "Pitch: " << (float)l_pMeshInstance->GetPitch() << endl;
    l_SStream << "Roll: " << (float)l_pMeshInstance->GetRoll() << endl;

    const vector<CMaterial*>& l_vMaterials = l_pMeshInstance->GetStaticMesh()->GetMaterials();

    l_SStream << ShowMaterialProperties(l_vMaterials);
  }

  //if(m_bShowHelp)
  //{
  //    l_SStreamHelp << "---<Ajuda>---" <<  endl;
  //    l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
  //    l_SStreamHelp << "[Següent Mesh] Fletxa Dreta" <<  endl;
  //    l_SStreamHelp << "[Anterior Mesh] Fletxa Esquerra" <<  endl;
  //    l_SStreamHelp << "[Moure Càmera] Ratolí" <<  endl;
  //    l_SStreamHelp << "[Zoom] Rodeta Ratolí" <<  endl;
  //    l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
  //        
  //    FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  //}

  //titol
  //FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
  //contingut
  FONT_MANAGER->DrawText(0,l_iPosicio,colBLACK,l_uiFontType,l_SStream.str().c_str());
}

void CViewer::ShowAnimatedModeInfo()
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 100;
  int l_iPosicio2 = 20;
  string l_szMsg("Mode Animats");
  stringstream l_SStream;
  //stringstream l_SStreamHelp;
  CRenderableAnimatedInstanceModel* l_pAnimatedInstance;

  if (m_vAnimatedModels.size() > 0)
  {
    l_pAnimatedInstance = (CRenderableAnimatedInstanceModel*)(*m_itCurrentAnimated);
    CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)l_pAnimatedInstance;

    l_SStream << "Nom: " << l_pAnimatedInstance->GetName() << endl;
    l_SStream << "Tipus: AnimatedModel" << endl;
    l_SStream << "Posicio: " << (float)l_pAnimatedInstance->GetPosition().x << " ";
    l_SStream << (float)l_pAnimatedInstance->GetPosition().y << " ";
    l_SStream << (float)l_pAnimatedInstance->GetPosition().y << endl;
    l_SStream << "Yaw: " << (float)l_pAnimatedInstance->GetYaw() << endl;
    l_SStream << "Pitch: " << (float)l_pAnimatedInstance->GetPitch() << endl;
    l_SStream << "Roll: " << (float)l_pAnimatedInstance->GetRoll() << endl;
    l_SStream << "Animacio: " << (int)l_pRenderModel->GetAnimatedInstanceModel()->GetCurrentCycle() << endl;

    const vector<CMaterial*>& l_vMaterials = l_pAnimatedInstance->GetAnimatedInstanceModel()->GetAnimatedCoreModel()->GetMaterials();
    l_SStream << ShowMaterialProperties(l_vMaterials);
  }

  //if(m_bShowHelp)
  //{
  //    l_SStreamHelp << "---<Ajuda>---" <<  endl;
  //    l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
  //    l_SStreamHelp << "[Següent Animat] Fletxa Dreta" <<  endl;
  //    l_SStreamHelp << "[Anterior Animat] Fletxa Esquerra" <<  endl;
  //    l_SStreamHelp << "[Següent Animacio] Fletxa UP" <<  endl;
  //    l_SStreamHelp << "[Anterior Animacio] Fletxa DOWN" <<  endl;
  //    l_SStreamHelp << "[Moure Càmera] Ratolí" <<  endl;
  //    l_SStreamHelp << "[Zoom] Rodeta Ratolí" <<  endl;
  //    l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
  //        
  //    FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  //}

  //titol
  //FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
  //contingut
  FONT_MANAGER->DrawText(0,l_iPosicio,colBLACK,l_uiFontType,l_SStream.str().c_str());
}

string CViewer::ShowMaterialProperties(const vector<CMaterial*>& _vMaterials)
{

  stringstream l_SStream;
  
  l_SStream << "Materials: " << endl;

  vector<CMaterial*>::const_iterator l_itMaterial = _vMaterials.begin();

  while(l_itMaterial != _vMaterials.end())
  {
    CMaterial* l_pMaterial = *l_itMaterial;

    int l_iMaterialType = l_pMaterial->GetMaterialType();

    l_SStream << "Material type: ";

    if(l_iMaterialType & DIFFUSE_MATERIAL_MASK)
    {
      l_SStream << "Diffuse ";
    }
      
    if(l_iMaterialType & NORMALMAP_MATERIAL_MASK)
    {
      l_SStream << "NormalMap ";
    }

    if(l_iMaterialType & LIGHTMAP_MATERIAL_MASK)
    {
      l_SStream << "LightMap ";
    }

    if(l_iMaterialType & SPECULARMAP_MATERIAL_MASK)
    {
      l_SStream << "SpecularMap ";
    }

    if(l_iMaterialType & GLOW_MATERIAL_MASK)
    {
      l_SStream << "Glow ";
    }

    if(l_iMaterialType & PARALLAX_MATERIAL_MASK)
    {
      l_SStream << "Parallax ";
    }
      
    l_SStream << endl;

    l_SStream << "Parallax Height: " << l_pMaterial->GetParallaxHeight() << endl;

    if(m_eCurrentMaterialProperty == CViewer::GLOSSINESS)
      l_SStream << "---[SELECTED]--->";
    l_SStream << "Glossiness: " << l_pMaterial->GetGlossiness() << "3dMAX(" << l_pMaterial->GetGlossiness() << ")" << endl;

    if(m_eCurrentMaterialProperty == CViewer::SPECULAR)
      l_SStream << "---[SELECTED]--->";
    l_SStream << "Specular factor: " << l_pMaterial->GetSpecularFactor() << "3dMAX(" << (l_pMaterial->GetSpecularFactor()/.03f) << ")" << endl;

    if(m_eCurrentMaterialProperty == CViewer::GLOW)
      l_SStream << "---[SELECTED]--->";
    l_SStream << "Glow intensity: " << l_pMaterial->GetGlowIntensity() << "3dMAX(" << (l_pMaterial->GetGlowIntensity()/.02f) << ")" << endl;

    l_SStream << "Alfa blend: " << l_pMaterial->HasAlphaBlending() << endl;

    ++l_itMaterial;
  }

  return l_SStream.str();
}