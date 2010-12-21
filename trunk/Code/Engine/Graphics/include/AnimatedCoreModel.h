#pragma once
#ifndef __ANIMATED_CORE_MODEL_H__
#define __ANIMATED_CORE_MODEL_H__

#include "base.h"
#include "Named.h"

// Forward declarations -------------
class CalCoreModel;
// ----------------------------------


class CAnimatedCoreModel:
  public CBaseControl,
  public CNamed
{
public:
  CAnimatedCoreModel(const string& _szName): 
                            CNamed(_szName),m_pCalCoreModel(0),
                            m_szMeshFilename(""),
                            m_szSkeletonFilename(""),m_szPath("")
                                        {};
  ~CAnimatedCoreModel(void)             {Done();};

  CalCoreModel *GetCoreModel      ( )                         { return m_pCalCoreModel; };
  const string & GetTextureName   ( size_t id ) const         { return m_vTextureFilenameList[id]; };
  size_t GetNumTextures           ( ) const                   { return m_vTextureFilenameList.size(); };
  bool Load                       (const std::string &_szPath);
  bool Reload                     ()                          { Release(); return Load(m_szPath);};
  bool Reload                     (const std::string &_szPath){ Release(); return Load( _szPath);};

protected:
  void Release                    ();

private:
  bool LoadMesh                   ();
  bool LoadSkeleton               ();
  bool LoadAnimation              (const string& _szName, const std::string& _szFilename);

  CalCoreModel*             m_pCalCoreModel;
  string                    m_szMeshFilename;
  string                    m_szSkeletonFilename;
  vector<std::string>       m_vTextureFilenameList;
  string                    m_szPath;
};

#endif
