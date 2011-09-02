#pragma once
#ifndef __CORE_AGGREGATE_EMITER_H__
#define __CORE_AGGREGATE_EMITER_H__
#include "base.h"
#include "Named.h"
#include "ParticleConstants.h"
#include "EmiterCore.h"
#include "Utils\Object3D.h"

// ----------------------------
class CXMLTreeNode;
// ----------------------------

class CAggregateEmiterCore:
   public CEmiterCore
{
public:
  struct SEmiters
  {
    CObject3D    movement;
    Vect3f       volume;
    string       emiter;
  };
public:
  CAggregateEmiterCore() {};

  bool Init(CXMLTreeNode& _xmlEmiter);

  const vector<SEmiters>& GetChilds    () const { return m_Childs; };
  const set<string>     & GetChildNames() const { return m_ChildNames; };
      
  virtual bool IsSimpleEmiter() { return false; };
  virtual bool IsAggregateEmiter() { return false; };

protected:
  virtual void Release() {m_Childs.clear();m_ChildNames.clear();};

private:

  vector<SEmiters>     m_Childs;
  set<string>          m_ChildNames;
};


#endif
