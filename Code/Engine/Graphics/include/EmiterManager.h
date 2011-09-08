#pragma once
#ifndef ___EMITER_MANAGER_H__
#define ___EMITER_MANAGER_H__

#include "base.h"
#include "Utils/MapManager.h"
#include "EmiterInstance.h"
#include "ParticleConstants.h"
#include "EmiterCoreManager.h"

// Forward declarations -------------
class CRenderManager;
// ----------------------------------


class CEmiterManager:
  private CMapManager<CEmiterInstance>
{
public:
  CEmiterManager() {SetOk(true);};
  ~CEmiterManager(void)  {Done();};
  
  CEmiterInstance* GetEmiter(const string &_szName) const;
  CEmiterInstance* CreateEmiter(const string &_szName, const string &_szCore, const CObject3D& _Position, const Vect3f& _vDimensions, int _iMaxParticles = DEFAULT_MAX_PARTICLES_PER_EMITER, bool _bBillboardMode = false);
  CEmiterInstance* CreateBillboard(const string &_szName, const string &_szCore, const CObject3D& _Position) {return CreateEmiter(_szName, _szCore, _Position, Vect3f(1,1,1), 1, true);};
  void DeleteEmiter(const string &_szName);


  void Update(float _fDeltaTime);
  void Render(CRenderManager* _pRM);
  void DebugRender(CRenderManager* _pRM, bool _bDebugRenderBoundings = false);

private:
  friend class CEmiterCoreManager;

  void ResetEmiters();
};

#endif