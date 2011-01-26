#include "DirectionalLight.h"

#include "RenderManager.h"
#include "SpotLight.h"
#include <d3d9.h>

void CDirectionalLight::Init(CXMLTreeNode& _XMLParams)
{
  CLight::Init(_XMLParams);
  m_vDirection = _XMLParams.GetVect3fProperty("direction",Vect3f(1.0f)).GetNormalized();
}

void CDirectionalLight::Render(CRenderManager *_pRM) const
{
    Mat44f t, r, r2, r3, total;
    
    t.SetIdentity();
    //r.SetIdentity();
    //r2.SetIdentity();
    //r3.SetIdentity();

    t.Translate(m_vPosition);
    //r.SetFromAngleY(-m_fYaw);
    //r2.SetFromAngleZ(m_fPitch);
    //r3.SetFromAngleX(m_fRoll);

    //total = t*r*r2*r3;

    _pRM->SetTransform(t);

    _pRM->DrawAxis();
    _pRM->DrawLine(v3fZERO,m_vDirection*2,colWHITE);
}


void CDirectionalLight::SetDirection(const Vect3f& _vDirection) 
{
  m_vDirection = _vDirection.GetNormalized();
  m_vDirection.GetAngles(m_fRoll, m_fYaw, m_fPitch);
};


Mat44f CDirectionalLight::GetLightViewMatrix()  const
{
  //TODO no calcular això cada vegada...
  D3DXMATRIX m_matView;
  D3DXVECTOR3 l_Eye(m_vPosition.x, m_vPosition.y, m_vPosition.z), 
              l_LookAt(m_vPosition.x + m_vDirection.x, m_vPosition.y + m_vDirection.y, m_vPosition.z + m_vDirection.z), 
              l_VUP(0.0f,1.0f,0.0f);
	D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);
  return Mat44f(m_matView);
}

Mat44f CDirectionalLight::GetLightProjectionMatrix() const
{
  float l_fAngle;

  if(GetType() == SPOT)
  {
    l_fAngle = ((CSpotLight*)this)->GetAngle()*2;
  } else {
    l_fAngle = 45.0f;
  }

  //TODO no calcular això cada vegada...
  D3DXMATRIX m_matProject;
  D3DXMatrixPerspectiveFovLH( 
        &m_matProject, 
        l_fAngle * D3DX_PI / 180.0f,            //angle de visió
        1.0f,                                   //aspect ratio
        1.0f,                                   //z near
        100.0f                                  //z far
        );

  return Mat44f(m_matProject);
}