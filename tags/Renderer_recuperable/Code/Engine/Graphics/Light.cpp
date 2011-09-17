#include "Light.h"

void CLight::Init(CXMLTreeNode& _XMLParams)
{
    SetName(_XMLParams.GetPszProperty("name" ,"",false));
    m_vPosition = _XMLParams.GetVect3fProperty("pos",Vect3f(0.0f),false);
    m_colColor = CColor(Vect4f(_XMLParams.GetVect3fProperty("color",Vect3f(1.0f),false),1.0f));
    m_bRenderShadows = _XMLParams.GetBoolProperty("render_shadows",false,false);
    m_fStartRangeAttenuation = _XMLParams.GetFloatProperty("att_start_range",false,false);
    m_fEndRangeAttenuation = _XMLParams.GetFloatProperty("att_end_range",false,false);
    m_bDynamicObjectsOnly = _XMLParams.GetBoolProperty("dynamic_objects_only",true,false);

    SetOk(true);
}