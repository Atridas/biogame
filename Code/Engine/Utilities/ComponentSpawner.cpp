#include "ComponentSpawner.h"
#include "Core.h"
#include "EntityManager.h"
#include "ComponentObject3D.h"

#include <sstream>


CComponentSpawner* CComponentSpawner::AddToEntity(CGameEntity* _pEntity, bool _bActive, float _fSpawnTime, int _iMaxEnemy, EEnemyType _EnemyType)
{
  CComponentSpawner *l_pComp = new CComponentSpawner();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bActive, _fSpawnTime, _iMaxEnemy, _EnemyType))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentSpawner::Init(CGameEntity* _pEntity, bool _bActive, float _fSpawnTime, int _iMaxEnemy, EEnemyType _EnemyType)
{
  m_fTime = 0.0f;
  m_iCurrentEnemy = 0;

  SetActive(_bActive);
  m_fSpawnTime = _fSpawnTime;
  m_iMaxEnemy = _iMaxEnemy;
  m_EnemyType = _EnemyType;

  if(!(m_pCObject3D = _pEntity->GetComponent<CComponentObject3D>()))
  {
    LOGGER->AddNewLog(ELL_ERROR, "CComponentSpawner no es troba el component Object 3D.");
    SetOk(false);
  }else
    SetOk(true);

  return IsOk();
}

void CComponentSpawner::Update(float _fDeltaTime)
{
  if(m_iCurrentEnemy < m_iMaxEnemy)
  {
    m_fTime += _fDeltaTime;
    if(m_fTime >= m_fSpawnTime)
    {
      m_fTime = 0.0f;

      std::stringstream l_SStream;
      l_SStream << GetEntity()->GetName() << "_enemy" << m_iCurrentEnemy << endl;

      if(m_EnemyType == EE_MILITAR)
        ENTITY_MANAGER->InitMilitar("Player", m_pCObject3D->GetPosition(), l_SStream.str());
      else if(m_EnemyType == EE_MINER)
        ENTITY_MANAGER->InitMiner("Player", m_pCObject3D->GetPosition(), l_SStream.str());
        
      m_iCurrentEnemy++;
    }
  }else{
    SetActive(false);
  }
}