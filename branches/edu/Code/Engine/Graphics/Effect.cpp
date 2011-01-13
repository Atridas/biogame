#include "Effect.h"


CEffect::CEffect(void)
{
}


CEffect::~CEffect(void)
{
}



void CEffect::SetNullParameters()
{
	m_WorldMatrixParameter=0;
	m_ViewMatrixParameter=0;
	m_ProjectionMatrixParameter=0;
	m_WorldViewMatrixParameter=0;
	m_ViewProjectionMatrixParameter=0;
	m_WorldViewProjectionMatrixParameter=0;
	m_ViewToLightProjectionMatrixParameter=0;
	m_LightEnabledParameter=0;
	m_LightsTypeParameter=0;
	m_LightsPositionParameter=0;
	m_LightsDirectionParameter=0;
	m_LightsAngleParameter=0;
	m_LightsColorParameter=0;
	m_LightsFallOffParameter=0;
	m_LightsStartRangeAttenuationParameter=0;
	m_LightsEndRangeAttenuationParameter=0;
	m_CameraPositionParameter=0;
	m_BonesParameter=0;
	m_TimeParameter=0;
}
void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{

}

bool CEffect::LoadEffect()
{

}

void CEffect::Unload()
{

}

bool CEffect::SetLights(size_t NumOfLights)
{

}
	
bool CEffect::Load(const std::string &FileName)
{

}
	
bool CEffect::Reload()
{

}
	
//DirectX Methods Interface
LPD3DXEFFECT CEffect::GetD3DEffect() const
{

}
	
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{

}