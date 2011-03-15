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

    map<string,HSTREAM>::iterator l_It = m_mResources.find(l_szName);
    if(l_It != m_mResources.end()) 
    {
      LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init Sampler repetit: \"%s\"", l_szName.c_str());
      continue;
    }

    int l_iMask = 0;

    if(l_bLoop)
      l_iMask |= BASS_SAMPLE_LOOP;

    if(l_bSound3D)
      l_iMask |= BASS_SAMPLE_3D;

    HSTREAM l_iStream = 0;

    if(l_szType.compare("music") == 0)
    {
      l_iStream = BASS_StreamCreateFile(false,l_szFile.c_str(), 0, 0, l_iMask);
      if(l_iStream)
      {
        m_vMusicChannels.push_back(SMusicChannel(l_iStream,1.0f));
      }else{
        LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init error al crear stream: \"%s\"", l_szFile.c_str());
      }
    }
    
    if(l_szType.compare("sample") == 0){
      int l_iMaxSamples = l_XMLSound.GetIntProperty("maxSamples",1);
      l_iStream = BASS_SampleLoad(false,l_szFile.c_str(), 0, 0, l_iMaxSamples, l_iMask);
      if(l_iStream)
      {

      }else{
        LOGGER->AddNewLog(ELL_WARNING,"CSoundManager::Init error al crear sample: \"%s\"", l_szFile.c_str());
      }
    }

    if(l_iStream)
    {
      m_mResources[l_szName] = l_iStream;
    }

  }

  SetOk(true);

  return IsOk();
}

void CSoundManager::Release()
{
  BASS_Stop();
  BASS_Free();

  m_mResources.clear();
}

HSTREAM CSoundManager::GetSample(const string& _szSample)
{
  map<string,HSTREAM>::iterator l_It = m_mResources.find(_szSample);
  if(l_It == m_mResources.end()) 
  {
    return 0;
  }
  return l_It->second;
}

void CSoundManager::PlaySample(const string& _szSample)
{
  HSTREAM l_Sample = GetSample(_szSample);

  if(l_Sample != 0)
  {
    HCHANNEL l_Channel = BASS_SampleGetChannel(l_Sample,false);

    if(l_Channel)
    {
      BASS_ChannelSetAttribute(l_Channel, BASS_ATTRIB_VOL, 1.0f);
      BASS_ChannelPlay(l_Channel,false);
    }
  }
}

void CSoundManager::PlaySample3D()
{

}

void CSoundManager::PlayMusic(const string& _szSample)
{
  HSTREAM l_Sample = GetSample(_szSample);

  if(l_Sample != 0)
  {
    BASS_ChannelSetAttribute(l_Sample, BASS_ATTRIB_VOL, 1.0f);
    BASS_ChannelPlay(l_Sample,false);
  }
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
