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

  void PlaySample(const string& _szSample);
  void PlaySample3D();
  void PlayMusic(const string& _szSample);
  void PlayMusic3D();
  void StopAll();
  void StopMusics();
  void StopSounds();
  void SetMasterVolume();
  void Pause();
  void Resume();

private:

  struct SMusicChannel
  {
    float m_fVolume;
    unsigned long m_iChannel;

    SMusicChannel(unsigned long _iChannel, float _fVolume) : m_iChannel(_iChannel), m_fVolume(_fVolume) {};
  };

  unsigned long GetSample(const string& _szSample);

  map<string,unsigned long> m_mResources;
  vector<SMusicChannel> m_vMusicChannels;

  int m_iCurrentMusic;

  //int m_iCurrentSample;
  //int m_iMaxChannels;
  //int m_iMusicChannels;
};

#endif
