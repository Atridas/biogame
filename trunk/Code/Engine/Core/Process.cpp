#include "Process.h"
#include "base.h"
#include "Core.h"
#include "Math/Color.h"
#include "FontManager.h"

void CProcess::DebugInformation(float _FPS)
{
	CColor col = colBLUE;
  CORE->GetFontManager()->DrawText(0,0,col,CORE->GetFontManager()->GetTTF_Id("xfiles"),"FPS: %.f",_FPS);
}


