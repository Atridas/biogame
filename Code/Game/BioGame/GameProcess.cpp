#include "GameProcess.h"
#include "RenderManager.h"

#include "Core.h"

void CGameProcess::Update(float elT)
{

}

void CGameProcess::RenderScene(CRenderManager* _pRM)
{

}

bool CGameProcess::Init()
{
  SetOk(true);

  m_pSceneEffectManager = CORE->GetSceneEffectManager();

  return IsOk();
}

void CGameProcess::Release()
{
	// ----
}

bool CGameProcess::ExecuteProcessAction(float _fDeltaSeconds, float _fDelta, const char* _szAction)
{
	return false;
}
