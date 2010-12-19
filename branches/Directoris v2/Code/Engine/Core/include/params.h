#pragma once
#ifndef __BIOGAME_PARAMS_H__
#define __BIOGAME_PARAMS_H__

#include <base.h>

struct SRenderManagerParams
{
  Vect2i v2iResolution;
  
  Vect2i v2iPosition;

  bool bFullscreen;

  vector<string> vRenderableMeshes;


  //Default params
  SRenderManagerParams():
    v2iResolution(800,600),

    v2iPosition(0,0),

    bFullscreen(false)
  {};
};

struct SFontManagerParams
{
  const char* pcFontsXML;

  //Default Params
  SFontManagerParams():
    pcFontsXML("./Data/Fonts/Fonts.xml")
  {};
};

struct SLanguageManagerParams
{
  const char* pcDefault;
  vector<string> vXMLFiles;

  SLanguageManagerParams():
  pcDefault("eng")
  {}
};

struct SInputManagerParams
{
  bool bExclusiveMouse;

  SInputManagerParams():
  bExclusiveMouse(false)
  {}
};

struct SActionToInputParams
{
  const char* pcFile;

  //Default Params
  SActionToInputParams():
    pcFile("./Data/XML/actions.xml")
  {};
};

struct SInitParams
{
  SRenderManagerParams    RenderManagerParams;
  SFontManagerParams      FontManagerParams;
  SLanguageManagerParams  LanguageManagerParams;
  SInputManagerParams     InputManagerParams;
  SActionToInputParams    ActionToInputParams;
};

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML);

#endif