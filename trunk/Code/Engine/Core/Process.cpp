#include "Process.h"
#include "base.h"
#include "Core.h"
#include "Math/Color.h"
#include "FontManager.h"
#include <sstream>
#include <Utils/LanguageManager.h>
#include <ActionToInput.h>

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


bool CProcess::ExecuteAction(float _fDeltaSeconds, float _fDelta, const string& _szAction)
{
  //TODO fer les accions per defecte del Engine
  if(_szAction == "ProcessInfo")
  {
    m_bRenderInfo = !m_bRenderInfo;
    return true;
  } else if(_szAction == "SaveLogsInFile")
  {
    LOGGER->SaveLogsInFile();
    return true;
  } else if(_szAction == "ReloadTTFs")
  {
    CORE->GetFontManager()->ReloadTTFs();
    return true;
  } else if(_szAction == "ReloadLanguageXMLs")
  {
    CORE->GetLanguageManager()->LoadXMLs();
    return true;
  } else if(_szAction == "ReloadActions")
  {
    CORE->GetActionToInput()->Reload();
    return true;
  }
  return ExecuteProcessAction(_fDeltaSeconds,_fDelta,_szAction);
}
bool CProcess::ExecuteScript(float _fDeltaSeconds, float _fDelta, const string& _szScript)
{
  return false;
}