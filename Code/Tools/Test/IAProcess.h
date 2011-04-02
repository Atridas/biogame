#pragma once

#include "Process.h"
#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CRenderManager;
class CSparseGraph;
//-----------------------------------------

class CIAProcess:
  public CProcess
{

public:
                        CIAProcess       (): CProcess("CIAProcess"), m_pTargetObject(0), m_pObjectCamera(0), m_pGraph(0)
                                                {};
  virtual               ~CIAProcess      ()                          {Done();};

  virtual bool          Init();

	void                  Update                  (float elapsedTime);
	void					        DebugInformation        (float FPS);
  void                  RenderScene             (CRenderManager* _pRM);
  void                  RenderINFO              (CRenderManager* _pRM);

  virtual bool          ExecuteProcessAction      (float _fDeltaSeconds, float _fDelta, const char* _pcAction);

private:

  void UpdateCamera(float _fDeltaPitch, float _fDeltaYaw);
  void UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw);

  void UpdateIA(float _fDT);

	void                  Release                 ();

  CObject3D*            m_pTargetObject;

  CCamera*              m_pObjectCamera;
  
  Vect3i                m_vMouseDelta;
  
  CSparseGraph*         m_pGraph;


  //Demo gràfica
  int                   m_iNodeObjectiu;
  list<int>             m_liPath;
  Vect3f                m_vDemoObj;
};