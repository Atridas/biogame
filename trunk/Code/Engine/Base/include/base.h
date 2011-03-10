#pragma once

/**
 * @file
**/

#ifndef NULL
#define NULL 0
#endif

#include <vector>
#include <map>
#include <string>
#include <assert.h>
#include "Utils/Logger.h"
#include "Utils/Types.h"
#include "Utils/BaseControl.h"
#include "Math/Color.h"
#include "Math/Matrix33.h"
#include "Math/Matrix34.h"
#include "Math/Matrix44.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/MathTypes.h"

using namespace std;

#if defined( _DEBUG )  
#ifndef __DONT_INCLUDE_MEM_LEAKS__
#include "Utils/MemLeaks.h" 
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

/**
 * @define Macro per alliberar punters.
 * Allibera el punter proporcionat si aquest té contingut.
 * @param Punter a alliberar.
**/
#define CHECKED_DELETE(x)                     if(x) delete x; x=NULL;
/**
 * @define Macro per alliberar punters a classes amb Release().
 * Allibera el punter proporcionat si aquest té contingut.
 * @param Punter a la classe a alliberar.
**/
#define CHECKED_RELEASE(x)                    if(x) x->Release(); x=NULL;
/**
 * @define Macro per alliberar punters a classes que deriven de BaseControl.
 * Allibera el punter proporcionat si aquest té contingut.
 * @param Punter a la classe a alliberar.
**/
#define CHECKED_DONE(x)                       if(x) x->Done(); x=NULL;
/**
 * @define Macro per alliberar arrays.
 * Allibera l'array proporcionada si aquesta té contingut.
 * @param Array a alliberar.
**/
#define CHECKED_DELETE_ARRAY(x)               if(x) delete [] x; x=NULL;

/**
 * @define Macro d'accés al Core.
**/
#define CORE CCore::GetSingletonPtr()
/**
 * @define Macro d'accés al Logger.
**/
#define LOGGER CLogger::GetSingletonPtr()
/**
 * @define Macro d'accés al Font Manager.
**/
#define FONT_MANAGER CORE->GetFontManager()
/**
 * @define Macro d'accés al Render Manager.
**/
#define RENDER_MANAGER CORE->GetRenderManager()
/**
 * @define Macro d'accés a l'Input Manager.
**/
#define INPUT_MANAGER CORE->GetInputManager()
/**
 * @define Macro d'accés a Animated Model Manager.
**/
#define ANIMATED_MANAGER CORE->GetAnimatedModelManager()
/**
 * @define Macro d'accés a Sound Manager.
**/
#define SOUND_MANAGER CORE->GetSoundManager()