#include "Process.h"
#include "base.h"
#include "Core.h"
#include "Math/Color.h"
#include "FontManager.h"
#include <sstream>
#include <Utils/LanguageManager.h>
#include <ActionToInput.h>
#include <RenderableObjectsManager.h>

void CProcess::DebugInformation(float _FPS)
{

  if(m_bRenderInfo)  //TODO: comprovar en una variable de debug
  {
    stringstream l_SStream;
	  CColor col = colBLUE;
    uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
    
    l_SStream << "FPS: " << (int)_FPS << endl;
    string l_szMsg(l_SStream.str());
    
    FONT_MANAGER->DrawText(0,40,col,l_uiFontType,l_szMsg.c_str());
    FONT_MANAGER->DrawText(0,65,col,l_uiFontType,m_szProcessName.c_str());
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
    CORE->GetActionToInput()->Reload();
    CORE->GetRenderableObjectsManager()->Reload();
    return true;
  }
  return ExecuteProcessAction(_fDeltaSeconds,_fDelta,_pcAction);
}
bool CProcess::ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript)
{
  return false;
}