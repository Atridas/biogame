#include "Enemy.h"


void CEnemy::InitTemplate(CXMLTreeNode& _XMLParams)
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



void CEnemy::SetWiew(bool _bView)
{
	//nomes mira al player, pasar un punter de object 3d per si de cas
	//si troba al player es true, si no el veu es false

	m_vPosition;
	m_fYaw+=m_fViewAngle;
}
  
void CEnemy::SetSound(bool _bSound)
{
	//nomes mira al player, pasar un punter de object 3d per si de cas
	//si escolta al player es true, si no l'escolta es false

}

