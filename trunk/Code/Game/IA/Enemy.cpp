#include "Enemy.h"


void CEnemy::Init(CXMLTreeNode& _XMLParams)
{
    SetName(_XMLParams.GetPszProperty("name" ,""));
    
    
    m_fRunSpeed = (_XMLParams.GetFloatProperty("run_speed",false));
    m_fWalkSpeed = (_XMLParams.GetFloatProperty("walk_speed",false));
    m_fAtacSpeed = (_XMLParams.GetFloatProperty("atac_speed",false));
    m_fLife = (_XMLParams.GetFloatProperty("life",false));
    m_iWeaponStrength = (_XMLParams.GetIntProperty("weapon_strength",false));
    m_fViewAngle = (_XMLParams.GetFloatProperty("view_angle",false));
    m_fViewRadius = (_XMLParams.GetFloatProperty("view_radius",false));
    m_fSoundRadius = (_XMLParams.GetFloatProperty("sound_radius",false));

    SetOk(true);
}




