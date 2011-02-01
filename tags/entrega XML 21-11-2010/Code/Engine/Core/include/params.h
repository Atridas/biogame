#pragma once
#ifndef __BIOGAME_PARAMS_H__
#define __BIOGAME_PARAMS_H__

#include <base.h>

struct SRenderManagerParams
{
  uint32 uiWidth;
  uint32 uiHeight;
  
  uint32 uiPosX;
  uint32 uiPosY;

  bool bFullscreen;


  //Default params
  SRenderManagerParams():
    uiWidth(800),
    uiHeight(600),

    uiPosX(0),
    uiPosY(0),

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

struct SInitParams
{
  SRenderManagerParams RenderManagerParams;
  SFontManagerParams FontManagerParams;
  SLanguageManagerParams LanguageManagerParams;
};

#endif