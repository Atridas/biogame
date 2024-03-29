#pragma once
#ifndef __BASE_BIOGAME_H__
#define __BASE_BIOGAME_H__

#ifndef NULL
#define NULL 0
#endif

#include <list>
#include <vector>
#include <map>
#include <string>
#include <set>
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
#include "Math/MathUtils.h"

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
 * Allibera el punter proporcionat si aquest t� contingut.
 * @param Punter a alliberar.
**/
#define CHECKED_DELETE(x)                     if(x) delete x; x=NULL;
/**
 * @define Macro per alliberar punters a classes amb Release().
 * Allibera el punter proporcionat si aquest t� contingut.
 * @param Punter a la classe a alliberar.
**/
#define CHECKED_RELEASE(x)                    if(x) x->Release(); x=NULL;
/**
 * @define Macro per alliberar punters a classes que deriven de BaseControl.
 * Allibera el punter proporcionat si aquest t� contingut.
 * @param Punter a la classe a alliberar.
**/
#define CHECKED_DONE(x)                       if(x) x->Done(); x=NULL;
/**
 * @define Macro per alliberar arrays.
 * Allibera l'array proporcionada si aquesta t� contingut.
 * @param Array a alliberar.
**/
#define CHECKED_DELETE_ARRAY(x)               if(x) delete [] x; x=NULL;

/**
 * @define Macro d'acc�s al Core.
**/
#define CORE CCore::GetSingletonPtr()
/**
 * @define Macro d'acc�s al Logger.
**/
#define LOGGER CLogger::GetSingletonPtr()
/**
 * @define Macro d'acc�s al Font Manager.
**/
#define FONT_MANAGER CORE->GetFontManager()
/**
 * @define Macro d'acc�s al Render Manager.
**/
#define RENDER_MANAGER CORE->GetRenderManager()
/**
 * @define Macro d'acc�s a l'Input Manager.
**/
#define INPUT_MANAGER CORE->GetInputManager()
/**
 * @define Macro d'acc�s a Animated Model Manager.
**/
#define ANIMATED_MANAGER CORE->GetAnimatedModelManager()
/**
 * @define Macro d'acc�s a Sound Manager.
**/
#define SOUND_MANAGER CORE->GetSoundManager()
/**
 * @define Macro d'acc�s al Physx Manager.
**/
#define PHYSICS_MANAGER CORE->GetPhysicsManager()
/**
 * @define Macro d'acc�s a Entity Manager.
**/
#define ENTITY_MANAGER CORE->GetEntityManager()


float  Random01();
float  RandomNumber(float _fNumA, float _fNumB);
int    RandomNumber(int   _iNumA, int   _iNumB);
Vect3f RandomUnitVector();
Mat33f RandomRotationMatrix(float _fMaxAngle);


D3DXVECTOR3 InterpolateNumber(const D3DXVECTOR3& _vInicial, const D3DXVECTOR3& _vFinal,float _fTime, float _fTimeDelta);
Vect3f InterpolateNumber(const Vect3f& _vInicial, const Vect3f& _vFinal,float _fTime, float _fTimeDelta);
D3DXCOLOR InterpolateNumber(const D3DXCOLOR& _vInicial, const D3DXCOLOR& _vFinal,float _fTime, float _fTimeDelta);
float InterpolateNumber(float _fInicial, float _fFinal,float _fTime, float _fTimeDelta);

Mat33f GetFastestRotationFromDirToDir(const Vect3f& _vOriginalDir, const Vect3f& _vDestinationDir);
Mat33f GetRotationFromOrientationToOrientation(
                              const Vect3f& _vOriginalDir,    const Vect3f& _vOriginalUp, 
                              const Vect3f& _vDestinationDir, const Vect3f& _vDestinationUp);

void GetFastestRotationFromDirToDir(const Vect3f& _vOriginalDir, const Vect3f& _vDestinationDir,
                                          Vect3f& vAxis_, float& fAngle_);
void GetRotationFromOrientationToOrientation(
                              const Vect3f& _vOriginalDir,    const Vect3f& _vOriginalUp, 
                              const Vect3f& _vDestinationDir, const Vect3f& _vDestinationUp,
                                    Vect3f& vAxis_, float& fAngle_);

void ConcatenateRotations(const Vect3f& _vAxis1, float _fAngle1, 
                          const Vect3f& _vAxis2, float _fAngle2, 
                          Vect3f& vAxis_, float& fAngle_);

//Entrar n�meros, mai funcions!!!!!!!
#define SIMPLE_INTERPOLATION(a, b, t) ((a * (1-t)) + (b * t))


class CRenderManager;
class CEffect;
class CMaterial;

class IRenderable
{
public:
  virtual void Render(CRenderManager *_pRM, const vector<CEffect*>& _vEffects) = 0;
  virtual const vector<CMaterial*>& GetMaterials() const = 0;
};

#endif