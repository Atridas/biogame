#include "IAProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "RenderableObjectsManager.h"
#include "LightManager.h"
#include "ActionManager.h"
#include "ScriptManager.h"
#include "GraphDefines.h"
#include "Heuristics.h"
#include "SearchAStar.h"

bool CIAProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CIAProcess::Init");
  
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  m_pTargetObject = new CObject3D();
  
  m_pTargetObject->SetPosition(Vect3f(-8.0f,2.0f,-4.0f));

  m_pObjectCamera = new CFPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pTargetObject);

  m_pCamera = m_pObjectCamera;

  m_pGraph = new CSparseGraph(false);

  GraphHelper_CreateGrid(*m_pGraph, 40, 40, 10, 10);


  m_vDemoObj = Vect3f(3.5f,0.f,3.5f);

  int m_iNodeInicial = m_pGraph->GetClosestNode(m_vDemoObj);

  
  m_iNodeObjectiu = RandomNumber(0, m_pGraph->NumNodes());

  CSearchAStar l_Search(*m_pGraph, &(CHeuristicEuclid::instance), m_iNodeInicial, m_iNodeObjectiu);

  m_liPath = l_Search.GetPathToTarget();

  SetOk(true);
  return IsOk();
}

void CIAProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CSoundTestProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pTargetObject)
  CHECKED_DELETE(m_pGraph)
}

void CIAProcess::Update(float _fElapsedTime)
{
  float l_fVelocity = 3.0f;
  Vect3f l_vPosDelta = Vect3f(0.0f);
  float l_fDeltaYaw = m_vMouseDelta.x * _fElapsedTime;
  float l_fDeltaPitch = m_vMouseDelta.y * _fElapsedTime;

  if(CORE->GetActionManager()->IsActionActive("Run"))
  {
    l_fVelocity = 5.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveFwd"))
  {
    l_vPosDelta.x += 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveBack"))
  {
    l_vPosDelta.x -= 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveLeft"))
  {
    l_vPosDelta.z += 1.0f;
  }

  if(CORE->GetActionManager()->IsActionActive("MoveRight"))
  {
    l_vPosDelta.z -= 1.0f;
  }

  if(l_vPosDelta != Vect3f(0.0f))
    l_vPosDelta.Normalize();

  l_vPosDelta *= l_fVelocity * _fElapsedTime;

  UpdatePosition(l_vPosDelta,l_fDeltaPitch,l_fDeltaYaw);
  UpdateCamera(l_fDeltaPitch, l_fDeltaYaw);

  Vect3f l_vFrontDirection(0.0f);

  l_vFrontDirection.x = 1.0f;
  l_vFrontDirection.RotateY(-m_pTargetObject->GetYaw());

  m_vMouseDelta = 0;

  UpdateIA(_fElapsedTime);
}

void CIAProcess::UpdateCamera(float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fTargetPitch = m_pTargetObject->GetPitch() - _fDeltaPitch;
  float l_fTargetYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;

  if(l_fTargetPitch < -FLOAT_PI_VALUE/3) l_fTargetPitch = -FLOAT_PI_VALUE/3;
  if(l_fTargetPitch > FLOAT_PI_VALUE/3) l_fTargetPitch = FLOAT_PI_VALUE/3;
 
  m_pTargetObject->SetYaw(l_fTargetYaw);
  m_pTargetObject->SetPitch(l_fTargetPitch);
}

void CIAProcess::UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw)
{
  float l_fYaw = 0.0f;

  l_fYaw = m_pTargetObject->GetYaw() - _fDeltaYaw;
  _PosDelta.RotateY(-l_fYaw);
  m_pTargetObject->SetPosition(m_pTargetObject->GetPosition()+_PosDelta);

}

void CIAProcess::UpdateIA(float _fDT)
{
  if(m_liPath.empty())
  {
    return;
  }

  Vect3f l_vObj = m_pGraph->GetNode(m_liPath.front()).GetPosition();

  Vect3f l_vDir = l_vObj - m_vDemoObj;
  l_vDir = l_vDir.Normalize(1.f);

  m_vDemoObj += l_vDir * _fDT;

  if(m_vDemoObj.SqDistance(l_vObj) < 0.2)
  {
    m_liPath.pop_front();
  }
}

void CIAProcess::RenderScene(CRenderManager* _pRM)
{
  CORE->GetRenderableObjectsManager()->Render(_pRM);


  //_pRM->DrawGrid(20,colWHITE,20,20);
  m_pGraph->DebugRender(_pRM);

  Mat44f mat;
  mat.SetIdentity();
  mat.Translate( m_vDemoObj );
  mat.m13 = 2.f;
  _pRM->SetTransform(mat);

  //Objecte que es mou
  _pRM->DrawCube(Vect3f(1,1,1),colYELLOW);
  
  //---

  mat.SetIdentity();
  mat.Translate( m_pGraph->GetNode(m_iNodeObjectiu).GetPosition() );
  mat.m13 = 2.f;
  _pRM->SetTransform(mat);

  //objectiu
  _pRM->DrawSphere(.5f,colYELLOW,10);

  //PATH

  mat.SetIdentity();
  mat.m13 = 1.f;
  _pRM->SetTransform(mat);
  

  list<int>::iterator l_itPath = m_liPath.begin();
  if(l_itPath != m_liPath.end())
  {
    Vect3f l_vLastPos = m_pGraph->GetNode(*l_itPath).GetPosition();
    _pRM->DrawLine(l_vLastPos, m_vDemoObj, colWHITE);

    ++l_itPath;
    while(l_itPath != m_liPath.end())
    {
      Vect3f l_vNewPos = m_pGraph->GetNode(*l_itPath).GetPosition();

      _pRM->DrawLine(l_vLastPos, l_vNewPos, colWHITE);

      ++l_itPath;
      l_vLastPos = l_vNewPos;
    }
  }
}

void CIAProcess::RenderINFO(CRenderManager* _pRM)
{


  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 0;
  int l_iPosicio2 = 130;
  FONT_MANAGER->DrawText(l_iPosicio,l_iPosicio2,colGREEN,l_uiFontType,"Press 'N' to change target");
}

bool CIAProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
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

  if(strcmp(_pcAction, "change IA target") == 0)
  {
    m_iNodeObjectiu = RandomNumber(0, m_pGraph->NumNodes());
    int m_iNodeInicial = m_pGraph->GetClosestNode(m_vDemoObj);

    CSearchAStar l_Search(*m_pGraph, &(CHeuristicEuclid::instance), m_iNodeInicial, m_iNodeObjectiu);

    m_liPath = l_Search.GetPathToTarget();
    return true;
  }

  return false;
}

