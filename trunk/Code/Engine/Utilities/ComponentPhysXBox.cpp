
#include "ComponentObject3D.h"
#include "PhysicActor.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "ComponentPhysXBox.h"

bool CComponentPhysXBox::Init(CGameEntity *_pEntity,
            float _fSizeX, float _fSizeY, float _fSizeZ,
            float _fPosX , float _fPosY , float _fPosZ,
            float _fDensity, int _iCollisionMask
            )
{
  assert(_pEntity->IsOk());
  SetEntity(_pEntity);

  m_pObject3D = dynamic_cast<CComponentObject3D*>(_pEntity->GetComponent(ECT_OBJECT_3D));
  assert(m_pObject3D); //TODO fer missatges d'error més elavorats

  m_pPhysXData = new CPhysicUserData(_pEntity->GetName().c_str());
  m_pPhysXData->SetPaint(true);
  m_pPhysXData->SetColor(colBLUE);

  m_pPhysXBox = new CPhysicActor(m_pPhysXData);
  if(_fDensity > 0)
  {
    m_pPhysXBox->CreateBody(_fDensity);
  }

  m_pPhysXBox->AddBoxSphape(Vect3f( _fSizeX, _fSizeY, _fSizeZ), 
                            Vect3f( _fPosX , _fPosY , _fPosZ ),
                            NULL, _iCollisionMask);


  CORE->GetPhysicsManager()->AddPhysicActor(m_pPhysXBox);
  m_pPhysXBox->SetMat44( m_pObject3D->GetMat44() );

  SetOk(true);
  return IsOk();
}

void CComponentPhysXBox::Update(float _fDeltaTime)
{
  Mat44f l_mat;
  m_pPhysXBox->GetMat44(l_mat);
  m_pObject3D->SetMat44(l_mat);
}


void CComponentPhysXBox::Release(void)
{
  //treure la box del physic manager
  CORE->GetPhysicsManager()->ReleasePhysicActor(m_pPhysXBox);

  CHECKED_DELETE(m_pPhysXBox);
  CHECKED_DELETE(m_pPhysXData);
}
