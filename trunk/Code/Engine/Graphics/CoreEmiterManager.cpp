#include <XML\XMLTreeNode.h>
#include "EmiterManager.h"

// Main include
#include "CoreEmiterManager.h"


bool CCoreEmiterManager::LoadFile(const string &_szFileName, bool _bReload)
{
  CXMLTreeNode l_xmlCoreEmiters;

  if(l_xmlCoreEmiters.LoadFile(_szFileName.c_str()))
  {
    if(strcmp("CoreEmiters",l_xmlCoreEmiters.GetName()) != 0)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CCoreEmiterManager::LoadFile Node arrel és \"%s\" i no \"CoreEmiters\"",l_xmlCoreEmiters.GetName());
      return false;
    }

    int l_iNumChildren = l_xmlCoreEmiters.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; ++i)
    {
      CXMLTreeNode l_xmlCoreEmiter = l_xmlCoreEmiters(i);
      if(strcmp("CoreEmiter",l_xmlCoreEmiter.GetName()) == 0)
      {
        CCoreEmiter* l_pCoreEmiter = new CCoreEmiter();
        if(l_pCoreEmiter->Init(l_xmlCoreEmiter))
        {
          if(GetResource(l_pCoreEmiter->GetName()))
          {
            if(_bReload)
            {
              delete GetResource(l_pCoreEmiter->GetName());
              AddResource(l_pCoreEmiter->GetName(), l_pCoreEmiter);
            }
            else
            {
              LOGGER->AddNewLog(ELL_WARNING, "CCoreEmiterManager::LoadFile Core Emiter \"%s\" repetit",l_pCoreEmiter->GetName().c_str());
            }
          }
          else
          {
            AddResource(l_pCoreEmiter->GetName(), l_pCoreEmiter);
            m_sCores.insert(l_pCoreEmiter->GetName());
          }
        }
        else
        {
          delete l_pCoreEmiter;
        }
      }
      else if(!l_xmlCoreEmiter.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING, "CCoreEmiterManager::LoadFile Node \"%s\" desconegut",l_xmlCoreEmiter.GetName());
      }
    }
    return true;
  }
  else
  {
    LOGGER->AddNewLog(ELL_WARNING, "CCoreEmiterManager::LoadFile can not load \"%s\"",_szFileName.c_str());
    return false;
  }
}

bool CCoreEmiterManager::Load(const string &_szFileName)
{
  if(m_sFiles.find(_szFileName) != m_sFiles.end())
  {
    LOGGER->AddNewLog(ELL_INFORMATION, "CCoreEmiterManager::Load \"%s\" ja s'ha carregat prèviament.", _szFileName.c_str());
    return LoadFile(_szFileName, true);
  }
  else
  {
    m_sFiles.insert(_szFileName);
    return LoadFile(_szFileName, false);
  }
}


bool CCoreEmiterManager::Load(const set<string> &_sFiles)
{
  bool l_bOk = true;
  set<string>::iterator l_it  = _sFiles.begin();
  set<string>::iterator l_end = _sFiles.end();

  for(; l_it != l_end; ++l_it)
  {
    if(!Load(*l_it))
      l_bOk = false;
  }
  return l_bOk;
}

bool CCoreEmiterManager::Reload()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CCoreEmiterManager::Reload");

  bool l_bOk = true;
  set<string>::iterator l_it  = m_sFiles.begin();
  set<string>::iterator l_end = m_sFiles.end();

  for(; l_it != l_end; ++l_it)
  {
    if(!LoadFile(*l_it, true))
      l_bOk = false;
  }

  CORE->GetEmiterManager()->ResetEmiters();
  return l_bOk; // False si n'hi ha algun de mal carregat. True en altre cas.
}

CCoreEmiter * CCoreEmiterManager::GetCoreEmiter(const string &_szName)
{
  CCoreEmiter* l_pCoreEmiter = GetResource(_szName);

  if(!l_pCoreEmiter)
  {
    l_pCoreEmiter = &m_NullEmiter;
  }

  return l_pCoreEmiter;
}