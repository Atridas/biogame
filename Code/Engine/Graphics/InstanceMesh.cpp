#include "InstanceMesh.h"
#include "Core.h"
#include "RenderManager.h"
#include "StaticMeshManager.h"

bool CInstanceMesh::Init(const string& _szCoreName)
{
  if(_szCoreName != "") 
  {
    m_StaticMesh = CORE->GetStaticMeshManager()->GetResource(_szCoreName);

    if(m_StaticMesh != 0)
    {
      m_BoundingBox.Init(m_StaticMesh->GetMin(),m_StaticMesh->GetMax());
      m_BoundingSphere.Init(m_StaticMesh->GetMin(),m_StaticMesh->GetMax());
      SetOk(true);
    }else
      SetOk(false);

  } else
    SetOk(false);

  return IsOk();
}

void CInstanceMesh::InitFromXML(CXMLTreeNode& l_XMLObject)
{
  if(IsOk())
  {
    //Posterior us un cop la informació del max i min estiguin a la core.
    //string l_szResource         = l_XMLObject.GetPszISOProperty("resource" ,"");
    string l_szDefaultAnimation = l_XMLObject.GetPszISOProperty("cycle" ,"", false);
    Vect3f l_vPos               = l_XMLObject.GetVect3fProperty("position",Vect3f(0.0f), false);
    float l_fYaw                = l_XMLObject.GetFloatProperty("yaw", 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    float l_fPitch              = l_XMLObject.GetFloatProperty("pitch", 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    float l_fRoll               = l_XMLObject.GetFloatProperty("roll", 0.0f, false) * FLOAT_PI_VALUE / 180.0f;
    
    SetPosition(l_vPos);
    SetYaw(l_fYaw);
    SetPitch(l_fPitch);
    SetRoll(l_fRoll);

    //Generem la matriu 4x4 amb la info anterior.
    //InitMat44();

  }
}

bool CInstanceMesh::ChangeInstance(const string& _szName)
{
  return Init(_szName);
}


bool CInstanceMesh::IsAlphaBlended() const 
{ 
  return m_StaticMesh->IsAlphaBlended(); 
};

void CInstanceMesh::Release()
{
  //res, el static mesh el peta el manager.
}

void CInstanceMesh::RenderRenderableObject(CRenderManager* _pRM, const vector<CEffect*>& _vEffects)
{
  if(m_StaticMesh)
    m_StaticMesh->Render(_pRM, _vEffects);
}