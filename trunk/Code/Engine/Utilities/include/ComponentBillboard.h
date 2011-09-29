#pragma once
#ifndef __COMPONENT_BILLBOARD__
#define __COMPONENT_BILLBOARD__

#include "base.h"
#include "EntityDefines.h"

class CEmiterInstance;

class CComponentBillboard:
  public CBaseComponent
{
public:

  ~CComponentBillboard() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_BILLBOARD;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_BILLBOARD;};

  static CComponentBillboard* AddToEntity(CGameEntity* _pEntity, const string& _szCore);
  
  void ChangeCore(const string& _szCore);

  CEmiterInstance* GetBillboard() const {return m_pEmiterInstance;};

protected:

  CComponentBillboard():m_pEmiterInstance(0) {};
  bool Init(CGameEntity* _pEntity, const string& _szCore);
  virtual void Release();

private:

  CEmiterInstance* m_pEmiterInstance;
  string           m_szEmiterName;
};

#endif
