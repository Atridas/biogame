#pragma once
#ifndef __BIOGAME_PARAMS_H__
#define __BIOGAME_PARAMS_H__

/**
 * @file
**/

#include <base.h>

/**
 * Par�metres del render manager.
 * Estructura que cont� els par�metres de configuraci�, referents a RenderManager, que es poden trobar al config del proc�s.
 *  - v2iResolution: Resoluci� WxH de la pantalla. Default 800x600.
 *  - v2iPosition: TODO. Default 0,0.
 *  - bFullScreen: FullScreen activat o desactivat. Default false.
 *  - vRenderableMeshes: Vector de meshes existents. Default buit.
**/
struct SRenderManagerParams
{
  Vect2i v2iResolution;
  
  Vect2i v2iPosition;

  bool bFullscreen;

  vector<string> vRenderableMeshes;
  vector<string> vAnimatedModels;

  string szEffectsXML;

  //Default params
  SRenderManagerParams():
    v2iResolution(800,600),

    v2iPosition(0,0),

    bFullscreen(false)
  {};
};

/**
 * Par�metres del font manager.
 * Estructura que cont� els par�metres de configuraci�, referents a FontManager, que es poden trobar al config del proc�s.
 *  - pcFontsXML: Ruta de localitzaci� del fitxer de fonts. Default "./Data/Fonts/Fonts.xml".
**/
struct SFontManagerParams
{
  const char* pcFontsXML;

  //Default Params
  SFontManagerParams():
    pcFontsXML("Data/Fonts/Fonts.xml")
  {};
};

/**
 * Par�metres del language manager.
 * Estructura que cont� els par�metres de configuraci�, referents a LanguageManager, que es poden trobar al config del proc�s.
 *  - pcDefault: Idioma per defecte. Default "eng".
 *  - vXMLFiles: Vector de fitxers d'idioma. Default buit.
**/
struct SLanguageManagerParams
{
  const char* pcDefault;
  vector<string> vXMLFiles;

  SLanguageManagerParams():
  pcDefault("eng")
  {}
};

/**
 * Par�metres de l'input manager.
 * Estructura que cont� els par�metres de configuraci�, referents a InputManager, que es poden trobar al config del proc�s.
 *  - bExclusiveMouse: Us exclusiu de mouse. Default false.
**/
struct SInputManagerParams
{
  bool bExclusiveMouse;

  SInputManagerParams():
  bExclusiveMouse(false)
  {}
};

/**
 * Par�metres d'Action to Input.
 * Estructura que cont� els par�metres de configuraci�, referents a ActiontoInput, que es poden trobar al config del proc�s.
 *  - pcFile: Ruta de localitzaci� del fitxer d'accions. Default "./Data/XML/actions.xml".
**/
struct SActionToInputParams
{
  const char* pcFile;

  //Default Params
  SActionToInputParams():
    pcFile("Data/XML/actions.xml")
  {};
};

/**
 * Par�metres del manager d'objectes renderables.
 * Estructura que cont� els par�metres de configuraci�, referents a RenderableObjectsManager, que es poden trobar al config del proc�s.
 *  - vXMLFiles: Vector de fitxers de configuraci� per a RenderableObjectsManager. Default buit.
**/
struct SRenderableObjectsManager
{
  vector<string> vXMLFiles;
};


struct SLightsManager
{
  string szFile;

  //Default Params
  SLightsManager():
    szFile("Data/XML/Lights.xml")
  {};
};

struct SSceneEffectManager
{
  string szFile;

  //Default Params
  SSceneEffectManager():
    szFile("Data/XML/SceneEffects.xml")
  {};
};

struct SScriptManager
{
  string szFile;

  //Default Params
  SScriptManager():
    szFile("Data/XML/Lua.xml")
  {};
};

/**
 * Par�metres d'inicialitzaci� del proc�s.
 * Estructura que cont� els par�metres de configuraci� que es poden trobar al config del proc�s.
 *  - RenderManagerParams: Par�metres del render manager.
 *  - FontManagerParams: Par�metres del font manager.
 *  - LanguageManagerParams: Par�metres del language manager.
 *  - InputManagerParams: Par�metres de l'input manager.
 *  - ActionToInputParams: Par�metres d'Action to Input.
 *  - RenderableObjectsManager: Par�metres del manager d'objectes renderables.
 * @see SRenderManagerParams
 * @see SFontManagerParams
 * @see SLanguageManagerParams
 * @see SInputManagerParams
 * @see SActionToInputParams
 * @see SRenderableObjectsManager
**/
struct SInitParams
{
  SRenderManagerParams      RenderManagerParams;
  SFontManagerParams        FontManagerParams;
  SLanguageManagerParams    LanguageManagerParams;
  SInputManagerParams       InputManagerParams;
  SActionToInputParams      ActionToInputParams;
  SRenderableObjectsManager RenderableObjectsManager;
  SLightsManager            LightsManager;
  SSceneEffectManager       SceneEffect;
  SScriptManager            ScriptManager;
};

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML);

#endif