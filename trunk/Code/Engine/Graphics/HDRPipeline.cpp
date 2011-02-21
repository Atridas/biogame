#include "HDRPipeline.h"

#include "DrawQuadToTextureSceneEffect.h"
#include "RenderToTextureSceneEffect.h"
#include "DrawQuadSceneEffect.h"


void CHDRPipeline::Release(void)
{
  CHECKED_DELETE(m_pFirstPass);
  CHECKED_DELETE(m_pBrightPass);
  CHECKED_DELETE(m_pDownSample);
  CHECKED_DELETE(m_pFirstBlurPass);
  CHECKED_DELETE(m_pBrightBlured);
  CHECKED_DELETE(m_pRenderEffect);
  
  vector<CDrawQuadToTextureSceneEffect*>::iterator l_it  = m_vLuminancePasses.begin();
  vector<CDrawQuadToTextureSceneEffect*>::iterator l_end = m_vLuminancePasses.end();

  while(l_it != l_end)
  {
    delete (*l_it);
    ++l_it;
  }
}

bool CHDRPipeline::Init(const string& _szXML)
{
  if( IsOk() ) 
  {
    return true;
  } else {
    Release();
    return false;
  }
}
  
void CHDRPipeline::PrepareTextures(CRenderManager* _pRM, CProcess* _pProcess)
{

}

void CHDRPipeline::Render(CRenderManager* _pRM)
{

}