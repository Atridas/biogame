#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"


bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---
  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");
	// ----
}

void CTestProcess::Update(float elapsedTime)
{

}

void CTestProcess::Render()
{
  CRenderManager* pRM = CORE->GetRenderManager();
  pRM->DrawAxis();

  /*CColor col = colBLUE;
  CORE->GetFontManager()->DrawText(0,0,col,0,"BioGame");*/
  
}

