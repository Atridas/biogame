#include "RenderableAnimatedInstanceModel.h"
#include "Core.h"
#include "RenderManager.h"
#include "base.h"
#include "AnimatedModelManager.h"

bool CRenderableAnimatedInstanceModel::Init(const string& _szCoreName)
{
  if(_szCoreName != "") 
  {
    m_pAnimatedInstanceModel = ANIMATED_MANAGER->GetInstance(_szCoreName);
    
    m_BoundingBox    = m_pAnimatedInstanceModel->GetAnimatedCoreModel()->GetBoundingBox();
    m_BoundingSphere = m_pAnimatedInstanceModel->GetAnimatedCoreModel()->GetBoundingSphere();

    if(m_pAnimatedInstanceModel != 0)
      SetOk(true);
    else
      SetOk(false);

  } else
    SetOk(false);

  return IsOk();
}

void CRenderableAnimatedInstanceModel::InitFromXML(CXMLTreeNode& l_XMLObject)
{
  if(IsOk())
  {
    //Posterior us un cop la informació del max i min estiguin a la core.
    //string l_szResource         = l_XMLObject.GetPszISOProperty("resource" ,"");
    string l_szDefaultAnimation = l_XMLObject.GetPszISOProperty("cycle" ,"");
    Vect3f l_vPos               = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f));
    float l_fYaw                = l_XMLObject.GetFloatProperty("yaw") * FLOAT_PI_VALUE / 180.0f;
    float l_fPitch              = l_XMLObject.GetFloatProperty("pitch") * FLOAT_PI_VALUE / 180.0f;
    float l_fRoll               = l_XMLObject.GetFloatProperty("roll") * FLOAT_PI_VALUE / 180.0f;
    Vect3f l_vScale             = l_XMLObject.GetVect3fProperty("scale",Vect3f(1.0f));

    SetPosition(l_vPos);
    SetYaw(l_fYaw);
    SetPitch(l_fPitch);
    SetRoll(l_fRoll);
    SetScale(l_vScale);

    InitMat44();

    if(l_szDefaultAnimation != "")
      m_pAnimatedInstanceModel->BlendCycle(l_szDefaultAnimation, 0.0f);
    
  }
}

void CRenderableAnimatedInstanceModel::RenderRenderableObject(CRenderManager* _pRM, const vector<CEffect*>& _vEffects)
{
  if(m_pAnimatedInstanceModel)
    m_pAnimatedInstanceModel->Render(_pRM, _vEffects);
}



void CRenderableAnimatedInstanceModel::Release()
{
  CHECKED_DELETE(m_pAnimatedInstanceModel);
}

void CRenderableAnimatedInstanceModel::Update(float _fElapsedTime)
{
  if(m_pAnimatedInstanceModel)
  {
    m_pAnimatedInstanceModel->Update(_fElapsedTime);
  }
}