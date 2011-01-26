#include <sstream>
#include "Process.h"
#include "Core.h"
#include "ActionToInput.h"
#include "Camera.h"
#include "Utils/Timer.h"

#include "Utils/LanguageManager.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "RenderableObjectsManager.h"
#include "SceneEffectManager.h"

void CProcess::DebugInformation()
{
  if(m_bRenderInfo)  //TODO: comprovar en una variable de debug
  {
    stringstream l_SStream;
	  CColor col = colBLUE;
    CTimer* l_pTimer = CORE->GetTimer();
    uint32 l_uiFontType = FONT_MANAGER->GetTTF_Id("xfiles");
    
    l_SStream << "Total:     " << (float) l_pTimer->GetTotalTime() << "s" << endl;
    l_SStream << "Relative:  " << (float) l_pTimer->GetRelativeTime() << "s" << endl;
    l_SStream << "FPS:       " << (int) l_pTimer->GetFPS() << endl;
    string l_szMsg(l_SStream.str());

    FONT_MANAGER->DrawText(0,40,col,l_uiFontType,m_szProcessName.c_str());
    FONT_MANAGER->DrawText(0,65,col,l_uiFontType,l_szMsg.c_str());
  }
}


bool CProcess::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  //TODO fer les accions per defecte del Engine
  if(strcmp(_pcAction, "ProcessInfo") == 0)
  {
    m_bRenderInfo = !m_bRenderInfo;
    return true;
  } else if(strcmp(_pcAction, "SaveLogsInFile") == 0)
  {
    LOGGER->SaveLogsInFile();
    return true;
  } else if(strcmp(_pcAction, "ReloadTTFs") == 0)
  {
    CORE->GetFontManager()->ReloadTTFs();
    return true;
  } else if(strcmp(_pcAction, "ReloadLanguageXMLs") == 0)
  {
    CORE->GetLanguageManager()->LoadXMLs();
    return true;
  } else if(strcmp(_pcAction, "ReloadActions") == 0)
  {
    CORE->GetActionToInput()->Reload();
    CORE->GetRenderableObjectsManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "ReloadShaders") == 0)
  {
    RENDER_MANAGER->GetEffectManager()->Reload();
    return true;
  } else if(strcmp(_pcAction, "ReloadMeshes") == 0)
  {
    CORE->GetRenderableObjectsManager()->Reload();
    return true;
  }
  return ExecuteProcessAction(_fDeltaSeconds,_fDelta,_pcAction);
}

bool CProcess::ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript)
{
  return false;
}


void CProcess::PreRender(CRenderManager* _pRM)
{
  //m_pSceneEffectManager->PreRender(_pRM, this);
  //Código de Prerender si necesitásemos hacer el pre-render de la GUI
}

//TODO: uncomment when ready
void CProcess::Render(CRenderManager* _pRM)
{
  CEffectManager* l_pEM = _pRM->GetEffectManager();

  m_pSceneEffectManager->ActivateRenderSceneEffects();

  //que usen la technique por defecto del tipo de vértice
  l_pEM->SetAnimatedModelTechnique(NULL);
  l_pEM->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(_pRM);

  //Capturamos el Frame Buffer antes de los efectos de post render
  //m_pSceneEffectManager->CaptureFrameBuffers(_pRM);
  //Efectuamos los efectos de post render
  m_pSceneEffectManager->PostRender(_pRM);
  //
  //m_pSceneEffectManager->CaptureFrameBuffersAfterPostRender(_pRM);
  
}