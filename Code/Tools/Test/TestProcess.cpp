#include "TestProcess.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"


bool CTestProcess::Init()
{
  LOGGER->AddNewLog(ELL_INFORMATION,"TestProcess::Init");
  // ---

  m_vPos = Vect2f(-150,0);
  m_fRotation = 0;

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
  float l_fVelX = 400;
  m_fRotation += 1.f * _fElapsedTime;

  while(m_fRotation > 2.f*FLOAT_PI_VALUE) {
    m_fRotation -= 2.f*FLOAT_PI_VALUE;
  }
  
  m_vPos.x += l_fVelX*_fElapsedTime;

  if(m_vPos.x > RENDER_MANAGER->GetScreenWidth())
    m_vPos.x = -150;

}

void CTestProcess::Render()
{
  CColor col = colBLUE;
  CColor l_CubeCol = colWHITE;

  CRenderManager* pRM = RENDER_MANAGER;

  Mat44f i;
  i.SetIdentity();
  pRM->SetTransform(i);
  pRM->DrawAxis();

  
  Mat44f r, t, s, r2, total;
  r.SetIdentity();
  r2.SetIdentity();
  t.SetIdentity();
  s.SetIdentity();

  t.Translate(Vect3f(0,0.8f,0));
  r.RotByAnglesYXZ(0.f, m_fRotation, 0.f);
  r2.RotByAnglesYXZ(0.f, 0, m_fRotation);
  s.Scale(0.3f,0.3f,0.3f);
  total = r*t*r2*s;

  pRM->SetTransform(total);

  pRM->DrawCube(1.0f,l_CubeCol);
  
  uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
  string l_szMsg("Biogame");

  FONT_MANAGER->DrawText((uint32)m_vPos.x,(uint32)m_vPos.y,col,l_uiFontType,l_szMsg.c_str());
  //FONT_MANAGER->DrawText(0,0,col,l_uiFontType,l_szMsg.c_str());
  
}

