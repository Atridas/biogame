#include "GameProcess.h"

void CGameProcess::Update(float elT)
{

}

void CGameProcess::Render()
{

}

bool CGameProcess::Init()
{
  SetOk(true);
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
