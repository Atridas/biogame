#pragma once

#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CCamera;
class CRenderableObject;
class CRenderableAnimatedInstanceModel;
class CDirectionalLight;
class CSpotLight;
class CRenderManager;
//-----------------------------------------

class CViewer
{
public:
  CViewer(void);
  ~CViewer(void);

  enum EModes
  {
    FREE_MODE = 0,
    MESH_MODE,
    ANIMATED_MODE
  };

  //Process
  void Init();

  void Update(const float _fElapsedTime,const Vect3i& _vMouseDelta);
  void ShowInfo();

  //setters
  void SetMode(EModes _eMode);
  void SetNextMode();

  //getters
  bool GetRenderLights() {return m_bRenderLights;};
  bool GetNormalRendering() {return m_bNormalRendering;};
  CCamera* GetCamera() {return m_pObjectCamera;};
  int GetCurrentMode() { return m_iMode; };

  //actions
  void SetRunning();
  void SetWalking();

  void MoveFwd();
  void MoveBack();
  void MoveLeft();
  void MoveRight();

  void IncrementAmbientLight();
  void DecrementAmbientLight();

  void ToggleLights();
  void ToggleHelp();

  void IncreaseZoom();
  void DecreaseZoom();

  void SelectNextMesh();
  void SelectPrevMesh();

  void SelectNextAnimatedModel();
  void SelectPrevAnimatedModel();

  void SetNextAnimation();
  void SetPrevAnimation();

  void ToggleNormalRendering();
  void ToggleShowBoxes();
  void ToggleShowSpheres();

  bool ExecuteAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);

  void Reset() {Release(); Init();};

  void SetViewMode(bool _bValue) { m_bViewMode = _bValue; };
  bool GetViewMode()             { return m_bViewMode; };

private:
  void Release();

  void UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw);
  void UpdateCamera(float _fDeltaPitch, float _fDeltaYaw);
  void FocusCurrentMesh();
  void FocusCurrentAnimatedModel();
  void ResetActions();
  void ProcessFreeMode(const float _fElapsedTime,const Vect3i& _vMouseDelta);
  void ProcessMeshMode(const float _fElapsedTime,const Vect3i& _vMouseDelta);
  void ProcessAnimatedMode(const float _fElapsedTime,const Vect3i& _vMouseDelta);
  void InitMode();
  void InitFreeMode();
  void InitMeshMode();
  void InitAnimatedMode();
  void ShowFreeModeInfo();
  void ShowMeshModeInfo();
  void ShowAnimatedModeInfo();

  bool ExecuteFreeModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  bool ExecuteMeshModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  bool ExecuteAnimatedModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);

  CCamera* m_pObjectCamera;
  CObject3D* m_pTargetObject;
  CRenderableAnimatedInstanceModel* m_pCharacter;
  CDirectionalLight* m_pObjectModeLight;
  CSpotLight* m_pSpotLight;
  Vect3f m_vAmbientLight;
  bool m_bEnableLights;
  float m_fInitialCharacterYaw;
  bool m_bRenderLights;
  float m_fVelocity;

  int  m_iMode;
  bool m_bViewMode;
  bool m_bShowHelp;

  bool m_bMoveFwd;
  bool m_bMoveBack;
  bool m_bMoveLeft;
  bool m_bMoveRight;

  bool m_bNormalRendering;
  bool m_bShowBoxes;
  bool m_bShowSpheres;

  vector<CRenderableObject*>::iterator m_itCurrentMesh;
  vector<CRenderableObject*>::iterator m_itCurrentAnimated;

  vector<CRenderableObject*> m_vMeshes;
  vector<CRenderableObject*> m_vAnimatedModels;

};
