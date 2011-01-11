#include "Light.h"

void CLight::Init(CXMLTreeNode& _XMLParams)
{
    m_szName = _XMLParams.GetPszProperty("name" ,"");
    m_vPosition = _XMLParams.GetVect3fProperty("pos",Vect3f(0.0f));
    m_colColor = CColor(Vect4f(_XMLParams.GetVect3fProperty("color",Vect3f(1.0f)),1.0f));
    m_bRenderShadows = _XMLParams.GetBoolProperty("render_shadows",false);
    m_fStartRangeAttenuation = _XMLParams.GetFloatProperty("att_start_range",false);
    m_fEndRangeAttenuation = _XMLParams.GetFloatProperty("att_end_range",false);
}