#pragma once

#include <algorithm>
#include "Process.h"
#include <base.h>

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
                        CViewerProcess            (): CProcess("Viewer Process"), m_pObject(0), m_pObjectCamera(0),
                                                    m_fVelocity(0.0f),m_bRenderLights(false),m_iMode(0),m_iAnimat(0),
                                                    m_iMesh(0),m_iRenderObject(0),m_bVisibleLights(true),m_bAjuda(true)
                                                {};
  virtual               ~CViewerProcess           ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);
  float                 GetZoomMesh             (Vect3f _vMax,Vect3f _vCentre);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

	void                  Release                 ();

  int                   m_iState;
  bool                  m_bStateChanged;
  CObject3D*            m_pObject;
  CObject3D*            m_pObjectBot;
  CCamera*              m_pObjectCamera;
  float                 m_fVelocity;
  bool                  m_bRenderLights;
  bool                  m_bVisibleLights;
  int                   m_iMode;
  int                   m_iAnimat;
  int                   m_iMesh;
  int                   m_iRenderObject;
  bool                  m_bAjuda;

  vector<CRenderableObject*> m_vMeshes;
  vector<CRenderableObject*> m_vAnimatedModels;
};