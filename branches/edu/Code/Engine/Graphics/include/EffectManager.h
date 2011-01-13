#pragma once

#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__


#include "base.h"
#include "Utils\MapManager.h"
#include "Effect.h"
#include "EffectTechnique.h"
#include <XML/XMLTreeNode.h>

class CEffectManager : 
	public CMapManager<CEffectTechnique>,
	public CBaseControl
{
private:
	typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
	TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
	Mat44f m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix,	m_ViewProjectionMatrix;
	Mat44f m_LightViewMatrix, m_ShadowProjectionMatrix;
	Vect3f m_CameraEye;
	CMapManager<CEffect> m_Effects;
	CEffectTechnique *m_StaticMeshTechnique;
	CEffectTechnique *m_AnimatedModelTechnique;
public:
	CEffectManager();
	~CEffectManager();
	
	virtual void Init(CXMLTreeNode& _XMLParams);

	const Mat44f & GetWorldMatrix() const;
	const Mat44f & GetProjectionMatrix() const;
	const Mat44f & GetViewMatrix() const;
	const Mat44f & GetViewProjectionMatrix();
	const Vect3f & GetCameraEye();
	const Mat44f & GetLightViewMatrix() const;
	const Mat44f & GetShadowProjectionMatrix();
	void ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye);
	void SetWorldMatrix(const Mat44f &Matrix);
	void SetProjectionMatrix(const Mat44f &Matrix);
	void SetViewMatrix(const Mat44f &Matrix);
	void SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix);
	void SetLightViewMatrix(const Mat44f &Matrix);
	void SetShadowProjectionMatrix(const Mat44f &Matrix);
	void SetCameraEye(const Vect3f &CameraEye);
	void Load(const std::string &FileName);
	void Reload();
	std::string GetTechniqueEffectNameByVertexDefault(unsigned short VertexType);
	size_t GetMaxLights() const;
	CEffect * GetEffect(const std::string &Name);
	CEffectTechnique * GetEffectTechnique(const std::string &Name);
	CEffectTechnique * GetStaticMeshTechnique() const;
	void SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique);
	CEffectTechnique * GetAnimatedModelTechnique() const;
	void SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique);
	void CleanUp();
};
#endif