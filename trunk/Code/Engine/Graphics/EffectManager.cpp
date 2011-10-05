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

  SetOk(Load(false));
  return IsOk();
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

        if(!l_pEffect->Reload(m_pEffectPool))
        {
          LOGGER->AddNewLog(ELL_ERROR,"CEffectManager::Load Error reloading effect \"%s\"", l_szEffectName.c_str());

          CHECKED_DELETE(l_pEffect);

          m_Resources.erase(l_szEffectName);
        }

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

  }

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
}

void CEffectManager::ActivateCamera(const Mat44f& _mViewMatrix, const Mat44f& _mProjectionMatrix, const Vect3f& _vCameraEye, const Vect3f& _vCameraUp, const Vect3f& _vCameraRight)
{
  SetProjectionMatrix(_mProjectionMatrix);
  SetViewMatrix(_mViewMatrix);
  SetCameraEye(_vCameraEye);
  SetCameraUp(_vCameraUp);
  SetCameraRight(_vCameraRight);
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

void CEffectManager::SetLight(CLight* _pLight)
{
  m_bLightEnabled = false;
  m_iLightType = 0;
  m_fLightAngleCos = 0.0f;
  m_fLightFallOffCos = 0.0f;
  m_fLightStartRangeSQ = 0.0f;
  m_fLightEndRangeSQ = 0.0f;
  m_vLightPosition = Vect3f(0.0f);
  m_vLightDirection = Vect3f(0.0f);
  m_cLightColor = CColor(Vect4f(1.0f));
  m_bLightShadowEnabled = false;
  m_bLightDynamicOnly = false;

  m_bLightEnabled = _pLight->IsActive();

  if(m_bLightEnabled)
  {
    m_iLightType = _pLight->GetType();
    m_fLightStartRangeSQ = _pLight->GetStartRangeAttenuation();
    m_fLightStartRangeSQ *= m_fLightStartRangeSQ;
    m_fLightEndRangeSQ = _pLight->GetEndRangeAttenuation();
    m_fLightEndRangeSQ *= m_fLightEndRangeSQ;
    Vect4f l_vLightPosition = m_mViewMatrix * Vect4f(_pLight->GetPosition().x, _pLight->GetPosition().y, _pLight->GetPosition().z, 1.0f);
    m_vLightPosition = Vect3f(l_vLightPosition.x, l_vLightPosition.y, l_vLightPosition.z);
    m_cLightColor = _pLight->GetColor();
    m_bLightShadowEnabled = _pLight->GetRenderShadows();
    m_bLightDynamicOnly = _pLight->GetDynamicObjectsOnly();

    if(_pLight->GetType() == CLight::DIRECTIONAL || _pLight->GetType() == CLight::SPOT)
    {
      CDirectionalLight* l_pDirectional = (CDirectionalLight*) _pLight;

      Vect4f l_vLightDirection = m_mViewMatrix * Vect4f(l_pDirectional->GetDirection().x, l_pDirectional->GetDirection().y, l_pDirectional->GetDirection().z, 0.0f);
      m_vLightDirection = Vect3f(l_vLightDirection.x, l_vLightDirection.y, l_vLightDirection.z);
      
      if(_pLight->GetType() == CLight::SPOT)
      {
        CSpotLight* l_pSpot = (CSpotLight*) l_pDirectional;

        m_fLightAngleCos   = l_pSpot->GetAngle() * FLOAT_PI_VALUE / 180.0f;
        m_fLightFallOffCos = l_pSpot->GetFallOff() * FLOAT_PI_VALUE / 180.0f;

        m_fLightAngleCos   *= 0.5f; // agafar només la meitat
        m_fLightFallOffCos *= 0.5f;

        m_fLightAngleCos   = cosf(m_fLightAngleCos); // volem el cosinus, que calcular-lo al shader val un iogurt.
        m_fLightFallOffCos = cosf(m_fLightFallOffCos);
      }
    }
  }

  m_bLightUpdated = true;
}

void CEffectManager::LoadShaderData(CEffect* _pEffect)
{
  if(!_pEffect)
    return;

  if(m_bSpecularUpdated)
  {
    _pEffect->SetValue("SpecularActive", (BOOL)m_bSpecularActive);
    m_bSpecularUpdated = false;
  }

  if(m_bEnvironmentUpdated)
  {
    _pEffect->SetValue("EnvironmentIntensity", m_fEnvironmentIntensity);
    m_bEnvironmentUpdated = false;
  }

  if(m_bSpecularParamsUpdated)
  {
    _pEffect->SetValue("Glossiness", m_fGlossiness);
    _pEffect->SetValue("SpecularLevel", m_fSpecularLevel);
    m_bSpecularParamsUpdated = false;
  }

  if(m_bLightUpdated)
  {
    //_pEffect->SetValue("LightEnabled", m_bLightEnabled);
    _pEffect->SetValue("LightType", m_iLightType);
    _pEffect->SetValue("LightPosition", m_vLightPosition);
    _pEffect->SetValue("LightDirection", m_vLightDirection);
    _pEffect->SetValue("LightColor", Vect4f(m_cLightColor.GetRed(),m_cLightColor.GetGreen(),m_cLightColor.GetBlue(),m_cLightColor.GetAlpha()));
    _pEffect->SetValue("LightAngleCos", m_fLightAngleCos);
    _pEffect->SetValue("LightFallOffCos", m_fLightFallOffCos);
    _pEffect->SetValue("LightStartRangeSQ", m_fLightStartRangeSQ);
    _pEffect->SetValue("LightEndRangeSQ", m_fLightEndRangeSQ);
    _pEffect->SetValue("LightShadowEnabled", m_bLightShadowEnabled);
    _pEffect->SetValue("LightDynamicObjectsOnly", m_bLightDynamicOnly);

    m_bLightUpdated = false;
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

    _pEffect->SetValue("AmbientLight", l_aAmbientLight, 3);
    _pEffect->SetValue("LightsEnabled", (BOOL*) l_aLightsEnabled, MAX_LIGHTS_BY_SHADER);
    _pEffect->SetValue("LightsType", l_aLightsType, l_iNumOfLights);
    _pEffect->SetValue("LightsAngleCos", l_aLightsAngle, l_iNumOfLights);
    _pEffect->SetValue("LightsFallOffCos", l_aLightsFallOff, l_iNumOfLights);
    _pEffect->SetValue("LightsStartRangeSQ",l_aLightsStartRangeAttenuation,  l_iNumOfLights);
    _pEffect->SetValue("LightsEndRangeSQ", l_aLightsEndRangeAttenuation, l_iNumOfLights);
    _pEffect->SetValue("LightsPosition", (float*)l_aLightsPosition, l_iNumOfLights * 3);
    _pEffect->SetValue("LightsDirection", (float*)l_aLightsDirection, l_iNumOfLights * 3);
    _pEffect->SetValue("LightsColor", (float*)l_aLightsColor, l_iNumOfLights * 4);
    _pEffect->SetValue("ShadowEnabled", (BOOL*)l_aShadowsEnabled, l_iNumOfLights);
    _pEffect->SetValue("DynamicObjectsOnly", (BOOL*)l_aDynamicObjectsOnly, l_iNumOfLights);
   
    m_bLightsUpdated = false;
  }

  if(m_bSkeletonUpdated)
  {
    D3DXMATRIX l_mTransformation[MAXBONES];

    int l_iBoneCount = m_pCalHardwareModel->getBoneCount();

    for(int l_iBoneId = 0; l_iBoneId < l_iBoneCount; l_iBoneId++)
    {
      D3DXMatrixRotationQuaternion(&l_mTransformation[l_iBoneId],(CONST D3DXQUATERNION*)& m_pCalHardwareModel->getRotationBoneSpace(l_iBoneId, m_pCalSkeleton));
      CalVector l_vTranslationBoneSpace = m_pCalHardwareModel->getTranslationBoneSpace(l_iBoneId, m_pCalSkeleton);
      l_mTransformation[l_iBoneId]._14 = l_vTranslationBoneSpace.x;
      l_mTransformation[l_iBoneId]._24 = l_vTranslationBoneSpace.y;
      l_mTransformation[l_iBoneId]._34 = l_vTranslationBoneSpace.z;
    }

    float l_mMatrix[MAXBONES*3*4];

    //memset(l_mMatrix,0,sizeof(float)*MAXBONES*3*4);

    for(int l_iB = 0; l_iB < l_iBoneCount; ++l_iB)
    {
      memcpy(&l_mMatrix[l_iB*3*4], &l_mTransformation[l_iB],sizeof(float)*3*4);
    }


    _pEffect->SetValue("Bones", (float *)l_mMatrix,l_iBoneCount*3*4);
    
    m_bSkeletonUpdated = false;
  }

  if(m_bWorldMatrixUpdated)
  {
    _pEffect->SetValue("World",m_mWorldMatrix);
    m_bWorldMatrixUpdated = false;
  }

  if(m_bProjectionMatrixUpdated)
  {
    _pEffect->SetValue("Projection", m_mProjectionMatrix);
    Mat44f l_mInvProjectionMatrix = m_mProjectionMatrix.GetInverted();
    _pEffect->SetValue("InvProjection", l_mInvProjectionMatrix);
    m_bProjectionMatrixUpdated = false;
  }

  if(m_bViewMatrixUpdated)
  {
    _pEffect->SetValue("View", m_mViewMatrix);
    m_bViewMatrixUpdated = false;
  }

  if(m_bLightViewMatrixUpdated)
  {
    Mat44f l_ViewToLightProjectionMatrix = m_mViewMatrix.GetInverted();
    l_ViewToLightProjectionMatrix = l_ViewToLightProjectionMatrix * m_mLightViewMatrix;
    m_mLightViewMatrix = m_mLightViewMatrix * m_mShadowProjectionMatrix;
    _pEffect->SetValue("ViewToLightProjection", l_ViewToLightProjectionMatrix);
    m_bLightViewMatrixUpdated = false;
  }
  
  if(m_bCameraEyeUpdated)
  {
    _pEffect->SetValue("CameraPosition",m_vCameraEye);
    m_bCameraEyeUpdated = false;
  }
  
  if(m_bCameraUpUpdated)
  {
    _pEffect->SetValue("CameraUp",m_vCameraUp);
    m_bCameraUpUpdated = false;
  }
  
  if(m_bCameraRightUpdated)
  {
    _pEffect->SetValue("CameraRight",m_vCameraRight);
    m_bCameraRightUpdated = false;
  }

  if(m_bViewProjectionMatrixUpdated)
  {
    _pEffect->SetValue("ViewProjection", GetViewProjectionMatrix());
    m_bViewProjectionMatrixUpdated = false;
  }

  if(m_bWorldViewMatrixUpdated)
  {
    _pEffect->SetValue("WorldView", GetWorldViewMatrix());
    m_bWorldViewMatrixUpdated = false;
  }
  
  if(m_bWorldViewProjectionMatrixUpdated)
  {
    _pEffect->SetValue("WorldViewProjection", GetWorldViewProjectionMatrix());
    m_bWorldViewProjectionMatrixUpdated = false;
  }
  
  if(m_bTextureWidthHeightUpdated)
  {
    _pEffect->SetValue("TextureWidth" ,m_iTextureWidth);
    _pEffect->SetValue("TextureHeight",m_iTextureHeight);
    m_bTextureWidthHeightUpdated = false;
  }
  
  if(m_bViewportUpdated)
  {
    _pEffect->SetValue("ViewportWidth" ,m_iViewportWidth);
    _pEffect->SetValue("ViewportHeight",m_iViewportHeight);
    _pEffect->SetValue("ViewportX"     ,m_iViewportX);
    _pEffect->SetValue("ViewportY"     ,m_iViewportY);
    m_bViewportUpdated = false;
  }

  if(m_bAlphaFactorUpdated)
  {
    _pEffect->SetValue("AlphaFactor",m_fAlphaFactor);
    m_bAlphaFactorUpdated = false;
  }

  if(m_bPoissonBlurKernelUpdated)
  {
    _pEffect->SetValue("PoissonBlurKernel", m_pfPoissonBlurKernel, 32);
    m_bPoissonBlurKernelUpdated = false;
  }

  if(m_bGlowUpdated)
  {
    _pEffect->SetValue("GlowActive",(BOOL)m_bGlowActive);
    _pEffect->SetValue("GlowIntensity",m_fGlowIntensity);
    m_bGlowUpdated = false;
  }

  if(m_bHDRParamsUpdated)
  {
    _pEffect->SetValue("BrightPassThreshold", m_fBrightPassThreshold);
    _pEffect->SetValue("Exposure",            m_fExposure);
    _pEffect->SetValue("GaussMultiplier",     m_fGaussMultiplier);
    m_bHDRParamsUpdated = false;
  }

  if(m_bSpriteSizeUpdated)
  {
    _pEffect->SetValue("SpriteSize",(FLOAT*)&m_vSpriteSize, 2);
  }
  
  if(m_bTimeUpdated)
  {
    _pEffect->SetValue("Time",m_fTime);
  }
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


