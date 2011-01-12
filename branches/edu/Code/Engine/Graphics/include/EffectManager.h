#pragma once
#include "base.h"
#include "Utils\MapManager.h"

class CEffectManager : public CMapManager<CEffectTechnique>
{
private:
	typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
	TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
	CMatrix m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix,
	m_ViewProjectionMatrix;
	CMatrix m_LightViewMatrix, m_ShadowProjectionMatrix;
	CPoint3D m_CameraEye;
	CMapManager<CEffect> m_Effects;
	CEffectTechnique *m_StaticMeshTechnique;
	CEffectTechnique *m_AnimatedModelTechnique;
public:
	CEffectManager();
	~CEffectManager();
	const CMatrix & GetWorldMatrix() const;
	const CMatrix & GetProjectionMatrix() const;
	const CMatrix & GetViewMatrix() const;
	const CMatrix & GetViewProjectionMatrix();
	const CPoint3D & GetCameraEye();
	const CMatrix & GetLightViewMatrix() const;
	const CMatrix & GetShadowProjectionMatrix();
	void ActivateCamera(const CMatrix &ViewMatrix, const CMatrix &ProjectionMatrix,
	const CPoint3D &CameraEye);
	void SetWorldMatrix(const CMatrix &Matrix);
	void SetProjectionMatrix(const CMatrix &Matrix);
	void SetViewMatrix(const CMatrix &Matrix);
	void SetViewProjectionMatrix(const CMatrix &ViewProjectionMatrix);
	void SetLightViewMatrix(const CMatrix &Matrix);
	void SetShadowProjectionMatrix(const CMatrix &Matrix);
	void SetCameraEye(const CPoint3D &CameraEye);
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