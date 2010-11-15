#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"


bool CTestProcess::Init()
{
  // ---

  return m_bIsOk = true;
}

void CTestProcess::Relase()
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
