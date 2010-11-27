#include "Process.h"
#include "base.h"
#include "Core.h"
#include "Math/Color.h"
#include "FontManager.h"
#include <sstream>

void CProcess::DebugInformation(float _FPS)
{

  if(true)  //TODO: comprovar en una variable de debug
  {
    stringstream l_SStream;
	  CColor col = colBLUE;
    uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
    
    l_SStream << "FPS: " << (int)_FPS << endl;
    string l_szMsg(l_SStream.str());

    FONT_MANAGER->DrawText(0,40,col,l_uiFontType,l_szMsg.c_str());
  }
}


bool CProcess::ExecuteAction(float _fDeltaSeconds, float _fDelta, const string& _szAction)
{
  //TODO fer les accions per defecte del Engine
  return false;
}
bool CProcess::ExecuteScript(float _fDeltaSeconds, float _fDelta, const string& _szScript)
{
  return false;
}