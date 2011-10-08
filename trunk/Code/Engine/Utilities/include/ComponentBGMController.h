#pragma once
#ifndef __COMPONENT_BGM__
#define __COMPONENT_BGM__

#include "base.h"
#include "EntityDefines.h"

class CComponentBGMController:
  public CBaseComponent
{
public:

  enum EMusicState {
    EMS_INIT_MENU = 0,
    EMS_MAIN_MENU,
    EMS_INIT_LEVEL,
    EMS_EXPLORE,
    EMS_BATTLE,
    EMS_BATTLE_END,
    EMS_BATTLE_TO_EXPLORE,
    EMS_RR
  };

  ~CComponentBGMController() {Done();}

  CBaseComponent::Type GetType() {return CBaseComponent::ECT_BGM;};
  static CBaseComponent::Type GetStaticType() {return CBaseComponent::ECT_BGM;};

  static CComponentBGMController* AddToEntity(CGameEntity* _pEntity, bool _bMain);

  void SetCurrentSong(const EMusicState _eState, bool _bReset = true);

  void Update(float _fDeltaTime);

  virtual void ReceiveEvent(const SEvent& _Event);
  
  virtual bool UpdateInPause() const {return true;};
protected:

  CComponentBGMController(): m_iAliveEnemy(0), m_eCurrentMusic(CComponentBGMController::EMS_MAIN_MENU), m_szCurrentMusic(""), m_bPlaying(false) {};
  bool Init(CGameEntity* _pEntity, bool _bMain);
  virtual void Release();

private:
  string GetSongInfo(const EMusicState _eState) const;
  EMusicState GetNextSong() const;
  void PlayNextSong(const EMusicState _eState);

  virtual void Enable();
  virtual void Disable();

  int m_iAliveEnemy;
  EMusicState m_eCurrentMusic;
  string m_szCurrentMusic;
  bool m_bPlaying;
};

#endif