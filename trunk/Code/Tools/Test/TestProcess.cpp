#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"


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

void CTestProcess::Update()
{

}

void CTestProcess::Render()
{
  CRenderManager* pRM = CORE->GetRenderManager();
  pRM->DrawAxis();
}
