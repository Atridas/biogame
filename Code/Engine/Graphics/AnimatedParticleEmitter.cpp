#include "AnimatedParticleEmitter.h"
#include "ParticleManager.h"

CAnimatedParticleEmitter::CAnimatedParticleEmitter()
{
  CParticleEmitter::CParticleEmitter();
  SetNumFiles(0);          
  SetNumColumnes(0);       
  SetTimeAnimationDiapo(0.0f);

}

/*void CAnimatedParticleEmitter::Update(float fElapsedTime, CCamera* camera)
{
  CParticleEmitter::Update(fElapsedTime,camera);

}*/

void CAnimatedParticleEmitter::Release()
{
  
}
  
void CAnimatedParticleEmitter::SetAttributes(SParticleInfo* _info)
{
  CParticleEmitter::SetAttributes(_info);
  m_iTexNumFiles = _info->m_iTexNumFiles;
  m_iTexNumColumnes = _info->m_iTexNumColumnes;
  m_fTimeAnimationDiapo = _info->m_fTimeAnimationDiapo;

}

void CAnimatedParticleEmitter::Render(CRenderManager* _pRM)
{
  CParticleEmitter::Render(_pRM);
}