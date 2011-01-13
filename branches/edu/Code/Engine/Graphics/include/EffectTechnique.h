#pragma once

#ifndef __EFFECT_TECHNIQUE_H__
#define __EFFECT_TECHNIQUE_H__

#include "Effect.h"
#include <d3dx9.h>

class CEffectTechnique
{
private:
	bool m_UseCameraPosition;
	bool m_UseInverseProjMatrix;
	bool m_UseInverseViewMatrix;
	bool m_UseInverseWorldMatrix;
	bool m_UseLights;
	int m_NumOfLights;
	bool m_UseLightAmbientColor;
	bool m_UseProjMatrix;
	bool m_UseViewMatrix;
	bool m_UseWorldMatrix;
	bool m_UseWorldViewMatrix;
	bool m_UseWorldViewProjectionMatrix;
	bool m_UseViewProjectionMatrix;
	bool m_UseViewToLightProjectionMatrix;
	bool m_UseTime;
	CEffect *m_Effect;
	D3DXHANDLE m_D3DTechnique;
	std::string m_TechniqueName;
public:
	CEffectTechnique ():
                      m_UseCameraPosition(),
	                    m_UseInverseProjMatrix(),
	                    m_UseInverseViewMatrix(),
	                    m_UseInverseWorldMatrix(),
	                    m_UseLights(),
	                    m_NumOfLights(),
	                    m_UseLightAmbientColor(),
	                    m_UseProjMatrix(),
	                    m_UseViewMatrix(),
	                    m_UseWorldMatrix(),
	                    m_UseWorldViewMatrix(),
	                    m_UseWorldViewProjectionMatrix(),
	                    m_UseViewProjectionMatrix(),
	                    m_UseViewToLightProjectionMatrix(),
                      m_UseTime()     {};
	~CEffectTechnique ();
	inline CEffect * GetEffect() const {return m_Effect;}
	bool BeginRender();
	bool Refresh();
	//DirectX Methods Interface
	D3DXHANDLE GetD3DTechnique();
};
#endif