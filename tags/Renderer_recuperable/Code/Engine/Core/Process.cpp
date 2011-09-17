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
#include "EmiterCoreManager.h"

void CProcess::DebugInformation(CRenderManager* _pRM)
{
  if(m_bRenderInfo)
  {
    CFontManager* l_pFontManager = CORE->GetFontManager();

    stringstream l_SStream;
	  CColor col = colBLUE;
    CTimer* l_pTimer = CORE->GetTimer();
    uint32 l_uiFontType = l_pFontManager->GetTTF_Id("Titania");
    
    l_SStream << "FPS: " << (float) l_pTimer->GetFPS() << endl;
    
    string l_szMsg(l_SStream.str());

    FONT_MANAGER->DrawText(0,0,col,l_uiFontType,l_szMsg.c_str());

    CLogRender* l_pLR = CORE->GetLogRender();
    if(l_pLR)
      l_pLR->Render(_pRM,l_pFontManager);

    CConsole* l_pC = CORE->GetConsole();
    if(l_pC)
      l_pC->Render(_pRM,l_pFontManager);

    RenderINFO(_pRM);
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
    CORE->GetEmiterCoreManager()->Reload();
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
