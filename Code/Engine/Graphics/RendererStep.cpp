#include "RendererStep.h"
//#include "RenderTarget.h"
//#include "TextureRenderTarget.h"
//#include "BackBufferRenderTarget.h"
#include "InputSampler.h"
#include "TextureInputSampler.h"
#include "FrameBufferInputSampler.h"
#include "XML/XMLTreeNode.h"
#include "Core.h"
#include "RenderManager.h"
#include "Camera.h"
#include "EffectManager.h"

bool CRendererStep::Init(CXMLTreeNode& _treeSceneRenderer, const string& _szDefaultRenderTarget) 
{
  string l_szName = _treeSceneRenderer.GetPszISOProperty("name","",false);
  m_szRenderTarget = _treeSceneRenderer.GetPszISOProperty("render_target",_szDefaultRenderTarget.c_str(),false);
  bool l_bActive = _treeSceneRenderer.GetBoolProperty("active",true,false);
  
  m_bRenderOpaque = _treeSceneRenderer.GetBoolProperty("render_opaque",true,false);
  m_bRenderAlphas = _treeSceneRenderer.GetBoolProperty("render_alpha" ,true,false);

  m_bClearColor = _treeSceneRenderer.GetBoolProperty("clear_color",false,false);
  if(m_bClearColor)
  {
    CColor l_DefaultColor = RENDER_MANAGER->GetClearColor();

    Vect4f l_Color = _treeSceneRenderer.GetVect4fProperty("clear_color_value", 
                                                          Vect4f(l_DefaultColor.GetRed(),  
                                                                 l_DefaultColor.GetGreen(),
                                                                 l_DefaultColor.GetBlue(), 
                                                                 l_DefaultColor.GetAlpha()),
                                                          false);
	  uint32 red		= (uint32) (l_Color.x * 255);
	  uint32 green	= (uint32) (l_Color.y * 255);
	  uint32 blue		= (uint32) (l_Color.z * 255);
	  uint32 alpha	= (uint32) (l_Color.w * 255);

    m_Color = D3DCOLOR_ARGB(alpha, red, green, blue);
  }

  m_bClearDepth = _treeSceneRenderer.ExistsProperty("clear_depth");
  if(m_bClearDepth)
  {
    m_fDepth = _treeSceneRenderer.GetFloatProperty("clear_depth");
  }

  m_bClearStencil = _treeSceneRenderer.ExistsProperty("clear_stencil");
  if(m_bClearStencil)
  {
    m_iStencil = _treeSceneRenderer.GetIntProperty("clear_stencil");
  }

  SetName(l_szName);
  SetActive(l_bActive);

  SetOk(l_szName != ""); 
  return IsOk();
};

bool CRendererStep::InitInputSamplers(CXMLTreeNode& _treeInputSamplers)
{
  if(!_treeInputSamplers.Exists())
  {
    LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers no hi ha input samplers.");
    
  }else{

    int l_iNumChildren = _treeInputSamplers.GetNumChildren();
    for(int i = 0; i < l_iNumChildren; ++i)
    {
      CXMLTreeNode l_treeInputSampler = _treeInputSamplers(i);
      if(string(l_treeInputSampler.GetName()) == "input_sampler")
      {
        string l_szType = l_treeInputSampler.GetPszISOProperty("type", "frame_buffer", true);
        int l_iIndex = l_treeInputSampler.GetIntProperty("index", 0, true);
        CInputSampler *l_pInputSampler = 0;
        if(l_szType == "frame_buffer")
        {
          CFrameBufferInputSampler *l_pFBInputSampler = new CFrameBufferInputSampler();
          if(l_pFBInputSampler->Init(l_iIndex))
          {
            l_pInputSampler = l_pFBInputSampler;
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers init input_sampler frame_buffer ha fallat.");
            delete l_pFBInputSampler;
          }
        }
        else if(l_szType == "texture")
        {
          CTextureInputSampler *l_pTInputSampler = new CTextureInputSampler();
          string l_szTexturename = l_treeInputSampler.GetPszISOProperty("texture", "", true);
          if(l_pTInputSampler->Init(l_iIndex, l_szTexturename))
          {
            l_pInputSampler = l_pTInputSampler;
          }
          else
          {
            LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers init input_sampler texture ha fallat.");
            delete l_pTInputSampler;
          }
        }
        else
        {
          LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitInputSamplers tipus input_sampler desconegut: %s",l_szType.c_str());
        }

        if(l_pInputSampler)
        {
          m_vInputSamplers.push_back(l_pInputSampler);
        }

      }
      else
      {
        LOGGER->AddNewLog(ELL_WARNING,"CRendererStep::InitRenderTargets element no reconegut %s",l_treeInputSampler.GetName());
      }
    }

    
  }

  return true;
}

void CRendererStep::ActivateInputSamplers()
{
  vector<CInputSampler*>::iterator l_itSampler = m_vInputSamplers.begin();
  vector<CInputSampler*>::iterator l_itSamplerEnd = m_vInputSamplers.end();

  for(;l_itSampler != l_itSamplerEnd;++l_itSampler)
  {
    CInputSampler* l_pInputSampler = (*l_itSampler);
    l_pInputSampler->Activate();
  }
}


void CRendererStep::ClearBuffer(CRenderManager* l_pRM) const
{
  uint32 flags = 0;
  flags |= (m_bClearColor)  ? D3DCLEAR_TARGET  : 0;
  flags |= (m_bClearDepth)  ? D3DCLEAR_ZBUFFER : 0;
  flags |= (m_bClearStencil)? D3DCLEAR_STENCIL : 0;

  if(flags == 0)
    return;

  l_pRM->GetDevice()->Clear(0, NULL, flags, m_Color, m_fDepth, m_iStencil);
}

void CRendererStep::Release()
{

  vector<CInputSampler*>::iterator l_itSampler = m_vInputSamplers.begin();
  vector<CInputSampler*>::iterator l_itSamplerEnd = m_vInputSamplers.end();

  for(;l_itSampler != l_itSamplerEnd;++l_itSampler)
  {
    CHECKED_DELETE(*l_itSampler);
  }
  m_vInputSamplers.clear();
}

void CRendererStep::SetViewProjectionMatrices(CRenderManager* _pRM, CCamera* _pCamera)
{
  Mat44f l_matView;
  Mat44f l_matProjection;
  
  Vect3f l_vEye;
  Vect3f l_vUp;
  Vect3f l_vRight;
  Vect3f l_vLookat;

  l_vEye = _pCamera->GetEye();
  l_vUp  = _pCamera->GetVecUp().GetNormalized();
	l_vLookat = _pCamera->GetLookAt();
  l_vRight = (l_vUp ^ (l_vLookat - l_vEye)).GetNormalized();

  l_matView = _pRM->GetLookAtMatrix(l_vEye,l_vLookat,l_vUp);

  l_matProjection = _pRM->GetPerspectiveFOVMatrix(_pCamera->GetFov(),_pCamera->GetAspectRatio(),_pCamera->GetZn(),_pCamera->GetZf());

  _pRM->SetViewMatrix(l_matView);
  _pRM->SetProjectionMatrix(l_matProjection);
  CORE->GetEffectManager()->ActivateCamera(l_matView,l_matProjection,l_vEye,l_vUp,l_vRight);
}

void CRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera, 
                                const vector<CObject3DRenderable*>& _vOpaqueObjects, 
                                const vector<CObject3DRenderable*>& _vAlphaObjects, 
                                const vector<CObject3DRenderable*>& _vParticleEmiters)
{
  //m_pCamera = _pCamera;

  CEffectManager* l_pEM = CORE->GetEffectManager();
  l_pEM->Begin();

  ActivateInputSamplers();
  //ActivateRenderTargets(_pRM);

  SetViewProjectionMatrices(_pRM, _pCamera);

  vector<CObject3DRenderable*>::const_iterator l_it, l_end;

  if(m_bRenderOpaque)
  {

    CORE->GetEffectManager()->ActivateDefaultRendering();
  
    l_it  = _vOpaqueObjects.begin();
    l_end = _vOpaqueObjects.end  ();

    for(; l_it != l_end; ++l_it)
    {
      CObject3DRenderable* l_pRO = *l_it;
      RenderObject3DRenderable(_pRM,l_pRO);
    }
  }

  if(m_bRenderAlphas)
  {
    CORE->GetEffectManager()->ActivateAlphaRendering();

    l_it  = _vAlphaObjects.begin();
    l_end = _vAlphaObjects.end  ();
  
    for(; l_it != l_end; ++l_it)
    {
      CObject3DRenderable* l_pRO = *l_it;
      RenderObject3DRenderable(_pRM,l_pRO);
    }
  
    l_it  = _vParticleEmiters.begin();
    l_end = _vParticleEmiters.end  ();
  
    for(; l_it != l_end; ++l_it)
    {
      CObject3DRenderable* l_pEmiter = *l_it;
      RenderEmiter(_pRM,l_pEmiter);
    }
    _pRM->GetDevice()->SetStreamSourceFreq(0, 1);
    _pRM->GetDevice()->SetStreamSourceFreq(1, 1);
  }

  //DeactivateRenderTargets(_pRM);
  DeactivateInputSamplers();
}
