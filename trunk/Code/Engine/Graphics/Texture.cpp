#include "include\Texture.h"


bool CTexture::LoadFile() 
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Load \"%s\"",m_FileName.c_str());
  return true;
}

void CTexture::Unload()
{
   LOGGER->AddNewLog(ELL_INFORMATION, "CTexture::Unload");
}

bool CTexture::Load(const std::string &FileName)
{
  if(m_FileName != "")
  {
    LOGGER->AddNewLog(ELL_WARNING, "CTexture::Load carregant \"%s\" quan ja hi ha \"%s\" carregada",FileName.c_str(),m_FileName.c_str());
  }
  m_FileName = FileName;
  return LoadFile();
}

bool CTexture::Reload()
{
  return true;
}

void CTexture::Activate(size_t StageId)
{

}