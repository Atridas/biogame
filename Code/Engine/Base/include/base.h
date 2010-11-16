#pragma once

#ifndef NULL
#define NULL 0
#endif

#include <assert.h>
#include "Utils/Logger.h"
#include "Utils/Types.h"
#include "Utils/BaseControl.h"
#include "Math/Matrix33.h"
#include "Math/Matrix34.h"
#include "Math/Matrix44.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/MathTypes.h"


#ifndef NULL
#define NULL 0
#endif

#define CHECKED_DELETE(x)                     if(x) delete x; x=NULL;
#define CHECKED_RELEASE(x)                    if(x) x->Release(); x=NULL;
#define CHECKED_DELETE_ARRAY(x)               if(x) delete [] x; x=NULL;

#define CORE CCore::GetSingletonPtr()
#define LOGGER CLogger::GetSingletonPtr()