#pragma once

#ifndef __EFFECT_H__
#define __EFFECT_H__
#define MAX_LIGHTS_BY_SHADER 4

#include "base.h"
#include <d3dx9.h>
#include <XML/XMLTreeNode.h>

class CEffect
{
private:
	std::string m_FileName;
	LPD3DXEFFECT m_Effect;
	bool m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int m_LightsType[MAX_LIGHTS_BY_SHADER];
	float m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];
	D3DXHANDLE m_WorldMatrixParameter, m_ViewMatrixParameter, m_ProjectionMatrixParameter;
	D3DXHANDLE m_WorldViewMatrixParameter, m_ViewProjectionMatrixParameter,	m_WorldViewProjectionMatrixParameter;
	D3DXHANDLE m_ViewToLightProjectionMatrixParameter;
	D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter, m_LightsPositionParameter, m_LightsDirectionParameter,
			   m_LightsAngleParameter, m_LightsColorParameter;
	D3DXHANDLE m_LightsFallOffParameter, m_LightsStartRangeAttenuationParameter, m_LightsEndRangeAttenuationParameter;
	D3DXHANDLE m_CameraPositionParameter;
	D3DXHANDLE m_BonesParameter;
	D3DXHANDLE m_TimeParameter;
	void SetNullParameters();
	void GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE	&l_Handle);
	bool LoadEffect();
	void Unload();
public:
	CEffect():
				m_FileName(" "),
				m_Effect(0),
				m_LightsEnabled(),
				m_LightsType(),
				m_LightsAngle(),
				m_LightsFallOff(),
				m_LightsStartRangeAttenuation(),
				m_LightsEndRangeAttenuation(),
				m_LightsPosition(),
				m_LightsDirection(),
				m_LightsColor(),
				m_WorldMatrixParameter(), 
				m_ViewMatrixParameter(), 
				m_ProjectionMatrixParameter(),
				m_WorldViewMatrixParameter(),
				m_ViewProjectionMatrixParameter(),
				m_WorldViewProjectionMatrixParameter(),
				m_ViewToLightProjectionMatrixParameter(),
				m_LightEnabledParameter(),
				m_LightsTypeParameter(),
				m_LightsPositionParameter(),
				m_LightsDirectionParameter(),
				m_LightsAngleParameter(),
				m_LightsColorParameter(),
				m_LightsFallOffParameter(),
				m_LightsStartRangeAttenuationParameter(),
				m_LightsEndRangeAttenuationParameter(),
				m_CameraPositionParameter(),
				m_BonesParameter(),
				m_TimeParameter()	{};
		
		
		
	~CEffect();
	
	bool SetLights(size_t NumOfLights);
	bool Load(const std::string &FileName);
	bool Reload();
	//DirectX Methods Interface
	LPD3DXEFFECT GetD3DEffect() const;
	D3DXHANDLE GetTechniqueByName(const std::string &TechniqueName);
};
#endif