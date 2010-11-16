#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"


bool CTestProcess::Init()
{
  // ---
  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{
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
