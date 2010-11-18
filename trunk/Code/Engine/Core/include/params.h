#pragma once
#ifndef __BIOGAME_PARAMS_H__
#define __BIOGAME_PARAMS_H__

#include <base.h>

struct SRenderManagerParams
{
  uint32 m_uiWidth;
  uint32 m_uiHeight;
  
  uint32 m_uiPositionWidth;
  uint32 m_uiPositionHeight;

  bool m_bFullscreen;


  //Default params
  SRenderManagerParams():
    m_uiWidth(800),
    m_uiHeight(600),

    m_uiPositionWidth(0),
    m_uiPositionHeight(0),

    m_bFullscreen(false)
  {};
};

struct SFontManagerParams
{
  const char* m_pcFontsXML;

  //Default Params
  SFontManagerParams():
    m_pcFontsXML("./Data/Fonts/Fonts.xml")
  {};
};


struct SInitParams
{
  SRenderManagerParams m_RenderManagerParams;
  SFontManagerParams m_FontManagerParams;
};

#endif