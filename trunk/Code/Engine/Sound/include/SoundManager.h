#pragma once

#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include "base.h"

//TODO: Usar MapManager.
class CSoundManager :
  public CBaseControl
{
public:
  CSoundManager() : m_iCurrentMusic(0) {};
  ~CSoundManager() {Done();};

  bool Init(const string& _szFile);
  void Release();

  void PlaySample(const string& _szSample, float _fVolume = 1.0f);
  void PlaySample3D();
  void PlayMusic(const string& _szSample, bool _bRestart = true, float _fVolume = 1.0f);
  void PlayMusic3D();
  void StopAll();
  void StopMusics();
  void StopSounds();
  void SetMasterVolume();
  void Pause(const string& _szSample);
  void Resume(const string& _szSample);

private:

  struct SSoundChannel
  {
    float m_fVolume;
    unsigned long m_iHandle;

    SSoundChannel(unsigned long _iChannel, float _fVolume) : m_iHandle(_iChannel), m_fVolume(_fVolume) {};
  };

  unsigned long GetSample(const string& _szSample);

  map<string,unsigned long> m_mResources;
  vector<SSoundChannel> m_vMusicChannels;
  vector<SSoundChannel> m_vSampleChannels;

  int m_iCurrentMusic;

  //int m_iCurrentSample;
  //int m_iMaxChannels;
  //int m_iMusicChannels;
};

#endif
