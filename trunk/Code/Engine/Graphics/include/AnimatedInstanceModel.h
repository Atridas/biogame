#pragma once
#ifndef __ANIMATED_INSTANCE_MODEL_H__
#define __ANIMATED_INSTANCE_MODEL_H__

#include "base.h"

#include <d3d9.h>

// Forward declarations -------------
class CRenderManager;
class CalModel;
class CAnimatedCoreModel;
class CTexture;
// ----------------------------------


class CAnimatedInstanceModel:
  public CBaseControl
{
public:
  CAnimatedInstanceModel(void): m_pCalModel(0),m_pAnimatedCoreModel(0),
                                m_iCurrentCycle(0),m_iCurrentAnimationId(0),
                                m_fCurrentAnimationTime(0),m_pVB(0),m_pIB(0),
                                m_iNumVtxs(0),m_iNumFaces(0)
                                {};
  ~CAnimatedInstanceModel(void) {Done();};

  void Render                 (CRenderManager *_pRM);
  void RenderModelBySoftware  (CRenderManager *_pRM);
  void Update                 (float _fElapsedTime);
  void Initialize             (CAnimatedCoreModel *_pAnimatedCoreModel);
  void InitD3D                (CRenderManager *_pRM);
  void Release                ();
  void ExecuteAction          (int _iId, float _fTime);
  void BlendCycle             (int _iId, float _fTime);
  void ClearCycle             (float _fTime);
  int GetCurrentCycle         () const              { return m_iCurrentCycle; };
  int GetCurrentAction        () const              { return m_iCurrentAnimationId; };

private:
  bool LoadVertexBuffer(CRenderManager *_pRM);
  void LoadTextures(CRenderManager *_pRM);

  CalModel*                 m_pCalModel;
  CAnimatedCoreModel*       m_pAnimatedCoreModel;
  vector<CTexture *>        m_vTextureList;
  int                       m_iCurrentCycle;
  int                       m_iCurrentAnimationId;
  float                     m_fCurrentAnimationTime;
  LPDIRECT3DVERTEXBUFFER9   m_pVB;
  LPDIRECT3DINDEXBUFFER9    m_pIB;
  int                       m_iNumVtxs;
  int                       m_iNumFaces;
};

#endif