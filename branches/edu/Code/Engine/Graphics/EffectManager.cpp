#include "EffectManager.h"


CEffectManager::CEffectManager(void)
{
}


CEffectManager::~CEffectManager(void)
{
}

void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye)
{

}
	
void CEffectManager::SetWorldMatrix(const Mat44f &Matrix)
{

}
	
void CEffectManager::SetProjectionMatrix(const Mat44f &Matrix)
{

}

void CEffectManager::SetViewMatrix(const Mat44f &Matrix)
{

}

void CEffectManager::SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix)
{

}
	
void CEffectManager::SetLightViewMatrix(const Mat44f &Matrix)
{

}
	
void CEffectManager::SetShadowProjectionMatrix(const Mat44f &Matrix)
{

}
	
void CEffectManager::SetCameraEye(const Vect3f &CameraEye)
{

}
	
void CEffectManager::Load(const std::string &FileName)
{

}
	
void CEffectManager::Reload()
{

}

//***************************************

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short VertexType)
{

}

size_t CEffectManager::GetMaxLights() const
{

}
	
CEffect * CEffectManager::GetEffect(const std::string &Name)
{

}
	
CEffectTechnique * CEffectManager::GetEffectTechnique(const std::string &Name)
{

}
	
CEffectTechnique * CEffectManager::GetStaticMeshTechnique() const
{

}
	
void CEffectManager::SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique)
{

}
	
CEffectTechnique * CEffectManager::GetAnimatedModelTechnique() const
{

}
	
void CEffectManager::SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique)
{

}

void CEffectManager::CleanUp()
{

}