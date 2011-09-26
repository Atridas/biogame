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
    LOGGER->AddNewLog(ELL_WARNING,"CActionToInput::Load No s'ha pogut carregar l'XML \"%s\"", m_szXMLFile.c_str());
    SetOk(false);
    return IsOk();
  }
  
  int l_iNumActions = l_XMLActions.GetNumChildren();

  CInputAction* l_pAction = 0;

  for(int i = 0; i < l_iNumActions; i++)
  {
    CXMLTreeNode l_XMLAction = l_XMLActions(i);
  
    if(strcmp(l_XMLAction.GetName(),"action") != 0)
    {
      if(!l_XMLAction.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING,"CInputAction::Init Error de format a l'xml, hi ha un element invàlid \"%s\"", l_XMLAction.GetName());
      }

    }else{
      l_pAction = new CInputAction();
      
      if(l_pAction->Init(&l_XMLAction))
      {
        if(!GetResource(l_pAction->GetName()))
        {
          AddResource(l_pAction->GetName(),l_pAction);
        }else{
          LOGGER->AddNewLog(ELL_WARNING,"CInputAction::Init Error accio repetida \"%s\"", l_pAction->GetName().c_str());
          CHECKED_DELETE(l_pAction);
        }
        
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
  if(m_bReload)
  {
    Done();
    Init(m_szXMLFile);
    m_bReload = false;
  }

  TMapResource::iterator l_It = m_Resources.begin();
  TMapResource::iterator l_End = m_Resources.end();
  while(l_It != l_End) 
  {
    CInputAction* l_pAction = (l_It->second);
    l_pAction->Update();
    if(l_pAction->IsTriggered())
    {
      ExecuteAction(_fDeltaSeconds,l_pAction->GetDelta(),l_pAction->GetName().c_str());

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

bool CActionManager::IsActionActive(const string& _szActionName)
{
  CInputAction* l_pInputAction = GetResource(_szActionName);

  if(l_pInputAction)
  {
    return l_pInputAction->IsTriggered();
  }

  return false;
}

