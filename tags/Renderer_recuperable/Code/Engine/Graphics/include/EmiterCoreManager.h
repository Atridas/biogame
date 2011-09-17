#pragma once
#ifndef __CORE_EMITER_MANAGER_H__
#define __CORE_EMITER_MANAGER_H__

#include "base.h"
#include "Utils/MapManager.h"
#include "EmiterCore.h"
#include "SimpleEmiterCore.h"
#include "ParticleConstants.h"

// Forward declarations -------------
// ----------------------------------


class CEmiterCoreManager:
  private CMapManager<CEmiterCore>
{
public:
  CEmiterCoreManager() {};
  ~CEmiterCoreManager(void)  {Done();};

  bool Init() {SetOk(m_NullEmiter.Init()); return IsOk();};
  
  bool Load(const string &_szFileName);
  bool Load(const set<string> &_sFiles);
  bool Reload();
  
  const CEmiterCore * GetEmiterCore(const string &_szName) const;
  const CSimpleEmiterCore * GetNullEmiter() const { return &m_NullEmiter; };
  const set<string>& GetCoreNames() const {return m_sCores;};

private:
  bool LoadFile(const string &_szFileName, bool _bReload);

  set<string> m_sFiles;

  set<string> m_sCores;

  CSimpleEmiterCore m_NullEmiter;
};

#endif