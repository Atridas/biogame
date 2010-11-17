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

  SRenderManagerParams():
    m_uiWidth(800),
    m_uiHeight(600),

    m_uiPositionWidth(0),
    m_uiPositionHeight(0),

    m_bFullscreen(false)
  {};
};



struct SInitParams
{
  SRenderManagerParams m_RenderManagerParams;

};

#endif