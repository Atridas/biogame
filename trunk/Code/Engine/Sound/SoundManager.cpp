#include "SoundManager.h"
#include "bass.h"
#include <XML/XMLTreeNode.h>

bool CSoundManager::Init(const string& _szFile)
{
  if(!BASS_Init(1, 44100, BASS_DEVICE_3D, 0, NULL))
  {
    LOGGER->AddNewLog(ELL_ERROR,"CSoundManager::Init Error al inicialitzar la BASS lib");
    SetOk(false);
    return IsOk();
  }

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

    l_szName = l_XMLSound.GetPszISOProperty("name" ,"");
    l_szFile = l_XMLSound.GetPszISOProperty("file" ,"");
    l_szType = l_XMLSound.GetPszISOProperty("type" ,"");
    l_bSound3D = l_XMLSound.GetBoolProperty("sound3D");
    l_bLoop = l_XMLSound.GetBoolProperty("loop");
    l_iPriority = l_XMLSound.GetIntProperty("priority");

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
          l_pSound = new SSoundChannel(l_iStream,1.0f);
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
          l_pSound = new SSoundChannel(l_iStream,1.0f);
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

void CSoundManager::PlaySample3D()
{

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

void CSoundManager::PlayMusic3D()
{

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

void CSoundManager::SetMasterVolume()
{

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
