#include "RenderableAnimatedInstanceModel.h"
#include "Core.h"
#include "RenderManager.h"
#include "base.h"
#include "AnimatedModelManager.h"


CRenderableAnimatedInstanceModel::CRenderableAnimatedInstanceModel(const string& _szName): CRenderableObject(_szName)
{
  
}

bool CRenderableAnimatedInstanceModel::Init(const string& _szCoreName)
{
  if(_szCoreName != "") 
  {
    m_pAnimatedInstanceModel = ANIMATED_MANAGER->GetInstance(_szCoreName);

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
    float l_fAltura             = l_XMLObject.GetFloatProperty("Altura");
    
    SetPosition(l_vPos);
    SetYaw(l_fYaw);
    SetPitch(l_fPitch);
    SetRoll(l_fRoll);

    //temporal mentre l'alçada no estigui a la core
    m_fAltura = l_fAltura;

    if(l_szDefaultAnimation != "")
      m_pAnimatedInstanceModel->BlendCycle(l_szDefaultAnimation, 0.0f);
    
  }
}

void CRenderableAnimatedInstanceModel::RenderRenderableObject(CRenderManager* _pRM)
{
  if(m_pAnimatedInstanceModel)
    m_pAnimatedInstanceModel->Render(_pRM);
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
    CRenderableObject::Update(_fElapsedTime);
  }
}