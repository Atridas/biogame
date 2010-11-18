#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"


bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---

  m_vPos = Vect2f(-150,0);

  SetOk(true);
  return IsOk();
}

void CTestProcess::Release()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Release");
	// ----
}

void CTestProcess::Update(float _fElapsedTime)
{
  float l_fVelX = 1100;
  
  m_vPos.x += l_fVelX*_fElapsedTime;

  if(m_vPos.x > RENDER_MANAGER->GetScreenWidth())
    m_vPos.x = -150;

}

void CTestProcess::Render()
{
  CColor col = colBLUE;
  CColor l_CubeCol = colWHITE;

  CRenderManager* pRM = RENDER_MANAGER;

  pRM->DrawAxis();
  pRM->DrawCube(Vect3f(0.0f,0.0f,0.0f),1.0f,l_CubeCol);
  
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Biogame");

  FONT_MANAGER->DrawText((uint32)m_vPos.x,(uint32)m_vPos.y,col,l_uiFontType,l_szMsg.c_str());
  //FONT_MANAGER->DrawText(0,0,col,l_uiFontType,l_szMsg.c_str());
  
}

