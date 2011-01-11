#include "SpotLight.h"

void CSpotLight::Init(CXMLTreeNode& _XMLParams)
{
  CDirectionalLight::Init(_XMLParams);
  m_fAngle = _XMLParams.GetFloatProperty("angle",1.0f);
  m_fFallOff = _XMLParams.GetFloatProperty("fall_off",1.0f);
}