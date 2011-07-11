#pragma once
#ifndef __COMPONENT_Door__
#define __COMPONENT_Door__

#include "base.h"
#include "EntityDefines.h"

class CComponentDoor:
  public CBaseComponent
{
public:

  ~CComponentDoor() {Done();}
  
  CBaseComponent::Type GetType() {return CBaseComponent::ECT_DOOR;};
  static CComponentDoor::Type GetStaticType() {return CBaseComponent::ECT_DOOR;};

  static CComponentDoor* AddToEntity(CGameEntity *_pEntity, bool _bOpen);

  bool Open();
  bool Close();
  bool IsOpen()            { return m_bOpen; };
  void Block(bool _bBlock) { m_bBlock = _bBlock; };

  float m_fTime;

protected:
  virtual void Release() {};

private:
  bool Init(CGameEntity* _pEntity, bool _bOpen);
  CComponentDoor(): m_bOpen(false), m_bBlock(false), m_fTime(0.0f) {};

  bool m_bOpen;
  bool m_bBlock;

};

#endif