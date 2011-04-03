#include "InputAction.h"
#include "InputTrigger.h"
#include <XML/XMLTreeNode.h>

bool CInputAction::Init(CXMLTreeNode* _pTreeNode)
{

  string l_szName = _pTreeNode->GetPszISOProperty("name","");
  m_szScriptAction = _pTreeNode->GetPszISOProperty("script","");

  if(l_szName.compare("") == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CInputAction::Init Acció sense nom.");
    return false;
  }

  if(m_szScriptAction.compare("") != 0)
  {
    m_bHasScript = true;
  }

  SetName(l_szName);

  LOGGER->AddNewLog(ELL_INFORMATION,"CInputAction::Init  Action \"%s\" script \"%s\"",l_szName.c_str(),m_szScriptAction.c_str());

  int l_iNumTriggers = _pTreeNode->GetNumChildren();
  
  m_vInputTriggers.reserve(l_iNumTriggers);
  
  CInputTrigger* l_pTrigger = 0;

  for(int j = 0; j < l_iNumTriggers; j++)
  {
    CXMLTreeNode l_XMLInput = (*_pTreeNode)(j);

    l_pTrigger = new CInputTrigger();

    if(l_pTrigger->Init(&l_XMLInput))
    {
      m_vInputTriggers.push_back(l_pTrigger);
    }else{
      LOGGER->AddNewLog(ELL_WARNING,"CInputAction::Init Error al carregar trigger %d a l'accio \"%s\"",j+1,l_szName.c_str());
      CHECKED_DELETE(l_pTrigger);
    }
  }

  return true; 
}

void CInputAction::Release()
{
  vector<CInputTrigger*>::iterator l_It;

  for(l_It = m_vInputTriggers.begin(); l_It != m_vInputTriggers.end();l_It++)
  {
    CHECKED_DELETE(*l_It);
  }

  m_vInputTriggers.clear();
}

void CInputAction::Update()
{
  vector<CInputTrigger*>::iterator l_It = m_vInputTriggers.begin();
  m_fDelta = 1.0f;
  
  do
  {
    (*l_It)->Update();
    m_fDelta *= (*l_It)->GetDelta();
    l_It++;
  }while(l_It != m_vInputTriggers.end() && m_fDelta != 0.0f);

}
