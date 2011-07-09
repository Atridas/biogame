#include "EffectManager.h"
#include "Effect.h"
#include "Material.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Core.h"
#include "VertexCalculations.h"

#include "AnimatedCoreModel.h"
#include <cal3d/cal3d.h>

#include <XML/XMLTreeNode.h>


void CEffectManager::ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye, const Vect3f& _vCameraUp, const Vect3f& _vCameraRight)
{
  SetProjectionMatrix(_mProjectionMatrix);
  SetViewMatrix(_mViewMatrix);
  SetCameraEye(_vCameraEye);
  SetCameraUp(_vCameraUp);
  SetCameraRight(_vCameraRight);
}

//TODO: UPDATE FORMAT!!!
/* format inventat de Effects.xml:
<shaders>
  <effects>
    <effect name=".../>
    <effect name=".../>
  </effects>
  <default_techniques>
    <default_technique .../>
    <default_technique .../>
    <default_technique .../>
  </default_techniques>
  <techniques>
    <technique name=".../>
    <technique name=".../>
    <technique name=".../>
  </techniques>
</shaders>
*/

bool CEffectManager::Load(const SEffectManagerParams& _params)
{
  m_pLightManager = CORE->GetLightManager();

  m_szFileName = _params.szFile;

  HRESULT l_hResult = D3DXCreateEffectPool(&m_pEffectPool);

  switch(l_hResult)
  {
  case D3DERR_INVALIDCALL:
    LOGGER->AddNewLog(ELL_ERROR, "CEffectManager::Load Error creating EffectPool: Invalid arguments");
    return false;
    break;
  case E_FAIL:
    LOGGER->AddNewLog(ELL_ERROR, "CEffectManager::Load Error creating EffectPool: D3DXCreateEffectPool Failed");
    return false;
    break;
  default:
    break;
  }

  CreatePoissonBlur16x2(m_pfPoissonBlurKernel);

  return Load(false);
}

bool CEffectManager::Load(bool _bReload)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CEffectManager::Load Carregant el fitxer \"%s\"", m_szFileName.c_str());

  CXMLTreeNode l_treeShaders;
  if(!l_treeShaders.LoadFile(m_szFileName.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CEffectManager::Load No s'ha trobat el XML \"%s\"", m_szFileName.c_str());
    return false;
  }

  if(strcmp(l_treeShaders.GetName(),"shaders") == 0)
  {

    //--------Effects-------------
    CXMLTreeNode l_treeEffects = l_treeShaders["effects"];
    int l_iNumChildren = l_treeEffects.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeEffect = l_treeEffects(i);
      if(l_treeEffect.IsComment())
        continue;
      

      CEffect* l_pEffect = 0;

      string l_szEffectName = l_treeEffect.GetPszISOProperty("name","");

      l_pEffect = GetResource(l_szEffectName);

      if(l_pEffect)
      {
        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Reloading effect \"%s\"", l_szEffectName.c_str());
        }

        l_pEffect->Reload(m_pEffectPool);

      }else{

        if(_bReload)
        {
          LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Found new effect during reload: \"%s\"", l_szEffectName.c_str());
        }

        l_pEffect = new CEffect();

        if(l_pEffect->Init(l_treeEffect,m_pEffectPool))
        {
          AddResource(l_pEffect->GetName(),l_pEffect);
        }else{
          CHECKED_DELETE(l_pEffect);
        }
      }
    }
    //----------------------------

    //--------Default effects-------------
    CXMLTreeNode l_treeDefEffects = l_treeShaders["default_effects"];
    l_iNumChildren = l_treeDefEffects.GetNumChildren();

    LOGGER->AddNewLog(ELL_INFORMATION,"CEffectManager::Load Loading %d default_effects.", l_iNumChildren);

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeDefEffect = l_treeDefEffects(i);
      if(l_treeDefEffect.IsComment())
        continue;
      
      int l_iMaterialType = l_treeDefEffect.GetIntProperty("material_type");
      string l_szEffectName = l_treeDefEffect.GetPszISOProperty("effect","");

      m_DefaultEffectMap[l_iMaterialType] = l_szEffectName;
      
    }
  }

  m_bSemanticsUpdated = true;

  return true;
}

void CEffectManager::Reload()
{
  Load(true);
}

void CEffectManager::Release()
{
  CMapManager::Release();

  CHECKED_RELEASE(m_pEffectPool);

  m_DefaultEffectMap.clear();

  m_pForcedStaticMeshEffect = 0;
  m_pForcedAnimatedModelEffect = 0;
  
}

void CEffectManager::SetSpecularParams(float _fGlossiness, float _fSpecularLevel)
{
  if(_fGlossiness != m_fGlossiness || _fSpecularLevel != m_fSpecularLevel)
  {
    m_fGlossiness = _fGlossiness;
    m_fSpecularLevel = _fSpecularLevel;
    m_bSpecularParamsUpdated = true;
  }
}

void CEffectManager::LoadShaderData(CEffect* _pEffect)
{
  LPD3DXEFFECT l_pD3DEffect = _pEffect->GetD3DEffect();

  if(m_bSemanticsUpdated)
  {
    m_pWorldMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"World");
    m_pViewMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"View");
    m_pProjectionMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"Projection");
    m_pWorldViewMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"WorldView");
    m_pViewProjectionMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"ViewProjection");
    m_pWorldViewProjectionMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"WorldViewProjection");
    m_pViewToLightProjectionMatrixParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"ViewToLightProjection");
    m_pCameraPositionParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"CameraPosition");
    m_pCameraUpParameter       = l_pD3DEffect->GetParameterBySemantic(NULL,"CameraUp");
    m_pCameraRightParameter    = l_pD3DEffect->GetParameterBySemantic(NULL,"CameraRight");
    m_pAmbientLight = l_pD3DEffect->GetParameterBySemantic(NULL,"AmbientLight");
    m_pLightsEnabledParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsEnabled");
    m_pLightsTypeParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsType");
    m_pLightsPositionParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsPosition");
    m_pLightsDirectionParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsDirection");
    m_pLightsAngleParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsAngleCos");
    m_pLightsColorParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsColor");
    m_pLightsFallOffParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsFallOffCos");
    m_pLightsStartRangeAttenuationParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsStartRangeSQ");
    m_pLightsEndRangeAttenuationParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"LightsEndRangeSQ");
    m_pShadowsEnabledParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"ShadowEnabled");
    m_pDynamicObjectsOnly = l_pD3DEffect->GetParameterBySemantic(NULL,"DynamicObjectsOnly");
    m_pBonesParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"Bones");
    m_pTimeParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"Time");
    m_pGlowActiveParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"GlowActive");
    m_pTextureWidth = l_pD3DEffect->GetParameterBySemantic(NULL,"TextureWidth");
    m_pTextureHeight = l_pD3DEffect->GetParameterBySemantic(NULL,"TextureHeight");
    m_pPoissonBlurKernelParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"PoissonBlurKernel");
    m_pGlowIntensityParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"GlowIntensity");

    //m_pParallaxHeight = l_pD3DEffect->GetParameterBySemantic(NULL,"TextureWidth");
    m_pSpecularActiveParameter = l_pD3DEffect->GetParameterBySemantic(NULL,"SpecularActive");
    m_pGlossiness    = l_pD3DEffect->GetParameterBySemantic(NULL,"Glossiness");
    m_pSpecularLevel = l_pD3DEffect->GetParameterBySemantic(NULL,"SpecularLevel");
    m_pBump          = l_pD3DEffect->GetParameterBySemantic(NULL,"BumpAmount");

    m_bSemanticsUpdated = false;
  }

  //Parametres Especulars
  if(m_bSpecularUpdated)
  {
    l_pD3DEffect->SetBool(m_pSpecularActiveParameter,(BOOL)m_bSpecularActive);
    m_bSpecularUpdated = false;
  }

  if(m_bSpecularParamsUpdated)
  {
    l_pD3DEffect->SetFloat(m_pGlossiness,    m_fGlossiness);
    l_pD3DEffect->SetFloat(m_pSpecularLevel, m_fSpecularLevel);
    m_bSpecularParamsUpdated = false;
  }

  if(m_bLightsUpdated)
  {
    const vector<CLight*>& l_vLights = m_pLightManager->GetLights();

    int l_iNumOfLights = l_vLights.size() > MAX_LIGHTS_BY_SHADER? MAX_LIGHTS_BY_SHADER : l_vLights.size();

    float l_aAmbientLight[3];
    BOOL l_aLightsEnabled[MAX_LIGHTS_BY_SHADER];
    int l_aLightsType[MAX_LIGHTS_BY_SHADER];
    float l_aLightsAngle[MAX_LIGHTS_BY_SHADER];
    float l_aLightsFallOff[MAX_LIGHTS_BY_SHADER];
    float l_aLightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
    float l_aLightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
    Vect3f l_aLightsPosition[MAX_LIGHTS_BY_SHADER];
    Vect3f l_aLightsDirection[MAX_LIGHTS_BY_SHADER];
    CColor l_aLightsColor[MAX_LIGHTS_BY_SHADER];
    BOOL l_aShadowsEnabled[MAX_LIGHTS_BY_SHADER];
    BOOL l_aDynamicObjectsOnly[MAX_LIGHTS_BY_SHADER];

    memset(l_aLightsEnabled,FALSE,sizeof(BOOL)*MAX_LIGHTS_BY_SHADER);
    memset(l_aShadowsEnabled,FALSE,sizeof(BOOL)*MAX_LIGHTS_BY_SHADER);
    memset(l_aDynamicObjectsOnly,FALSE,sizeof(BOOL)*MAX_LIGHTS_BY_SHADER);

    const Vect3f& l_vAmbient = m_pLightManager->GetAmbientLight();
    l_aAmbientLight[0] = l_vAmbient.x;
    l_aAmbientLight[1] = l_vAmbient.y;
    l_aAmbientLight[2] = l_vAmbient.z;

    for(int i = 0; i < l_iNumOfLights; ++i)
    {
      CLight* l_pLight = l_vLights[i];

      l_aLightsEnabled[i] = l_pLight->IsActive();

      if(!l_pLight->IsActive())
      {
        continue;
      }

      l_aLightsType[i] = l_pLight->GetType();
      l_aLightsStartRangeAttenuation[i] = l_pLight->GetStartRangeAttenuation();
      l_aLightsStartRangeAttenuation[i] *= l_aLightsStartRangeAttenuation[i];
      l_aLightsEndRangeAttenuation[i] = l_pLight->GetEndRangeAttenuation();
      l_aLightsEndRangeAttenuation[i] *= l_aLightsEndRangeAttenuation[i];
      l_aLightsPosition[i] = l_pLight->GetPosition();
      l_aLightsColor[i] = l_pLight->GetColor();
      l_aShadowsEnabled[i] = l_pLight->GetRenderShadows();
      l_aDynamicObjectsOnly[i] = l_pLight->GetDynamicObjectsOnly();

      if(l_pLight->GetType() == CLight::DIRECTIONAL || l_pLight->GetType() == CLight::SPOT)
      {
        CDirectionalLight* l_pDirectional = (CDirectionalLight*) l_pLight;

        l_aLightsDirection[i] = l_pDirectional->GetDirection();
      
        if(l_pLight->GetType() == CLight::SPOT)
        {
          CSpotLight* l_pSpot = (CSpotLight*) l_pDirectional;

          l_aLightsAngle[i]   = l_pSpot->GetAngle() * FLOAT_PI_VALUE / 180.0f;
          l_aLightsFallOff[i] = l_pSpot->GetFallOff() * FLOAT_PI_VALUE / 180.0f;

          l_aLightsAngle[i]   *= 0.5f; // agafar només la meitat
          l_aLightsFallOff[i] *= 0.5f;

          l_aLightsAngle[i]   = cosf(l_aLightsAngle[i]); // volem el cosinus, que calcular-lo al shader val un iogurt.
          l_aLightsFallOff[i] = cosf(l_aLightsFallOff[i]);
        }
      }
    }

    l_pD3DEffect->SetFloatArray   (m_pAmbientLight,                        l_aAmbientLight,                 3);
    l_pD3DEffect->SetBoolArray    (m_pLightsEnabledParameter,      (BOOL*) l_aLightsEnabled,                MAX_LIGHTS_BY_SHADER);
    l_pD3DEffect->SetIntArray     (m_pLightsTypeParameter,                 l_aLightsType,                   l_iNumOfLights);
    l_pD3DEffect->SetFloatArray   (m_pLightsAngleParameter,                l_aLightsAngle,                  l_iNumOfLights);
    l_pD3DEffect->SetFloatArray   (m_pLightsFallOffParameter,              l_aLightsFallOff,                l_iNumOfLights);
    l_pD3DEffect->SetFloatArray   (m_pLightsStartRangeAttenuationParameter,l_aLightsStartRangeAttenuation,  l_iNumOfLights);
    l_pD3DEffect->SetFloatArray   (m_pLightsEndRangeAttenuationParameter,  l_aLightsEndRangeAttenuation,    l_iNumOfLights);
    l_pD3DEffect->SetFloatArray   (m_pLightsPositionParameter,     (float*)l_aLightsPosition,               l_iNumOfLights * 3);
    l_pD3DEffect->SetFloatArray   (m_pLightsDirectionParameter,    (float*)l_aLightsDirection,              l_iNumOfLights * 3);
    l_pD3DEffect->SetFloatArray   (m_pLightsColorParameter,        (float*)l_aLightsColor,                  l_iNumOfLights * 4);
    l_pD3DEffect->SetBoolArray    (m_pShadowsEnabledParameter,      (BOOL*)l_aShadowsEnabled,               l_iNumOfLights);
    l_pD3DEffect->SetBoolArray    (m_pDynamicObjectsOnly,           (BOOL*)l_aDynamicObjectsOnly,           l_iNumOfLights);
   
    m_bLightsUpdated = false;
  }

  if(m_bSkeletonUpdated)
  {
    D3DXMATRIX l_mTransformation[MAXBONES];

    for(int l_iBoneId = 0; l_iBoneId < m_pCalHardwareModel->getBoneCount(); l_iBoneId++)
    {
      D3DXMatrixRotationQuaternion(&l_mTransformation[l_iBoneId],(CONST D3DXQUATERNION*)& m_pCalHardwareModel->getRotationBoneSpace(l_iBoneId, m_pCalSkeleton));
      CalVector l_vTranslationBoneSpace = m_pCalHardwareModel->getTranslationBoneSpace(l_iBoneId, m_pCalSkeleton);
      l_mTransformation[l_iBoneId]._14 = l_vTranslationBoneSpace.x;
      l_mTransformation[l_iBoneId]._24 = l_vTranslationBoneSpace.y;
      l_mTransformation[l_iBoneId]._34 = l_vTranslationBoneSpace.z;
    }

    float l_mMatrix[MAXBONES*3*4];

    for(int l_iB = 0; l_iB < m_pCalHardwareModel->getBoneCount(); ++l_iB)
    {
      memcpy(&l_mMatrix[l_iB*3*4], &l_mTransformation[l_iB],sizeof(float)*3*4);
    }

    l_pD3DEffect->SetFloatArray(m_pBonesParameter, (float *)l_mMatrix,(m_pCalHardwareModel->getBoneCount())*3*4);
    
    m_bSkeletonUpdated = false;
  }

  if(m_bWorldMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pWorldMatrixParameter, &(m_mWorldMatrix.GetD3DXMatrix()));
    m_bWorldMatrixUpdated = false;
  }

  if(m_bProjectionMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pProjectionMatrixParameter, &(m_mProjectionMatrix.GetD3DXMatrix()));
    m_bProjectionMatrixUpdated = false;
  }

  if(m_bViewMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pViewMatrixParameter, &(m_mViewMatrix.GetD3DXMatrix()));
    m_bViewMatrixUpdated = false;
  }

  if(m_bLightViewMatrixUpdated)
  {
    D3DXMATRIX  l_ViewToLightProjectionMatrix = m_mViewMatrix.GetD3DXMatrix();
    D3DXMatrixInverse(&l_ViewToLightProjectionMatrix, NULL, &l_ViewToLightProjectionMatrix);
    l_ViewToLightProjectionMatrix = l_ViewToLightProjectionMatrix * m_mLightViewMatrix.GetD3DXMatrix();
    l_ViewToLightProjectionMatrix = l_ViewToLightProjectionMatrix * m_mShadowProjectionMatrix.GetD3DXMatrix();
    l_pD3DEffect->SetMatrix(m_pViewToLightProjectionMatrixParameter, &l_ViewToLightProjectionMatrix);
    m_bLightViewMatrixUpdated = false;
  }
  
  if(m_bCameraEyeUpdated)
  {
    const float l_vfPos[3] = {m_vCameraEye.x,m_vCameraEye.y,m_vCameraEye.z};
    l_pD3DEffect->SetFloatArray(m_pCameraPositionParameter,l_vfPos,3);
    m_bCameraEyeUpdated = false;
  }
  
  if(m_bCameraUpUpdated)
  {
    const float l_vfUp[3] = {m_vCameraUp.x,m_vCameraUp.y,m_vCameraUp.z};
    l_pD3DEffect->SetFloatArray(m_pCameraUpParameter,l_vfUp,3);
    m_bCameraUpUpdated = false;
  }
  
  if(m_bCameraRightUpdated)
  {
    const float l_vfRight[3] = {m_vCameraRight.x,m_vCameraRight.y,m_vCameraRight.z};
    l_pD3DEffect->SetFloatArray(m_pCameraRightParameter,l_vfRight,3);
    m_bCameraRightUpdated = false;
  }

  if(m_bViewProjectionMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pViewProjectionMatrixParameter, &(GetViewProjectionMatrix().GetD3DXMatrix()));
    m_bViewProjectionMatrixUpdated = false;
  }

  if(m_bWorldViewMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pWorldViewMatrixParameter, &(GetWorldViewMatrix().GetD3DXMatrix()));
    m_bWorldViewMatrixUpdated = false;
  }
  
  if(m_bWorldViewProjectionMatrixUpdated)
  {
    l_pD3DEffect->SetMatrix(m_pWorldViewProjectionMatrixParameter, &(GetWorldViewProjectionMatrix().GetD3DXMatrix()));
    m_bWorldViewProjectionMatrixUpdated = false;
  }
  
  if(m_bTextureWidthHeightUpdated)
  {
    l_pD3DEffect->SetInt(m_pTextureWidth,m_iTextureWidth);
    l_pD3DEffect->SetInt(m_pTextureHeight,m_iTextureHeight);
    m_bTextureWidthHeightUpdated = false;
  }

  if(m_bPoissonBlurKernelUpdated)
  {
    l_pD3DEffect->SetFloatArray(m_pPoissonBlurKernelParameter, m_pfPoissonBlurKernel, 32);
    m_bPoissonBlurKernelUpdated = false;
  }

  if(m_bGlowUpdated)
  {
    l_pD3DEffect->SetBool(m_pGlowActiveParameter,(BOOL)m_bGlowActive);
    l_pD3DEffect->SetFloat(m_pGlowIntensityParameter,(FLOAT)m_fGlowIntensity);
    m_bGlowUpdated = false;
  }

  if(m_bBumpUpdated)
  {
    l_pD3DEffect->SetFloat(m_pBump,(FLOAT)m_fBump);
    m_bBumpUpdated = false;
  }
  
}

CEffect* CEffectManager::ActivateMaterial(CMaterial* _pMaterial)
{
  CEffect* l_pEffect = 0;
  
  int l_iMaterialType = _pMaterial->GetMaterialType();
  bool l_bStaticMesh = (l_iMaterialType & ANIMATED_MESH_MATERIAL_MASK) == 0;
  bool l_bAnimatedMesh = (l_iMaterialType & ANIMATED_MESH_MATERIAL_MASK) > 0;

  if(l_bStaticMesh && m_pForcedStaticMeshEffect)
  {
    l_pEffect = m_pForcedStaticMeshEffect;
  }

  if(l_bAnimatedMesh && m_pForcedAnimatedModelEffect)
  {
    l_pEffect = m_pForcedAnimatedModelEffect;
  }

  if(!l_pEffect || !l_pEffect->IsOk())
  {
    l_pEffect = GetResource(m_DefaultEffectMap[l_iMaterialType]);

    if(l_pEffect && l_pEffect->IsOk())
    {
      float l_fParallax = _pMaterial->GetParallaxHeight();
      SetBump(_pMaterial->GetBump());

    }else{
      l_pEffect = GetResource("White");
    }
  }

  if(l_pEffect)
  {
    SetGlow((l_iMaterialType & GLOW_MATERIAL_MASK) > 0);
    if(m_bGlowActive)
    {
      SetGlowIntensity(_pMaterial->GetGlowIntensity());
    }

    SetSpecular((l_iMaterialType & SPECULARMAP_MATERIAL_MASK) > 0);

    SetSpecularParams(_pMaterial->GetGlossiness(), _pMaterial->GetSpecularFactor());

    _pMaterial->Activate(l_pEffect->GetTextureMask());

    LoadShaderData(l_pEffect);
  }

  return l_pEffect;
}


void CEffectManager::ActivateDefaultRendering(void)
{
  CMapManager<CEffect>::TMapResource::iterator l_it = m_Resources.begin();
  CMapManager<CEffect>::TMapResource::iterator l_end = m_Resources.end();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->ActivateDefaultRendering();
  }

}

void CEffectManager::ActivateAlphaRendering(void)
{
  CMapManager<CEffect>::TMapResource::iterator l_it = m_Resources.begin();
  CMapManager<CEffect>::TMapResource::iterator l_end = m_Resources.end();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->ActivateAlphaRendering();
  }

}

void CEffectManager::ActivateInstancedRendering(void)
{
  CMapManager<CEffect>::TMapResource::iterator l_it = m_Resources.begin();
  CMapManager<CEffect>::TMapResource::iterator l_end = m_Resources.end();

  for(; l_it != l_end; ++l_it)
  {
    l_it->second->ActivateInstancedRendering();
  }

}


const Mat44f& CEffectManager::GetViewProjectionMatrix()
{
  if(m_bViewProjectionUpdated)
  {
    m_mViewProjectionMatrix = m_mProjectionMatrix * m_mViewMatrix;
    m_bViewProjectionUpdated = false;
  }
  return m_mViewProjectionMatrix;
}

const Mat44f& CEffectManager::GetWorldViewMatrix()
{
  if(m_bWorldViewUpdated)
  {
    m_mWorldViewMatrix = m_mViewMatrix * m_mWorldMatrix;
    m_bWorldViewUpdated = false;
  }
  return m_mWorldViewMatrix;
}

const Mat44f& CEffectManager::GetWorldViewProjectionMatrix()
{
  if(m_bWorldViewProjectionUpdated)
  {
    m_mWorldViewProjectionMatrix = m_mProjectionMatrix * m_mViewMatrix * m_mWorldMatrix;
    m_bWorldViewProjectionUpdated = false;
  }
  return m_mWorldViewProjectionMatrix;
}


