#pragma once
#ifndef __IA_MANAGER__
#define __IA_MANAGER__

#include "base.h"

// forward declarations
class CSparseGraph;
// --------------------

class CIAManager:
  public CBaseControl
{
public:
  CIAManager():m_pGraph(0) {};
  ~CIAManager() {Done();};

  bool Init();

  CSparseGraph* GetGraph() const {return m_pGraph;};

protected:
  virtual void Release();
private:

  CSparseGraph * m_pGraph;
};

#endif