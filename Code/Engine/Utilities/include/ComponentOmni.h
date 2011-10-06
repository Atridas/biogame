#pragma once
#ifndef __COMPONENT_OMNI__
#define __COMPONENT_OMNI__

#include "base.h"
#include "EntityDefines.h"

class COmniLight;
class CComponentObject3D;

class CComponentOmni:
  public CBaseComponent
{
public:

  ~CComponentOmni() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_OMNI;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_OMNI;};

  static CComponentOmni* AddToEntity(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt, const string& _szScript = "");
  static CComponentOmni* AddToEntityFromResource(CGameEntity* _pEntity, const string& _szResource, const string& _szScript);

  void Update(float _fDeltaTime);

  COmniLight* GetOmniLight();

protected:

  CComponentOmni(): m_bFromResource(false), m_szScript(""), m_pOmni(0), m_pObject3D(0) {};
  bool Init(CGameEntity* _pEntity, const Vect3f& _vOffsetPosition, const CColor& _vColor, float _fStartRangeAtt, float _fEndRangeAtt, const string& _szScript);
  bool InitFromResource(CGameEntity* _pEntity, const string& _szResource, const string& _szScript);
  virtual void Release();

  virtual void Enable(void);
  virtual void Disable(void);

private:
  void RunScript(float _fDeltaTime);

  bool                m_bFromResource;

  string              m_szScript;

  COmniLight*         m_pOmni;
  CComponentObject3D* m_pObject3D;
  Vect3f              m_vOffset;
};

#endif