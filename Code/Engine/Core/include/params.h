#pragma once
#ifndef __BIOGAME_PARAMS_H__
#define __BIOGAME_PARAMS_H__

/**
 * @file
**/

#include <base.h>

/**
 * Paràmetres del render manager.
 * Estructura que conté els paràmetres de configuració, referents a RenderManager, que es poden trobar al config del procés.
 *  - v2iResolution: Resolució WxH de la pantalla. Default 800x600.
 *  - v2iPosition: TODO. Default 0,0.
 *  - bFullScreen: FullScreen activat o desactivat. Default false.
 *  - vRenderableMeshes: Vector de meshes existents. Default buit.
**/
struct SRenderManagerParams
{
  Vect2i v2iResolution;
  
  Vect2i v2iPosition;

  bool bFullscreen;

  //Default params
  SRenderManagerParams():
    v2iResolution(800,600),

    v2iPosition(0,0),

    bFullscreen(false)
  {};
};

struct SEngineParams
{
  string szHDRFile;

  //Default params
  SEngineParams():
    szHDRFile("Data/XML/HDR.xml")
    {};
};

/**
 * Paràmetres del font manager.
 * Estructura que conté els paràmetres de configuració, referents a FontManager, que es poden trobar al config del procés.
 *  - pcFontsXML: Ruta de localització del fitxer de fonts. Default "./Data/Fonts/Fonts.xml".
**/
struct SFontManagerParams
{
  const char* pcFontsXML;

  //Default Params
  SFontManagerParams():
    pcFontsXML("Data/XML/fonts.xml")
  {};
};

/**
 * Paràmetres del language manager.
 * Estructura que conté els paràmetres de configuració, referents a LanguageManager, que es poden trobar al config del procés.
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
 * Paràmetres de l'input manager.
 * Estructura que conté els paràmetres de configuració, referents a InputManager, que es poden trobar al config del procés.
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
 * Paràmetres d'Action to Input.
 * Estructura que conté els paràmetres de configuració, referents a ActiontoInput, que es poden trobar al config del procés.
 *  - pcFile: Ruta de localització del fitxer d'accions. Default "./Data/XML/actions.xml".
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
 * Paràmetres del manager d'objectes renderables.
 * Estructura que conté els paràmetres de configuració, referents a RenderableObjectsManager, que es poden trobar al config del procés.
 *  - vXMLFiles: Vector de fitxers de configuració per a RenderableObjectsManager. Default buit.
**/
struct SRenderableObjectsManagerParams
{
  vector<string> vXMLFiles;
};


struct SLightsManagerParams
{
  string szFile;

  //Default Params
  SLightsManagerParams():
    szFile("Data/XML/Lights.xml")
  {};
};

struct SSceneEffectManagerParams
{
  string szFile;

  //Default Params
  SSceneEffectManagerParams():
    szFile("Data/XML/SceneEffects.xml")
  {};
};

struct SScriptManagerParams
{
  string szFile;

  //Default Params
  SScriptManagerParams():
    szFile("Data/XML/Lua.xml")
  {};
};

struct SStaticMeshManagerParams
{
  vector<string> vRenderableMeshes;

};

struct SAnimatedModelManagerParams
{
  vector<string> vAnimatedModels;

};

struct SEffectManagerParams
{
  string szFile;

  //Default Params
  SEffectManagerParams():
    szFile("Data/XML/Effects.xml")
  {};
};

struct SLogRenderParams
{
  Vect2i vPosition;

  //Default Params
  SLogRenderParams():
    vPosition(10,0)
  {};
};

struct SGUIManagerParams
{
  string szXML;
  string szInitWindow;

  //Default Params
  SGUIManagerParams():
    szXML("Data/XML/GUI.xml"),
    szInitWindow("Main.xml")
  {};
};

/**
 * Paràmetres d'inicialització del procés.
 * Estructura que conté els paràmetres de configuració que es poden trobar al config del procés.
 *  - RenderManagerParams: Paràmetres del render manager.
 *  - FontManagerParams: Paràmetres del font manager.
 *  - LanguageManagerParams: Paràmetres del language manager.
 *  - InputManagerParams: Paràmetres de l'input manager.
 *  - ActionToInputParams: Paràmetres d'Action to Input.
 *  - RenderableObjectsManager: Paràmetres del manager d'objectes renderables.
 * @see SRenderManagerParams
 * @see SFontManagerParams
 * @see SLanguageManagerParams
 * @see SInputManagerParams
 * @see SActionToInputParams
 * @see SRenderableObjectsManager
**/
struct SInitParams
{
  SEngineParams                   EngineParams;
  SRenderManagerParams            RenderManagerParams;
  SStaticMeshManagerParams        StaticMeshManagerParams;
  SAnimatedModelManagerParams     AnimatedModelManagerParams;
  SEffectManagerParams            EffectManagerParams;
  SFontManagerParams              FontManagerParams;
  SLanguageManagerParams          LanguageManagerParams;
  SInputManagerParams             InputManagerParams;
  SActionToInputParams            ActionToInputParams;
  SRenderableObjectsManagerParams RenderableObjectsManagerParams;
  SLightsManagerParams            LightsManagerParams;
  SSceneEffectManagerParams       SceneEffectParams;
  SScriptManagerParams            ScriptManagerParams;
  SLogRenderParams                LogRenderParams;
  SGUIManagerParams               GUIManagerParams;
};

void ReadXMLInitParams(SInitParams& InitParams_, const char* _pcPathXML);

#endif