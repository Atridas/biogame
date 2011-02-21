#include <algorithm>
#include <sstream>
#include "Viewer.h"
#include "Core.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "FontManager.h"
#include "ThPSCamera.h"
#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"
#include <LightManager.h>
#include "SpotLight.h"

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
  m_bRenderLights = false;

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
  }

  m_pTargetObject = new CObject3D();
  m_pTargetObject->SetPosition(Vect3f(0.0f,2.0f,0.0f));

  m_pObjectCamera = new CThPSCamera(
      0.1f,
      100.0f,
      35.0f * FLOAT_PI_VALUE/180.0f,
      ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
      m_pTargetObject,
      2.5f);

  m_pObjectModeLight = (CDirectionalLight*)CORE->GetLightManager()->GetResource("ObjectModeLight");
  m_pSpotLight = (CSpotLight*)CORE->GetLightManager()->GetResource("FreeModeLight");

  m_vAmbientLight = CORE->GetLightManager()->GetAmbientLight();

  m_bEnableLights = true;
  m_bShowHelp = true;
  m_bNormalRendering = false;
  m_bShowBoxes = false;

  m_iMode = FREE_MODE;

  ResetActions();

  InitMode();

}


CViewer::~CViewer(void)
{
  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pTargetObject)
}

void CViewer::InitMode()
{
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
  m_fVelocity = 5.0f;

  CORE->GetLightManager()->SetLightsEnabled(m_bEnableLights);
  CORE->GetRenderableObjectsManager()->SetAllVisibility(true);

  if(m_pCharacter)
  {
    m_pTargetObject->SetYaw(m_pCharacter->GetYaw()-m_fInitialCharacterYaw);
    m_pTargetObject->SetPosition(m_pCharacter->GetPosition()+Vect3f(0.0f,2.0f,0.0f));
  }else{
    m_pTargetObject->SetYaw(0.0f);
    m_pTargetObject->SetPosition(Vect3f(m_pTargetObject->GetPosition().x,2.0f,m_pTargetObject->GetPosition().z));
  }

  m_pTargetObject->SetPitch(0.0f);

  ((CThPSCamera*)m_pObjectCamera)->SetZoom(2.5f);

  if(m_pObjectModeLight)
  {
    m_pObjectModeLight->SetActive(false);
  }

  CORE->GetLightManager()->SetAmbientLight(m_vAmbientLight);

  UpdatePosition(Vect3f(0.0f),0.0f,0.0f);
  UpdateCamera(0.0f,0.0f);
}

void CViewer::InitMeshMode()
{
  CORE->GetLightManager()->SetLightsEnabled(false);

  if(m_vMeshes.size() == 0)
  {
    SetNextMode();
    return;
  }

  m_pTargetObject->SetPitch(0.0f);
  m_pTargetObject->SetYaw(0.0f);

  if(m_pObjectModeLight)
  {
    CORE->GetLightManager()->SetAmbientLight(Vect3f(0.0f,0.0f,0.0f));
    m_pObjectModeLight->SetActive(true);
  }else{

  }

  FocusCurrentMesh();
}

void CViewer::InitAnimatedMode()
{
  CORE->GetLightManager()->SetLightsEnabled(false);

  if(m_vAnimatedModels.size() == 0)
  {
    SetNextMode();
    return;
  }

  if(m_pObjectModeLight)
  {
    CORE->GetLightManager()->SetAmbientLight(Vect3f(0.0f,0.0f,0.0f));
    m_pObjectModeLight->SetActive(true);
  }else{

  }

  m_pTargetObject->SetPitch(0.0f);
  m_pTargetObject->SetYaw(0.0f);

  FocusCurrentAnimatedModel();
}

void CViewer::ProcessFreeMode(const float _fElapsedTime,const Vect3i& _vMouseDelta)
{
  Vect3f l_vPosDelta = Vect3f(0.0f);
  float l_fDeltaYaw = _vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = _vMouseDelta.y * _fElapsedTime;

  CORE->GetLightManager()->SetAmbientLight(m_vAmbientLight);
  CORE->GetLightManager()->SetLightsEnabled(m_bEnableLights);
  if(m_pObjectModeLight)
  {
    m_pObjectModeLight->SetActive(false);
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

  if(l_vPosDelta != Vect3f(0.0f))
    l_vPosDelta.Normalize();

  l_vPosDelta *= m_fVelocity * _fElapsedTime;

  UpdatePosition(l_vPosDelta,l_fDeltaPitch,l_fDeltaYaw);
  UpdateCamera(l_fDeltaPitch, l_fDeltaYaw);

  if(m_pSpotLight)
  {
    Vect3f l_vPosition = m_pTargetObject->GetPosition();
    m_pSpotLight->SetPosition(l_vPosition);
    m_pSpotLight->SetDirection(m_pObjectCamera->GetDirection());
  }

}

void CViewer::ProcessMeshMode(const float _fElapsedTime,const Vect3i& _vMouseDelta)
{  
  float l_fDeltaYaw = _vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = _vMouseDelta.y * _fElapsedTime;

  UpdateCamera(l_fDeltaPitch,l_fDeltaYaw);

  if(m_pObjectModeLight)
  {
    m_pObjectModeLight->SetPosition(m_pObjectCamera->GetEye());
    m_pObjectModeLight->SetDirection(m_pObjectCamera->GetDirection());
  }
}

void CViewer::ProcessAnimatedMode(const float _fElapsedTime,const Vect3i& _vMouseDelta)
{
  float l_fDeltaYaw = _vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = _vMouseDelta.y * _fElapsedTime;

  UpdateCamera(l_fDeltaPitch,l_fDeltaYaw);

  if(m_pObjectModeLight)
  {
    m_pObjectModeLight->SetPosition(m_pObjectCamera->GetEye());
    m_pObjectModeLight->SetDirection(m_pObjectCamera->GetDirection());
  }
}

void CViewer::Update(const float _fElapsedTime,const Vect3i& _vMouseDelta)
{
  switch(m_iMode) {
  case FREE_MODE:
    ProcessFreeMode(_fElapsedTime,_vMouseDelta);
    break;
  case MESH_MODE:
    ProcessMeshMode(_fElapsedTime,_vMouseDelta);
    break;
  case ANIMATED_MODE:
    ProcessAnimatedMode(_fElapsedTime,_vMouseDelta);
    break;
  default:
    break;
  }

  ResetActions();
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
    m_pTargetObject->SetPosition(m_pCharacter->GetPosition()+Vect3f(0.0f,2.0f,0.0f));
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
  float l_vZoom = l_BoundingBox->GetMaxSideLength() * 3.0f;

  CORE->GetRenderableObjectsManager()->SetAllVisibility(false);
  (*m_itCurrentMesh)->SetVisible(true);

  m_pTargetObject->SetPosition((*m_itCurrentMesh)->GetCenterPosition());
  ((CThPSCamera*)m_pObjectCamera)->SetZoom(l_vZoom);

}

void CViewer::FocusCurrentAnimatedModel()
{
  Vect3f l_vPosition = (*m_itCurrentAnimated)->GetPosition();
  float l_vZoom = 6.0f;

  CORE->GetRenderableObjectsManager()->SetAllVisibility(false);
  (*m_itCurrentAnimated)->SetVisible(true);

  m_pTargetObject->SetPosition(Vect3f(l_vPosition.x,1.0f,l_vPosition.z));
  ((CThPSCamera*)m_pObjectCamera)->SetZoom(l_vZoom);

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
  if(m_vAnimatedModels.size() == 0)
    return;

  m_itCurrentAnimated++;

  if(m_itCurrentAnimated == m_vAnimatedModels.end())
    m_itCurrentAnimated = m_vAnimatedModels.begin();

  FocusCurrentAnimatedModel();
}

void CViewer::SelectPrevAnimatedModel()
{
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

void CViewer::IncrementAmbientLight()
{
  m_vAmbientLight += 0.05f;
  m_vAmbientLight.x = m_vAmbientLight.x > 1.0f? 1.0f : m_vAmbientLight.x;
  m_vAmbientLight.y = m_vAmbientLight.y > 1.0f? 1.0f : m_vAmbientLight.y;
  m_vAmbientLight.z = m_vAmbientLight.z > 1.0f? 1.0f : m_vAmbientLight.z;
}

void CViewer::DecrementAmbientLight()
{
  m_vAmbientLight -= 0.05f;
  m_vAmbientLight.x = m_vAmbientLight.x < 0.0f? 0.0f : m_vAmbientLight.x;
  m_vAmbientLight.y = m_vAmbientLight.y < 0.0f? 0.0f : m_vAmbientLight.y;
  m_vAmbientLight.z = m_vAmbientLight.z < 0.0f? 0.0f : m_vAmbientLight.z;
}

void CViewer::ToggleLights()
{
  m_bEnableLights = !m_bEnableLights;
}

void CViewer::ToggleHelp()
{
  m_bShowHelp = !m_bShowHelp;
}

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

void CViewer::SetPrevAnimation()
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

void CViewer::ToggleNormalRendering()
{
  m_bNormalRendering = !m_bNormalRendering;
}

void CViewer::ToggleShowBoxes()
{
  m_bShowBoxes = !m_bShowBoxes;

  CORE->GetRenderableObjectsManager()->SetAllRenderBoundingBox(m_bShowBoxes);
}

bool CViewer::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(strcmp(_pcAction, "ChangeMode") == 0)
  {
    SetNextMode(); 
    return true;
  }

  if(strcmp(_pcAction, "ShowAjuda") == 0)
  {
    ToggleHelp();
    return true;
  }

  if(strcmp(_pcAction, "ToggleShowBoxes") == 0)
  {
    ToggleShowBoxes();
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

  if(strcmp(_pcAction, "Walk") == 0)
  {
    SetWalking();
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

 if(strcmp(_pcAction, "AugmentaAmbient") == 0)
  {
    IncrementAmbientLight();
    return true;
  }

  if(strcmp(_pcAction, "DisminueixAmbient") == 0)
  {
    DecrementAmbientLight();
    return true;
  }

  if(strcmp(_pcAction, "SetLightsONOFF") == 0)
  {
    ToggleLights();
  }

  if(strcmp(_pcAction, "ShootBOT") == 0)
  {
    if(m_pCharacter)
    {
      CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)m_pCharacter;
      l_pAnim->GetAnimatedInstanceModel()->ExecuteAction(4,1);
    }

    return true;
  }

  if(strcmp(_pcAction, "HurtBOT") == 0)
  {
    if(m_pCharacter)
    {
      CRenderableAnimatedInstanceModel* l_pAnim = (CRenderableAnimatedInstanceModel*)m_pCharacter;
      l_pAnim->GetAnimatedInstanceModel()->ExecuteAction(3,1);
    }

    return true;
  }
  return false;
}

bool CViewer::ExecuteMeshModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

    if (l_vDelta.z < 0)
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

  return false;
}

bool CViewer::ExecuteAnimatedModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{

  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

    if (l_vDelta.z < 0)
    {
      IncreaseZoom();
    }else{
      DecreaseZoom();
    }

    return true;
  }

  if(strcmp(_pcAction, "CanviObjecteDRETA") == 0)
  {
    SelectNextAnimatedModel();
    return true;
  }

  if(strcmp(_pcAction, "CanviObjecteESQUERRA") == 0)
  {
    SelectPrevAnimatedModel();
    return true;
  }

  if(strcmp(_pcAction, "CanviAnimacioUP") == 0)
  {
    SetNextAnimation();
    return true;
  }


  if(strcmp(_pcAction, "CanviAnimacioDOWN") == 0)
  {
    SetPrevAnimation();
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
  m_bMoveFwd = true;
}

void CViewer::MoveBack()
{
    m_bMoveBack = true;
}

void CViewer::MoveLeft()
{
  m_bMoveLeft = true;
}

void CViewer::MoveRight()
{
  m_bMoveRight = true;
}

void CViewer::SetNextMode()
{
  m_iMode++;
  m_iMode %= 3;

  InitMode();
}

void CViewer::ShowInfo()
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

void CViewer::ShowFreeModeInfo()
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 420;
  int l_iPosicio2 = 400;
  string l_szMsg3("Mode Escena");
  stringstream l_SStream;
  stringstream l_SStreamHelp;

  FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg3.c_str());

  if(m_bShowHelp)
  {
    l_SStreamHelp << "---<Ajuda>---" <<  endl;
    l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
    l_SStreamHelp << "[Mov. Endavant] W" <<  endl;
    l_SStreamHelp << "[Mov. Endarrera] S" <<  endl;
    l_SStreamHelp << "[Mov. Dreta] D" <<  endl;
    l_SStreamHelp << "[Mov. Esquerra] A" <<  endl;
    l_SStreamHelp << "[Vista Càmera] Ratolí" <<  endl;
    l_SStreamHelp << "[Correr] Mantenir L_Shift" <<  endl;
    l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
  }

  FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());

}

void CViewer::ShowMeshModeInfo()
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 420;
  int l_iPosicio2 = 400;
  string l_szMsg("Mode Meshes");
  stringstream l_SStream;
  stringstream l_SStreamHelp;
  CRenderableObject* l_pMeshInstance;

  if (m_vMeshes.size() > 0)
  {
    FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
    l_pMeshInstance = *m_itCurrentMesh;
    Vect3f l_vDimension = l_pMeshInstance->GetBoundingBox()->GetDimension();

    l_SStream << "Nom: " << l_pMeshInstance->GetName() << endl;
    l_SStream << "Tipus: StaticMesh" << endl;
    l_SStream << "Dimensions(XYZ): " << (float)l_vDimension.x << "x";
    l_SStream << (float)l_vDimension.y << "x";
    l_SStream << (float)l_vDimension.z << endl;
    l_SStream << "Posicio: " << (float)l_pMeshInstance->GetPosition().x << " ";
    l_SStream << (float)l_pMeshInstance->GetPosition().y << " ";
    l_SStream << (float)l_pMeshInstance->GetPosition().y << endl;
    l_SStream << "Yaw: " << (float)l_pMeshInstance->GetYaw() << endl;
    l_SStream << "Pitch: " << (float)l_pMeshInstance->GetPitch() << endl;
    l_SStream << "Roll: " << (float)l_pMeshInstance->GetRoll() << endl;
    FONT_MANAGER->DrawText(0,l_iPosicio,colGREEN,l_uiFontType,l_SStream.str().c_str());
  }else{
    FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
    l_SStream << "No hi ha cap OBJECTE MESH per mostrar" << endl;
    FONT_MANAGER->DrawText(0,l_iPosicio,colGREEN,l_uiFontType,l_SStream.str().c_str());
  }

  if(m_bShowHelp)
  {
         
      l_SStreamHelp << "---<Ajuda>---" <<  endl;
      l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
      l_SStreamHelp << "[Següent Mesh] Fletxa Dreta" <<  endl;
      l_SStreamHelp << "[Anterior Mesh] Fletxa Esquerra" <<  endl;
      l_SStreamHelp << "[Moure Càmera] Ratolí" <<  endl;
      l_SStreamHelp << "[Zoom] Rodeta Ratolí" <<  endl;
      l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
          
      FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
  }
}

void CViewer::ShowAnimatedModeInfo()
{
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 420;
  int l_iPosicio2 = 400;
  string l_szMsg("Mode Animats");
  stringstream l_SStream;
  stringstream l_SStreamHelp;
  CRenderableObject* l_pAnimatedInstance;

  if (m_vAnimatedModels.size() > 0)
    {
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
      l_pAnimatedInstance = *m_itCurrentAnimated;
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
      FONT_MANAGER->DrawText(0,l_iPosicio,colGREEN,l_uiFontType,l_SStream.str().c_str());
    }
    else
    {
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
      l_SStream << "No hi ha cap OBJECTE ANIMAT per mostrar" << endl;
      FONT_MANAGER->DrawText(0,l_iPosicio,colGREEN,l_uiFontType,l_SStream.str().c_str());
    }

    if(m_bShowHelp)
    {
        l_SStreamHelp << "---<Ajuda>---" <<  endl;
        l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
        l_SStreamHelp << "[Següent Animat] Fletxa Dreta" <<  endl;
        l_SStreamHelp << "[Anterior Animat] Fletxa Esquerra" <<  endl;
        l_SStreamHelp << "[Següent Animacio] Fletxa UP" <<  endl;
        l_SStreamHelp << "[Anterior Animacio] Fletxa DOWN" <<  endl;
        l_SStreamHelp << "[Moure Càmera] Ratolí" <<  endl;
        l_SStreamHelp << "[Zoom] Rodeta Ratolí" <<  endl;
        l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
          
        FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
    }
}
