#include "ViewerProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "FPSCamera.h"
#include "ThPSCamera.h"
#include "InputManager.h"
#include "Texture.h"
#include "TextureManager.h"
#include "StaticMesh.h"
#include "StaticMeshManager.h"
#include "RenderableObjectsManager.h"
#include "AnimatedModelManager.h"
#include "RenderableObject.h"
#include "AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"

#include <IndexedVertexs.h>
#include "VertexsStructs.h"

#include <LightManager.h>
#include <sstream>
#include "SpotLight.h"

bool SortRenderableObjectByName(CRenderableObject* _pRO1, CRenderableObject* _pRO2)
{
  string l_szName1 = _pRO1->GetName();
  string l_szName2 = _pRO2->GetName();

  transform(l_szName1.begin(), l_szName1.end(), l_szName1.begin(), tolower);
  transform(l_szName2.begin(), l_szName2.end(), l_szName2.begin(), tolower);

  return (l_szName1.compare(l_szName2) < 0);
}

bool CViewerProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Init");
  
 
  m_pObject = new CObject3D();
  m_fVelocity = 5;
  
  m_pObject->SetPosition(Vect3f(-6,1.7f,0));

  m_pObjectBot = CORE->GetRenderableObjectsManager()->GetResource("bot");
  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  m_iState = 0;
  m_iMode = 0;

  m_bStateChanged = false;

  //m_pObjectCamera = new CFPSCamera(
  //  0.1f,
  //  100.0f,
  //  45.0f * FLOAT_PI_VALUE/180.0f,
  //  ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
  //  m_pObject);

  m_pObjectCamera = new CThPSCamera(
    0.1f,
    100.0f,
    35.0f * FLOAT_PI_VALUE/180.0f,
    ((float)RENDER_MANAGER->GetScreenWidth())/((float)RENDER_MANAGER->GetScreenHeight()),
    m_pObject,
    2.5f);

  m_pCamera = m_pObjectCamera;
  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  
  if (l_Spot != 0)
  {
    l_Spot->SetDirection(m_pObject->GetPosition());
    l_Spot->SetActive(true);
  }
  
  m_vMeshes = CORE->GetRenderableObjectsManager()->GetMeshes();
  m_vAnimatedModels = CORE->GetRenderableObjectsManager()->GetAnimatedModels();

  sort(m_vMeshes.begin(),m_vMeshes.end(),SortRenderableObjectByName);
  sort(m_vAnimatedModels.begin(),m_vAnimatedModels.end(),SortRenderableObjectByName);

  m_bRenderLights = false;

  SetOk(true);
  return IsOk();
}

void CViewerProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"CViewerProcess::Release");

  CHECKED_DELETE(m_pObjectCamera)
  CHECKED_DELETE(m_pObject)
}

void CViewerProcess::Update(float _fElapsedTime)
{
  //Actualitze el pitch i el yaw segons els delta del mouse
  float l_fPitch, l_fYaw;

  Vect3i l_vVec = INPUT_MANAGER->GetMouseDelta();
  CSpotLight* l_Spot = (CSpotLight*)CORE->GetLightManager()->GetResource("Spot01");

  l_fPitch = m_pObject->GetPitch();
  l_fYaw = m_pObject->GetYaw();
  
  m_pObject->SetYaw(l_fYaw-l_vVec.x*_fElapsedTime);
  
  l_fPitch -= l_vVec.y*_fElapsedTime;
  if(l_fPitch < - FLOAT_PI_VALUE/3) l_fPitch = - FLOAT_PI_VALUE/3;
  if(l_fPitch >   FLOAT_PI_VALUE/3) l_fPitch =   FLOAT_PI_VALUE/3;
  m_pObject->SetPitch(l_fPitch);

  if (m_iMode == 0)
  {
    if (m_pObjectBot != 0)
    {
      m_pObjectBot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObjectBot->GetPosition().y, m_pObject->GetPosition().z));
      m_pObjectBot->SetYaw(m_pObject->GetYaw()-FLOAT_PI_VALUE/2.0f);
      l_fPitch = m_pObject->GetPitch();
      l_fYaw = m_pObjectBot->GetYaw();
      l_fYaw = l_fYaw+FLOAT_PI_VALUE/2;
      if (l_Spot != 0)
      {
        l_Spot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObject->GetPosition().y+0.5f, m_pObject->GetPosition().z));
        Vect3f l_vec(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch),sin(l_fYaw) * cos(l_fPitch) );
        l_Spot->SetDirection(Vect3f(l_vec.x,l_vec.y,l_vec.z));
      }
    }
    else
    {
      if (l_Spot != 0)
      {
        l_fPitch = m_pObject->GetPitch();
        l_fYaw = m_pObject->GetYaw();
        l_Spot->SetPosition(Vect3f(m_pObject->GetPosition().x, m_pObject->GetPosition().y+0.5f, m_pObject->GetPosition().z));
        Vect3f l_vec(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch),sin(l_fYaw) * cos(l_fPitch) );
        l_Spot->SetDirection(Vect3f(l_vec.x,l_vec.y,l_vec.z));
        //l_Spot->
      }
    }
  }
 


  if(m_bStateChanged)
  {
    if (m_pObjectBot != 0)
    {
      ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle(m_iState,0);
    }
    m_bStateChanged = false;
  }

}

void CViewerProcess::RenderScene(CRenderManager* _pRM)
{
  //Render Objects
  CORE->GetRenderableObjectsManager()->Render(_pRM);
 

  //Render Lights
  if(m_bRenderLights)
    CORE->GetLightManager()->Render(_pRM);

  //Matrix for testing
  Mat44f r,r2, t, s, identity, total;

  identity.SetIdentity();
  r.SetIdentity();
  r2.SetIdentity();
  t.SetIdentity();
  s.SetIdentity();

  //Draw Grid and Axis
  _pRM->SetTransform(identity);

 

  
}

bool CViewerProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

  if(strcmp(_pcAction, "Run") == 0)
  {
    if (m_iMode == 0)
    {
      m_fVelocity = 10;
    }
    
    return true;
  }

  if(strcmp(_pcAction, "Walk") == 0)
  {
    if (m_iMode == 0)
    {
      m_fVelocity = 5;
    }
    return true;
  }

  if(strcmp(_pcAction, "MoveFwd") == 0)
  {

    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }
    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

 if(strcmp(_pcAction, "MoveBack") == 0)
  {

    Vect3f l_vPos = m_pObject->GetPosition();
    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x - cos(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z - sin(m_pObject->GetYaw())*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }
    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveLeft") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()+FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }

  if(strcmp(_pcAction, "MoveRight") == 0)
  {
    Vect3f l_vPos = m_pObject->GetPosition();

    if (m_iMode == 0)
    {
      l_vPos.x = l_vPos.x + cos(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      l_vPos.z = l_vPos.z + sin(m_pObject->GetYaw()-FLOAT_PI_VALUE/2)*_fDeltaSeconds*m_fVelocity;
      m_pObject->SetPosition(l_vPos);
    }

    if(m_iState != 1)
      m_bStateChanged = true;

    m_iState = 1;

    return true;
  }


  if(strcmp(_pcAction, "ChangeMode") == 0)
  {
    m_iMode = m_iMode + 1;
    if (m_iMode == 3)
    {
      m_iMode = 0;
      CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
      l_pCam->SetZoom(2.5f);

      CRenderableObject* l_pRenderableObject = m_vAnimatedModels[0];
      CORE->GetRenderableObjectsManager()->SetAllVisible(true,l_pRenderableObject);

      m_pObject->SetPosition(Vect3f(-6,1.7f,0));

      if (m_pObjectBot != 0)
      {
        ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->ClearCycle(0);
        ((CRenderableAnimatedInstanceModel*)m_pObjectBot)->GetAnimatedInstanceModel()->BlendCycle("run",0);
      }

    }
    else 
    {
      m_iMesh = 0;
      m_iAnimat = 0;
      m_iRenderObject = 0;
      
      Vect3f l_pos;

      if (m_iMode == MODE_ANIMATS)
      {
        if (m_vMeshes.size() != 0)
        {
          CRenderableObject* l_pRenderableObject = m_vAnimatedModels[m_iAnimat];
          
          l_pos = l_pRenderableObject->GetPosition();   
          l_pROM->SetAllVisible(false,l_pRenderableObject);

          CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
          float l_fAltura = l_pRenderableObject->m_fAltura;

          l_pCam->SetZoom(l_fAltura*3.0f);
          m_pObject->SetPosition(Vect3f(l_pos.x,l_fAltura/2,l_pos.z));
        }
        else
        {
          m_pObject->SetPosition(Vect3f(0.0f,0.0f,0.0f));
        }

      }
      else if (m_iMode == MODE_MESH)
      {
        if (m_vMeshes.size() != 0)
        {
          CRenderableObject* l_pRenderableObject = m_vMeshes[m_iMesh];

          l_pos = l_pRenderableObject->GetPosition();  
          l_pROM->SetAllVisible(false,l_pRenderableObject);

          CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
          CBoundingBox l_BoundingBox = l_pRenderableObject->GetBoundingBox();

          Vect3f l_vMiddle = l_BoundingBox.MiddlePoint();
          float l_vZoom = l_BoundingBox.GetMaxSideLength();
          m_pObject->SetPosition(l_vMiddle);
          l_pCam->SetZoom(l_vZoom*3.0f);
        }
      }
      
      
    }
    
    return true;
  }


  if(strcmp(_pcAction, "ZoomCamera") == 0)
  {
    if (m_iMode != 0)
    {
      CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
      Vect3i l_vDelta = INPUT_MANAGER->GetMouseDelta();

      if (l_vDelta.z < 0)
      {
        l_pCam->AddZoom(0.3f);
      }
      else
      {
        l_pCam->AddZoom(-0.3f);
      }

    }
    return true;
  }


  if(strcmp(_pcAction, "CanviObjecteDRETA") == 0)
  {
    if (m_iMode == MODE_MESH)
    {
      m_iMesh++;

      if (m_iMesh == m_vMeshes.size())
        m_iMesh = 0;

      if (m_vMeshes.size() != 0)
      {
        CRenderableObject* l_pRenderableObject = m_vMeshes[m_iMesh];

        CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
        CBoundingBox l_BoundingBox = l_pRenderableObject->GetBoundingBox();

        Vect3f l_vMiddle = l_BoundingBox.MiddlePoint();
        float l_vZoom = l_BoundingBox.GetMaxSideLength();
        m_pObject->SetPosition(l_vMiddle);
        l_pCam->SetZoom(l_vZoom*3.0f);


        l_pROM->SetAllVisible(false,l_pRenderableObject);
      }
    }
    else if (m_iMode == MODE_ANIMATS)
    {
      m_iAnimat++;

      if (m_iAnimat == m_vAnimatedModels.size())
        m_iAnimat = 0;

      if (m_vAnimatedModels.size() != 0)
      {
        CRenderableObject* l_pRenderableObject = m_vAnimatedModels[m_iAnimat];

        CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
        float l_fAltura = l_pRenderableObject->m_fAltura;
        Vect3f l_pos = l_pRenderableObject->GetPosition();
     
        l_pCam->SetZoom(l_fAltura*3.0f);
        m_pObject->SetPosition(Vect3f(l_pos.x,l_fAltura/2,l_pos.z));
        l_pROM->SetAllVisible(false,l_pRenderableObject);
      }
    }

    return true;
  }


  if(strcmp(_pcAction, "CanviObjecteESQUERRA") == 0)
  {
    if (m_iMode == MODE_MESH)
    {
      m_iMesh--;

      if (m_iMesh == -1)
        m_iMesh = m_vMeshes.size()-1;


      if (m_vMeshes.size() != 0)
      {
        CRenderableObject* l_pRenderableObject = m_vMeshes[m_iMesh];

        CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
        CBoundingBox l_BoundingBox = l_pRenderableObject->GetBoundingBox();

        Vect3f l_vMiddle = l_BoundingBox.MiddlePoint();
        float l_vZoom = l_BoundingBox.GetMaxSideLength();
        m_pObject->SetPosition(l_vMiddle);
        l_pCam->SetZoom(l_vZoom*3.0f);

        l_pROM->SetAllVisible(false,l_pRenderableObject);
      }
    }
    else if (m_iMode == MODE_ANIMATS)
    {
      m_iAnimat--;

      if (m_iAnimat == -1)
        m_iAnimat = m_vAnimatedModels.size()-1;

      if (m_vAnimatedModels.size() != 0)
      {
        CRenderableObject* l_pRenderableObject = m_vAnimatedModels[m_iAnimat];

        CThPSCamera* l_pCam = (CThPSCamera*) m_pCamera;
        float l_fAltura = l_pRenderableObject->m_fAltura;
        Vect3f l_pos = l_pRenderableObject->GetPosition();
     
        l_pCam->SetZoom(l_fAltura*3.0f);
        m_pObject->SetPosition(Vect3f(l_pos.x,l_fAltura/2,l_pos.z));
        l_pROM->SetAllVisible(false,l_pRenderableObject);
      }
    }

    return true;
  }


  //En el cas que canviem d'animacio!!!!
  if(strcmp(_pcAction, "CanviAnimacioUP") == 0)
  {
    if (m_iMode == MODE_ANIMATS)
    {
      CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
      if (m_vAnimatedModels.size() != 0)
      {
        CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)m_vAnimatedModels[m_iAnimat];
        int l_iNumAnimations = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimationCount();
        int l_iCurrentCycle = l_pRenderModel->GetAnimatedInstanceModel()->GetCurrentCycle();

        l_iCurrentCycle++;

        if (l_iCurrentCycle == l_iNumAnimations)
        {
          l_iCurrentCycle = 0;
        }
        l_pRenderModel->GetAnimatedInstanceModel()->ClearCycle(0);
        l_pRenderModel->GetAnimatedInstanceModel()->BlendCycle(l_iCurrentCycle,0);
      }
    }
    return true;
  }


  if(strcmp(_pcAction, "CanviAnimacioDOWN") == 0)
  {
    if (m_iMode == MODE_ANIMATS)
    {
      CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();
      if (m_vAnimatedModels.size() != 0)
      {
        CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)m_vAnimatedModels[m_iAnimat];
        int l_iNumAnimations = l_pRenderModel->GetAnimatedInstanceModel()->GetAnimationCount();
        int l_iCurrentCycle = l_pRenderModel->GetAnimatedInstanceModel()->GetCurrentCycle();

        l_iCurrentCycle--;

        if (l_iCurrentCycle == -1)
        {
          l_iCurrentCycle = l_iNumAnimations-1;
        }

        l_pRenderModel->GetAnimatedInstanceModel()->ClearCycle(0);
        l_pRenderModel->GetAnimatedInstanceModel()->BlendCycle(l_iCurrentCycle,0);
      }
    }
    return true;
  }


  if(strcmp(_pcAction, "AugmentaAmbient") == 0)
  {

    Vect3f l_vAmbient = CORE->GetLightManager()->GetAmbientLight();
    l_vAmbient = l_vAmbient + 0.05f;
    if (l_vAmbient.x > 1.0f)
    {
      l_vAmbient.x = 1.0f;
      l_vAmbient.y = 1.0f;
      l_vAmbient.z = 1.0f;
    }

    CORE->GetLightManager()->SetAmbientLight(l_vAmbient);
    return true;
  }

  if(strcmp(_pcAction, "DisminueixAmbient") == 0)
  {
    Vect3f l_vAmbient = CORE->GetLightManager()->GetAmbientLight();
    l_vAmbient = l_vAmbient - 0.05f;
    if (l_vAmbient.x < 0.0f)
    {
      l_vAmbient.x = 0.0f;
      l_vAmbient.y = 0.0f;
      l_vAmbient.z = 0.0f;
    }

    CORE->GetLightManager()->SetAmbientLight(l_vAmbient);
    return true;
  }

  if(strcmp(_pcAction, "SetLightsONOFF") == 0)
  {
    m_bVisibleLights = !m_bVisibleLights;
    CORE->GetLightManager()->SetLightsEnabled(m_bVisibleLights);
  }

  if(strcmp(_pcAction, "ShowAjuda") == 0)
  {
    m_bAjuda = !m_bAjuda;
    //CORE->GetLightManager()->SetLightsVisible(m_bVisibleLights);
  }

  return false;
}


void CViewerProcess::RenderINFO(CRenderManager* _pRM)
{
  CRenderableObjectsManager* l_pROM = CORE->GetRenderableObjectsManager();

   //text
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("arial");
  uint32 l_uiFontTypeTitle = FONT_MANAGER->GetTTF_Id("Deco");
  uint32 l_uiFontTypeTitle2 = FONT_MANAGER->GetTTF_Id("xfiles");
  int l_iPosicio = 420;
  int l_iPosicio2 = 400;
  string l_szMsg("Mode Animats");
  string l_szMsg2("Mode Meshes");
  string l_szMsg3("Mode Escena");
  CRenderableObject* l_pAnimatedInstance;
  CRenderableObject* l_pMeshInstance;
  stringstream l_SStream;
  stringstream l_SStreamHelp;

  switch (m_iMode)
  {
    case MODE_ESCENA:
      FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg3.c_str());
      //_pRM->DrawGrid(30.0f,colCYAN,30,30);
      //_pRM->DrawAxis();

      if (m_bAjuda)
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
          
          FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
      }
      break;

    case MODE_MESH:
      if (m_vMeshes.size() != 0)
      {
        FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg2.c_str());
        l_pMeshInstance = m_vMeshes[m_iMesh];
        Vect3f l_vDimension = l_pMeshInstance->GetBoundingBox().GetOriginDimension();

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
      }
      else
      {
        FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
        l_SStream << "No hi ha cap OBJECTE MESH per mostrar" << endl;
        FONT_MANAGER->DrawText(0,l_iPosicio,colGREEN,l_uiFontType,l_SStream.str().c_str());
      }

      if (m_bAjuda)
      {
         
          l_SStreamHelp << "---<Ajuda>---" <<  endl;
          l_SStreamHelp << "[Canvi de Mode] Tecla M" <<  endl;
          l_SStreamHelp << "[Següent Mesh] Fletxa Dreta" <<  endl;
          l_SStreamHelp << "[Anterior Mesh] Fletxa Esquerra" <<  endl;
          l_SStreamHelp << "[Moure Càmera] Ratolí" <<  endl;
          l_SStreamHelp << "[Zoom] Rodeta Ratolí" <<  endl;
          l_SStreamHelp << "[Ocultar Ajuda] F1" <<  endl;
          
          FONT_MANAGER->DrawText(550,l_iPosicio2,colGREEN,l_uiFontType,l_SStreamHelp.str().c_str());
          
         // l_SStream << "Dimensions(XYZ): " << (float)l_vDimension.x << "x";
  
  
  
      }


      break;

    case MODE_ANIMATS:

      if (m_vAnimatedModels.size() != 0)
      {
        FONT_MANAGER->DrawText((uint32)300,(uint32)10,colGREEN,l_uiFontTypeTitle,l_szMsg.c_str());
        l_pAnimatedInstance = m_vAnimatedModels[m_iAnimat];
        CRenderableAnimatedInstanceModel* l_pRenderModel = (CRenderableAnimatedInstanceModel*)m_vAnimatedModels[m_iAnimat];

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

      if (m_bAjuda)
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

      break; 
  }


  

  //}
}



float CViewerProcess::GetZoomMesh(Vect3f _vMax,Vect3f _vCentre)
{
  float l_fZoom;
  
 /* float l_DistX = _vMax.x-_vCentre.x;
  float l_DistY = _vMax.y-_vCentre.y;
  float l_DistZ = _vMax.z-_vCentre.z;
*/

 l_fZoom = _vCentre.Distance(_vMax);
  /*if ((l_DistX > l_DistY) && (l_DistX > l_DistZ))
    l_fZoom = l_DistX;

  if ((l_DistY > l_DistX) && (l_DistY > l_DistZ))
    l_fZoom = l_DistY;

  if ((l_DistZ > l_DistX) && (l_DistZ > l_DistY))
    l_fZoom = l_DistZ;*/

  return (l_fZoom*3);
}