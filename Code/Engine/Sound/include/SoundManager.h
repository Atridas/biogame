#pragma once

#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include "base.h"

class CSoundManager :
  public CBaseControl
{
public:
  CSoundManager() : m_iCurrentMusic(0) {};
  ~CSoundManager() {Done();};

  bool Init(const string& _szFile);
  void Release();

  void PlaySample(const string& _szSample);
  void PlaySample3D(const string& _szSample, Vect3f _vPosition);
  void ChangeMusic(const string& _szMusic, unsigned long _ulFadeOutTimeMs, bool _bRestart = true);
  void PlayMusic(const string& _szMusic, bool _bRestart = true);
  void SetMusic3DPosition(const string& _szMusic, const Vect3f& _vSoundEmmiterPosition);
  void UpdateSound3DSystem(const Vect3f& _vListenerPosition, const Vect3f& _vListenerDirection);
  void StopAll();
  void StopMusics();
  void StopSounds();
  void SetMasterVolume(float _fVolume);
  void Pause(const string& _szMusic);
  void Resume(const string& _szMusic);

private:

  struct SSoundChannel
  {
    float m_fVolume;
    unsigned long m_iHandle;

    SSoundChannel(unsigned long _iChannel, float _fVolume) : m_iHandle(_iChannel), m_fVolume(_fVolume) {};
  };

  SSoundChannel* GetSample(const string& _szSample);
  SSoundChannel* GetMusic(const string& _szMusic);

  map<string,SSoundChannel*> m_mapSamples;
  map<string,SSoundChannel*> m_mapMusics;

  vector<SSoundChannel*> m_vMusics;
  vector<SSoundChannel*> m_vSamples;

  int m_iCurrentMusic;

  //int m_iCurrentSample;
  //int m_iMaxChannels;
  //int m_iMusicChannels;
};

#endif
