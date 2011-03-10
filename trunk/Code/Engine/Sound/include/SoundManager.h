#pragma once

#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include "base.h"

class CSoundManager :
  public CBaseControl
{
public:
  CSoundManager() : m_iMaxChannels(0),m_iMusicChannels(0) {};
  ~CSoundManager() {Done();};

  void Init();
  void Release();

  void PlaySample();
  void PlaySample3D();
  void PlayMusic();
  void PlayMusic3D();
  void StopAll();
  void StopMusics();
  void StopSounds();
  void SetMasterVolume();
  void Pause();
  void Resume();

private:

  struct SChannel
  {
    float m_fVolume;
    int m_iResourceIndex;
  };

  map<string,int> m_mResources;
  vector<SChannel> m_vChannels;

  int m_iMaxChannels;
  int m_iMusicChannels;
};

#endif
