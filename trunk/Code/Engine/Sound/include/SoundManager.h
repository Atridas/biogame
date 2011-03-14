#pragma once

#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include "base.h"

//TODO: Usar MapManager.
class CSoundManager :
  public CBaseControl
{
public:
  CSoundManager() : m_iMaxChannels(0),m_iMusicChannels(0),m_iCurrentMusic(0),m_iCurrentSample(0) {};
  ~CSoundManager() {Done();};

  bool Init(const string& _szFile);
  void Release();

  void PlaySample(const string& _szSample);
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
    unsigned long m_iResourceIndex;

    SChannel(unsigned long _iResourceIndex, float _fVolume) : m_iResourceIndex(_iResourceIndex), m_fVolume(_fVolume) {};
  };

  unsigned long GetSample(const string& _szSample);

  map<string,unsigned long> m_mResources;
  vector<SChannel> m_vChannels;

  int m_iCurrentMusic;
  int m_iCurrentSample;
  int m_iMaxChannels;
  int m_iMusicChannels;
};

#endif
