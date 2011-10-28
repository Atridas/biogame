#include "SoundManager.h"
#include "bass.h"
#include <XML/XMLTreeNode.h>

bool CSoundManager::Init(const string& _szFile)
{
  if(!BASS_Init(1, 44100, BASS_DEVICE_3D, 0, NULL))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSoundManager::Init Error al inicialitzar la BASS lib");
    SetOk(true);
    return IsOk();
  }

  BASS_Set3DFactors(1.0, 1.0, 1.0);

  CXMLTreeNode l_XMLSounds;
  if(!l_XMLSounds.LoadFile(_szFile.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init No s'ha trobat el XML \"%s\"", _szFile.c_str());
    SetOk(false);
    return false;
  }

  int l_iNumSounds = l_XMLSounds.GetNumChildren();
  for(int i = 0; i < l_iNumSounds; i++)
  {
    CXMLTreeNode l_XMLSound = l_XMLSounds(i);
    if(l_XMLSound.IsComment()) {
      continue;
    }

    string l_szName = "";
    string l_szFile = "";
    string l_szType = "";
    bool l_bSound3D = false;
    bool l_bLoop = false;
    int l_iPriority = 0;
    float l_fVolume = 0.0f;
    float l_fMinDistance = -1.0f;
    float l_fMaxDistance = -1.0f;

    l_szName = l_XMLSound.GetPszISOProperty("name" ,"");
    l_szFile = l_XMLSound.GetPszISOProperty("file" ,"");
    l_szType = l_XMLSound.GetPszISOProperty("type" ,"");
    l_bSound3D = l_XMLSound.GetBoolProperty("sound3D");
    l_bLoop = l_XMLSound.GetBoolProperty("loop");
    l_iPriority = l_XMLSound.GetIntProperty("priority");
    l_fVolume = l_XMLSound.GetFloatProperty("volume",1.0f);
    l_fMinDistance = l_XMLSound.GetFloatProperty("minDistance",-1.0f);
    l_fMaxDistance = l_XMLSound.GetFloatProperty("maxDistance",-1.0f);

    if(l_fVolume > 1.0f)
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init Invalid volume value in \"%s\"", l_szName.c_str());
      l_fVolume = 1.0f;
    }

    if(l_fVolume < 0.0f)
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init Invalid volume value in \"%s\"", l_szName.c_str());
      l_fVolume = 0.0f;
    }

    int l_iMask = 0;

    if(l_bLoop)
      l_iMask |= BASS_SAMPLE_LOOP;

    if(l_bSound3D)
      l_iMask |= BASS_SAMPLE_3D;

    HSTREAM l_iStream = 0;

    SSoundChannel* l_pSound = 0;

    if(l_szType.compare("music") == 0)
    {
      map<string,SSoundChannel*>::iterator l_It = m_mapMusics.find(l_szName);
      if(l_It == m_mapMusics.end()) 
      {
        l_iStream = BASS_StreamCreateFile(false,l_szFile.c_str(), 0, 0, l_iMask);

        if(l_iStream)
        {
          if(l_bSound3D)
          {
            BASS_ChannelSet3DAttributes(l_iStream,BASS_3DMODE_NORMAL,l_fMinDistance,l_fMaxDistance,360,360,0.0f);
          }

          l_pSound = new SSoundChannel(l_iStream,l_fVolume);
          m_mapMusics[l_szName] = l_pSound;
          m_vMusics.push_back(l_pSound);

        }else{
          LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init error al crear stream: \"%s\"", l_szFile.c_str());
        }
      }else{
        LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init Musica repetida: \"%s\"", l_szName.c_str());
      }
    }else if(l_szType.compare("sample") == 0)
    {
      map<string,SSoundChannel*>::iterator l_It = m_mapSamples.find(l_szName);
      if(l_It == m_mapSamples.end()) 
      {
        int l_iMaxSamples = l_XMLSound.GetIntProperty("maxSamples",1);
        l_iStream = BASS_SampleLoad(false,l_szFile.c_str(), 0, 0, l_iMaxSamples, l_iMask);

        if(l_iStream)
        {
          if(l_bSound3D)
          {
            BASS_ChannelSet3DAttributes(l_iStream,BASS_3DMODE_NORMAL,l_fMinDistance,l_fMaxDistance,360,360,0.0f);
          }

          l_pSound = new SSoundChannel(l_iStream,l_fVolume);
          m_mapSamples[l_szName] = l_pSound;
          m_vSamples.push_back(l_pSound);

        }else{
          LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init error al crear sample: \"%s\"", l_szFile.c_str());
        }
      }else{
        LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init Sampler repetit: \"%s\"", l_szName.c_str());
      }
    }
  }

  SetOk(true);

  return IsOk();
}

void CSoundManager::Release()
{
  BASS_Stop();
  BASS_Free();

  vector<SSoundChannel*>::iterator l_It = m_vMusics.begin();
  vector<SSoundChannel*>::iterator l_ItEnd = m_vMusics.end();

  while(l_It != l_ItEnd)
  {
    CHECKED_DELETE(*l_It);
    ++l_It;
  }

  m_mapMusics.clear();
  m_vMusics.clear();

  l_It = m_vSamples.begin();
  l_ItEnd = m_vSamples.end();

  while(l_It != l_ItEnd)
  {
    CHECKED_DELETE(*l_It);
    ++l_It;
  }

  m_mapSamples.clear();
  m_vSamples.clear();
}

CSoundManager::SSoundChannel* CSoundManager::GetSample(const string& _szSample)
{
  map<string,SSoundChannel*>::iterator l_It = m_mapSamples.find(_szSample);
  if(l_It == m_mapSamples.end()) 
  {
    return 0;
  }
  return l_It->second;
}

CSoundManager::SSoundChannel* CSoundManager::GetMusic(const string& _szSample)
{
  map<string,SSoundChannel*>::iterator l_It = m_mapMusics.find(_szSample);
  if(l_It == m_mapMusics.end()) 
  {
    return 0;
  }
  return l_It->second;
}

void CSoundManager::PlaySample(const string& _szSample)
{
  SSoundChannel* l_pSample = GetSample(_szSample);

  if(l_pSample)
  {
    HCHANNEL l_Channel = BASS_SampleGetChannel(l_pSample->m_iHandle,false);

    if(l_Channel)
    {
      BASS_ChannelSetAttribute(l_Channel, BASS_ATTRIB_VOL, l_pSample->m_fVolume);
      BASS_ChannelPlay(l_Channel,false);
    }
  }
}

void CSoundManager::StopSample(const string& _szSample)
{
  SSoundChannel* l_pSample = GetSample(_szSample);

  if(l_pSample)
  {
    BASS_SampleStop(l_pSample->m_iHandle);
  }
}

void CSoundManager::PlaySample3D(const string& _szSample, Vect3f _vPosition)
{
  SSoundChannel* l_pSample = GetSample(_szSample);

  if(l_pSample)
  {
    HCHANNEL l_Channel = BASS_SampleGetChannel(l_pSample->m_iHandle,false);

    if(l_Channel)
    {
      BASS_ChannelSetAttribute(l_Channel, BASS_ATTRIB_VOL, l_pSample->m_fVolume);

      BASS_3DVECTOR l_pos;

      l_pos.x = _vPosition.x;
      l_pos.y = _vPosition.y;
      l_pos.z = _vPosition.z;

      BASS_ChannelSet3DPosition(l_Channel, &l_pos, 0, 0);

      BASS_ChannelPlay(l_Channel,false);
    }
  }
}

void CSoundManager::ChangeMusic(const string& _szMusic, unsigned long _ulFadeOutTimeMs, bool _bRestart)
{
  vector<SSoundChannel*>::iterator l_It = m_vMusics.begin();
  vector<SSoundChannel*>::iterator l_ItEnd = m_vMusics.end();

  SSoundChannel* l_pMusic = 0;

  while(l_It != l_ItEnd)
  {
    l_pMusic = (*l_It);

    if(BASS_ChannelIsActive(l_pMusic->m_iHandle) == BASS_ACTIVE_PLAYING)
    {
      BASS_ChannelSlideAttribute(l_pMusic->m_iHandle,BASS_ATTRIB_VOL,-1,_ulFadeOutTimeMs);
    }
    ++l_It;
  }

  l_pMusic = GetMusic(_szMusic);

  if(l_pMusic)
  {
    BASS_ChannelSetAttribute(l_pMusic->m_iHandle, BASS_ATTRIB_VOL, 0.0f);
    BASS_ChannelSlideAttribute(l_pMusic->m_iHandle,BASS_ATTRIB_VOL,l_pMusic->m_fVolume,_ulFadeOutTimeMs);
    BASS_ChannelPlay(l_pMusic->m_iHandle,_bRestart);
  }
  
}


void CSoundManager::PlayMusic(const string& _szMusic, bool _bRestart)
{
  SSoundChannel* l_pMusic = GetMusic(_szMusic);

  if(l_pMusic)
  {
    BASS_ChannelSetAttribute(l_pMusic->m_iHandle, BASS_ATTRIB_VOL, l_pMusic->m_fVolume);
    BASS_ChannelPlay(l_pMusic->m_iHandle,_bRestart);
  }
}


float CSoundManager::GetMusicRemainingTime(const string& _szMusicName)
{
  SSoundChannel* l_pMusic = GetMusic(_szMusicName);

  if(l_pMusic)
  {
    QWORD l_qwRemaining = BASS_ChannelGetLength(l_pMusic->m_iHandle, BASS_POS_BYTE) - BASS_ChannelGetPosition(l_pMusic->m_iHandle, BASS_POS_BYTE);
    return (float) BASS_ChannelBytes2Seconds(l_pMusic->m_iHandle, l_qwRemaining) * 1000;
  }
  return 0;
}

void CSoundManager::SetMusic3DPosition(const string& _szMusic, const Vect3f& _vSoundEmmiterPosition)
{
  SSoundChannel* l_pMusic = GetMusic(_szMusic);

  if(l_pMusic)
  {
    BASS_3DVECTOR l_pos;

    l_pos.x = _vSoundEmmiterPosition.x;
    l_pos.y = _vSoundEmmiterPosition.y;
    l_pos.z = _vSoundEmmiterPosition.z;

    BASS_ChannelSet3DPosition(l_pMusic->m_iHandle, &l_pos, 0, 0);
  }
}

void CSoundManager::UpdateSound3DSystem(const Vect3f& _vListenerPosition, const Vect3f& _vListenerDirection)
{
  BASS_3DVECTOR l_Pos;
  BASS_3DVECTOR l_Dir;
  BASS_3DVECTOR l_Top;

  l_Pos.x = _vListenerPosition.x;
  l_Pos.y = _vListenerPosition.y;
  l_Pos.z = _vListenerPosition.z;

  l_Dir.x = _vListenerDirection.x;
  l_Dir.y = _vListenerDirection.y;
  l_Dir.z = _vListenerDirection.z;

  l_Top.x = 0.0f;
  l_Top.y = 1.0f;
  l_Top.z = 0.0f;

  BASS_Set3DPosition(&l_Pos,0,&l_Dir,&l_Top);
  BASS_Apply3D();
}

void CSoundManager::StopAll()
{
  BASS_Stop();
}

void CSoundManager::StopMusics()
{
  vector<SSoundChannel*>::iterator l_It;

  for(l_It = m_vMusics.begin(); l_It != m_vMusics.end(); ++l_It)
  {
    BASS_ChannelPause((*l_It)->m_iHandle);
  }
}

void CSoundManager::StopSounds()
{
  vector<SSoundChannel*>::iterator l_It;

  for(l_It = m_vSamples.begin(); l_It != m_vSamples.end(); ++l_It)
  {
    BASS_SampleStop((*l_It)->m_iHandle);
  }
}

void CSoundManager::PauseSamples()
{
  BASS_SAMPLE l_Info;
  HCHANNEL* l_pChannels;
  DWORD l_iCount;

  vector<SSoundChannel*>::iterator l_It;

  for(l_It = m_vSamples.begin(); l_It != m_vSamples.end(); ++l_It)
  {
    BASS_SampleGetInfo((*l_It)->m_iHandle, &l_Info);
    l_pChannels = new HCHANNEL[l_Info.max*sizeof(HCHANNEL)];
    l_iCount = BASS_SampleGetChannels((*l_It)->m_iHandle, l_pChannels);

    for( int i = 0; i < l_iCount; i++)
    {
        BASS_ChannelPause(l_pChannels[i]);
    }

    delete[] l_pChannels;
  }

}

void CSoundManager::ResumeSamples()
{
  BASS_SAMPLE l_Info;
  HCHANNEL* l_pChannels;
  DWORD l_iCount;

  vector<SSoundChannel*>::iterator l_It;

  for(l_It = m_vSamples.begin(); l_It != m_vSamples.end(); ++l_It)
  {
    BASS_SampleGetInfo((*l_It)->m_iHandle, &l_Info);
    l_pChannels = new HCHANNEL[l_Info.max*sizeof(HCHANNEL)];

    if(l_pChannels)
    {
      l_iCount = BASS_SampleGetChannels((*l_It)->m_iHandle, l_pChannels);

      for( int i = 0; i < l_iCount; i++)
      {
        if(BASS_ChannelIsActive(l_pChannels[i]) & BASS_ACTIVE_PAUSED)
        {
          BASS_ChannelPlay(l_pChannels[i],false);
        }
      }


      delete[] l_pChannels;
    }
  }
}

void CSoundManager::SetMasterVolume(float _fVolume)
{
  if(_fVolume > 1.0f)
    _fVolume = 1.0f;

  if(_fVolume < 0.0f)
    _fVolume = 0.0f;

  BASS_SetVolume(_fVolume);
}

void CSoundManager::Pause(const string& _szMusic)
{
  SSoundChannel* l_pMusic = GetMusic(_szMusic);

  if(l_pMusic)
  {
    BASS_ChannelPause(l_pMusic->m_iHandle);
  }
}

void CSoundManager::Resume(const string& _szMusic)
{
  PlayMusic(_szMusic,false);
}

void CSoundManager::FadeMusicVolume(const string& _szMusic, float _fVolume, unsigned long _iTimeMs)
{
  SSoundChannel* l_pMusic = GetMusic(_szMusic);

  if(l_pMusic)
  {
    BASS_ChannelSlideAttribute(l_pMusic->m_iHandle,BASS_ATTRIB_VOL,_fVolume,_iTimeMs);
  }
}
