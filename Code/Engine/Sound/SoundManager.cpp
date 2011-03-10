#include "SoundManager.h"
#include "bass.h"

void CSoundManager::Init()
{
  BASS_Init(1, 44100, BASS_DEVICE_3D, 0, NULL);
}

void CSoundManager::Release()
{
  BASS_Stop();
  BASS_Free();

  m_vChannels.clear();
  m_mResources.clear();
}

void CSoundManager::PlaySample()
{

}

void CSoundManager::PlaySample3D()
{

}

void CSoundManager::PlayMusic()
{

}

void CSoundManager::PlayMusic3D()
{

}

void CSoundManager::StopAll()
{

}

void CSoundManager::StopMusics()
{

}

void CSoundManager::StopSounds()
{

}

void CSoundManager::SetMasterVolume()
{

}

void CSoundManager::Pause()
{

}

void CSoundManager::Resume()
{

}
