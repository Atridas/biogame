#pragma once
#ifndef __CORE_EMITER_MANAGER_H__
#define __CORE_EMITER_MANAGER_H__

#include "base.h"
#include "Utils/MapManager.h"
#include "CoreEmiter.h"
#include "ParticleConstants.h"

// Forward declarations -------------
// ----------------------------------


class CCoreEmiterManager:
  private CMapManager<CCoreEmiter>
{
public:
  CCoreEmiterManager() {m_NullEmiter.Init(); SetOk(true);};
  ~CCoreEmiterManager(void)  {Done();};
  
  bool Load(const string &_szFileName);
  bool Load(const set<string> &_sFiles);
  bool Reload();

  CCoreEmiter * GetCoreEmiter(const string &_szName);
  const set<string>& GetCoreNames() const {return m_sCores;};

private:
  bool LoadFile(const string &_szFileName, bool _bReload);

  set<string> m_sFiles;

  set<string> m_sCores;

  CCoreEmiter m_NullEmiter;
};

#endif