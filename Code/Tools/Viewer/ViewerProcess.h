#pragma once

#include <algorithm>
#include "Process.h"
#include <base.h>
#include "Viewer.h"

#define MODE_ESCENA 0
#define MODE_MESH 1
#define MODE_ANIMATS 2
//#define MODE_AJUDA 3


//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CalModel;
class CalCoreModel;
class CalHardwareModel;
class CRenderableVertexs;
class CEffectMaterial;
class CRenderableObject;
//-----------------------------------------

class CViewerProcess:
  public CProcess
{

public:
                        CViewerProcess            (): CProcess("Viewer Process")  {};
  virtual               ~CViewerProcess           ()                              {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

  virtual void          RegisterLuaFunctions      ();

  void                  ResetViewer               ();

private:

  CViewer* m_pViewer;
	void                  Release                 ();

};