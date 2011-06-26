#pragma once

#include <base.h>

//Forward declarations---------------------
class CObject3D;
class CCamera;
class CRenderableObject;
class CRenderableAnimatedInstanceModel;
class CDirectionalLight;
class CSpotLight;
class COmniLight;
class CRenderManager;
class CMaterial;
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

  void Update(const float _fElapsedTime);
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

  void IncrementGlowMesh();
  void DecrementGlowMesh();
  void IncrementGlowAnimated();
  void DecrementGlowAnimated();
  void IncrementSpecMesh();
  void DecrementSpecMesh();
  void IncrementSpecAnimated();
  void DecrementSpecAnimated();
  void IncrementGlossMesh();
  void DecrementGlossMesh();
  void IncrementGlossAnimated();
  void DecrementGlossAnimated();
  void IncrementBumpMesh();
  void DecrementBumpMesh();
  void IncrementBumpAnimated();
  void DecrementBumpAnimated();

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

  void ActivateGui(bool _bValue) { m_bGuiActive = _bValue; };
  bool IsGuiActive() {return m_bGuiActive;};

private:
  void Release();

  void UpdatePosition(Vect3f& _PosDelta, float _fDeltaPitch, float _fDeltaYaw);
  void UpdateCamera(float _fDeltaPitch, float _fDeltaYaw);
  void FocusCurrentMesh();
  void FocusCurrentAnimatedModel();
  void ResetActions();
  void ProcessFreeMode(const float _fElapsedTime);
  void ProcessMeshMode(const float _fElapsedTime);
  void ProcessAnimatedMode(const float _fElapsedTime);
  void InitMode();
  void InitFreeMode();
  void InitMeshMode();
  void InitAnimatedMode();
  void ShowFreeModeInfo();
  void ShowMeshModeInfo();
  void ShowAnimatedModeInfo();

  void ReloadCurrentMesh();
  void ReloadCurrentAnimatedMesh();

  bool ExecuteFreeModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  bool ExecuteMeshModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  bool ExecuteAnimatedModeAction(float _fDeltaSeconds, float _fDelta, const char* _pcAction);
  string ShowMaterialProperties(const vector<CMaterial*>& _vMaterials);

  void ReloadMaterialValues(const vector<CMaterial*>& _vMaterials);

  CCamera* m_pObjectCamera;
  CObject3D* m_pTargetObject;
  CRenderableAnimatedInstanceModel* m_pCharacter;
  CDirectionalLight* m_pObjectModeLight;
  Vect3f m_vAmbientLight;
  Vect3f m_vOmniColor;
  bool m_bEnableLights;
  float m_fInitialCharacterYaw;
  bool m_bRenderLights;
  float m_fVelocity;

  Vect3i m_vMouseDelta;

  int  m_iMode;
  bool m_bGuiActive;
  bool m_bShowHelp;

  bool m_bMoveFwd;
  bool m_bMoveBack;
  bool m_bMoveLeft;
  bool m_bMoveRight;
  bool m_bMoveUp;
  bool m_bMoveDown;

  bool m_bNormalRendering;
  bool m_bShowBoxes;
  bool m_bShowSpheres;

  //material info
  float m_fGlowIntensity;
  float m_fSpecIntensity;
  float m_fGlossiness;
  float m_fBump;

  vector<CRenderableObject*>::iterator m_itCurrentMesh;
  vector<CRenderableObject*>::iterator m_itCurrentAnimated;

  vector<CRenderableObject*> m_vMeshes;
  vector<CRenderableObject*> m_vAnimatedModels;

};

