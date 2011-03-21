#include "ActionManager.h"
#include <XML/XMLTreeNode.h>
#include "Process.h"

bool CActionManager::Init(string _szXMLFile)
{
  m_szXMLFile = _szXMLFile;
  
  SetOk(true);

  LOGGER->AddNewLog(ELL_INFORMATION,"CActionToInput::Init");

  CXMLTreeNode l_XMLActions;
  if(!l_XMLActions.LoadFile(m_szXMLFile.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CActionToInput::Load No s'ha trobat el XML \"%s\"", m_szXMLFile.c_str());
    SetOk(false);
    return IsOk();
  }
  
  int l_iNumActions = l_XMLActions.GetNumChildren();

  for(int i = 0; i < l_iNumActions; i++)
  {
    CXMLTreeNode l_XMLAction = l_XMLActions(i);
  
    if(strcmp(l_XMLAction.GetName(),"action") != 0)
    {
      if(!l_XMLAction.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING,"CInputAction::Init Error de format a l'xml, hi ha un element invàlid \"%s\"", l_XMLAction.GetName());
        return false;
      }

    }else{
      CInputAction* l_pAction = new CInputAction();
      
      if(l_pAction->Init(&l_XMLAction))
      {
        AddResource(l_pAction->GetName(),l_pAction);
      }else{
        CHECKED_DELETE(l_pAction);
      }
    }
  }

  return IsOk();
}

void CActionManager::Release()
{
  CMapManager::Release();
}

void CActionManager::Update (float _fDeltaSeconds)
{
  TMapResource::iterator l_It = m_Resources.begin();
  TMapResource::iterator l_End = m_Resources.end();
  while(l_It != l_End) 
  {
    CInputAction* l_pAction = (l_It->second);
    l_pAction->Update();
    if(l_pAction->IsTriggered())
    {
      if(l_pAction->HasName())
      {
        ExecuteAction(_fDeltaSeconds,l_pAction->GetDelta(),l_pAction->GetName().c_str());
      }

      if(l_pAction->HasScript())
      {
        ExecuteScript(_fDeltaSeconds,l_pAction->GetDelta(),l_pAction->GetScriptAction().c_str());
      }
    }
    ++l_It;
  }
}

void CActionManager::ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction)
{
  if(m_pProcess)
  {
    if(m_pProcess->ExecuteAction(_fDeltaSeconds, _fDelta, _pcAction))
      return;
  }
}

void CActionManager::ExecuteScript(float _fDeltaSeconds, float _fDelta, const char* _pcScript)
{
  if(m_pProcess)
  {
    if(m_pProcess->ExecuteScript(_fDeltaSeconds, _fDelta, _pcScript))
      return;
  }
}

