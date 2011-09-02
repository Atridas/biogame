#include <sstream>
#include "Process.h"
#include "Core.h"
#include "ActionManager.h"
#include "Camera.h"
#include "Utils/Timer.h"

#include "Utils/LanguageManager.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "RenderableObjectsManager.h"
#include "SceneEffectManager.h"
#include "ScriptManager.h"
#include "LogRender.h"
#include "Console.h"
#include "CoreEmiterManager.h"

void CProcess::DebugInformation()
{
  if(m_bRenderInfo)
  {
    stringstream l_SStream;
	  CColor col = colBLUE;
    CTimer* l_pTimer = CORE->GetTimer();
    uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("Titania");
    
    l_SStream << "FPS: " << (float) l_pTimer->GetFPS() << endl;
    //l_SStream << "Time: " << (float) l_pTimer->GetRelativeTime() << "s" << endl;
    //l_SStream << "Faces: " << CORE->GetRenderableObjectsManager()->GetActiveFaces() << endl;
    //l_SStream << "Primitives: " << CORE->GetRenderableObjectsManager()->GetActivePrimitiveCalls() << endl;
    
    string l_szMsg(l_SStream.str());

    //FONT_MANAGER->DrawText(0,12,colRED,l_uiFontType,m_szProcessName.c_str());
    FONT_MANAGER->DrawText(0,0,col,l_uiFontType,l_szMsg.c_str());


    RenderINFO(RENDER_MANAGER);
  }
}


bool CProcess::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  //TODO fer les accions per defecte del Engine
  if(strcmp(_pcAction, "ProcessInfo") == 0)
  {
    m_bRenderInfo = !m_bRenderInfo;
    return true;
  } else if(strcmp(_pcAction, "SaveLogsInFile") == 0)
  {
    LOGGER->SaveLogsInFile();
    return true;
  } else if(strcmp(_pcAction, "ReloadTTFs") == 0)
  {
    CORE->GetFontManager()->ReloadTTFs();
    return true;
  } else if(strcmp(_pcAction, "ReloadLanguageXMLs") == 0)
  {
    CORE->GetLanguageManager()->LoadXMLs();
    return true;
  } else if(strcmp(_pcAction, "ReloadActions") == 0)
  {
    CORE->GetActionManager()->Reload();
    //CORE->GetRenderableObjectsManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "ReloadShaders") == 0)
  {
    CORE->GetEffectManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "ReloadMeshes") == 0)
  {
    CORE->GetRenderableObjectsManager()->Reload();
    return true;
  }else if(strcmp("ReloadParticles",_pcAction) == 0)
  {
    CORE->GetCoreEmiterManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "ReloadScript") == 0)
  {
    CORE->GetScriptManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "LogRender") == 0)
  {
    CORE->GetLogRender()->Toggle();
    return true;
  } else if(strcmp(_pcAction, "LogRender_PageDown") == 0)
  {
    CORE->GetLogRender()->PageDown();
    return true;
  } else if(strcmp(_pcAction, "LogRender_PageUp") == 0)
  {
    CORE->GetLogRender()->PageUp();
    return true;
  } else if(strcmp(_pcAction, "LogRender_PrevLine") == 0)
  {
    CORE->GetLogRender()->PrevLine();
    return true;
  } else if(strcmp(_pcAction, "LogRender_NextLine") == 0)
  {
    CORE->GetLogRender()->NextLine();
    return true;

  } else if(strcmp(_pcAction, "Console") == 0)
  {
    CORE->GetConsole()->Toggle();
    return true;
  }
  return ExecuteProcessAction(_fDeltaSeconds,_fDelta,_pcAction);
}

bool CProcess::ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript)
{
  CORE->GetScriptManager()->RunCode(_pcScript);
  return true;
}


void CProcess::PreRender(CRenderManager* _pRM)
{
  if(m_pSceneEffectManager)
  {
    m_pSceneEffectManager->PreRender(_pRM, this);
  }
  //C�digo de Prerender si necesit�semos hacer el pre-render de la GUI
}

void CProcess::Render(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();

  if(m_pSceneEffectManager)
  {
    m_pSceneEffectManager->ActivateRenderSceneEffects();
  }

  if(l_pEM)
  {
    l_pEM->SetForcedAnimatedModelEffect(m_pAnimatedMeshEffect);
    l_pEM->SetForcedStaticMeshEffect(m_pStaticMeshEffect);
  }

  //Renderizamos la escena
  RenderScene(_pRM);

  
}

void CProcess::PostRender(CRenderManager* _pRM, bool _bCaptureNoPostFX)
{
  if(m_pSceneEffectManager)
  {
    if(_bCaptureNoPostFX)
    {
      //Capturamos el Frame Buffer antes de los efectos de post render
      m_pSceneEffectManager->CaptureFrameBuffers(_pRM);
    }
    //Efectuamos los efectos de post render
    m_pSceneEffectManager->PostRender(_pRM);
    //Capturamos el Frame Buffer despu�s de los efectos de post render
    m_pSceneEffectManager->CaptureFrameBuffersAfterPostRender(_pRM);

    m_pSceneEffectManager->WarpRender(_pRM);

    m_pSceneEffectManager->GUIRender(_pRM);
  }
}
