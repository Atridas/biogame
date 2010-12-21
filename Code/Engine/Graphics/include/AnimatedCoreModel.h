#pragma once
#ifndef __ANIMATED_CORE_MODEL_H__
#define __ANIMATED_CORE_MODEL_H__

#include "base.h"

// Forward declarations -------------
class CalCoreModel;
// ----------------------------------


class CAnimatedCoreModel:
  public CBaseControl
{
public:
  CAnimatedCoreModel(void): m_pCalCoreModel(0),
                            m_szName(""),m_szMeshFilename(""),
                            m_szSkeletonFilename(""),m_szPath("")
                                        {};
  ~CAnimatedCoreModel(void)             {Done();};

  CalCoreModel *GetCoreModel      ( )                         { return m_pCalCoreModel; };
  const string & GetTextureName   ( size_t id ) const         { return m_vTextureFilenameList[id]; };
  size_t GetNumTextures           ( ) const                   { return m_vTextureFilenameList.size(); };
  bool Load                       (const std::string &_szPath);

private:
  bool LoadMesh();
  bool LoadSkeleton();
  bool LoadAnimation(const string& _szName, const std::string& _szFilename);

  CalCoreModel*             m_pCalCoreModel;
  string                    m_szName;
  string                    m_szMeshFilename;
  string                    m_szSkeletonFilename;
  vector<std::string>       m_vTextureFilenameList;
  string                    m_szPath;
};

#endif
