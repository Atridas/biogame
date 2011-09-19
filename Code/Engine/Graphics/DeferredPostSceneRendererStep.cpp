#include "DeferredPostSceneRendererStep.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Core.h"
#include "LightManager.h"
#include "Camera.h"

void CDeferredPostSceneRendererStep::Release()
{
  CPostSceneRendererStep::Release();
}

void CDeferredPostSceneRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera)
{
  CLightManager* l_pLightManager = CORE->GetLightManager();
  CEffectManager* l_pEM = CORE->GetEffectManager();

  CEffect* l_pEffect = l_pEM->GetResource(m_szEffect);
  if(l_pEffect)
  {
    ActivateInputSamplers();

    const vector<CLight*>& l_vLights = l_pLightManager->GetLights();

    for(uint32 i = 0; i < l_vLights.size(); i++)
    {
      if(!l_vLights[i]->IsActive())
        continue;

      l_pEM->SetLight(l_vLights[i]);

      if(l_vLights[i]->GetType() == CLight::OMNI)
      {
        RECT rect;

        if(!ClipOmniLight(l_vLights[i], rect, _pCamera))
          continue;

        _pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
        _pRM->GetDevice()->SetScissorRect(&rect);

      }else{
        _pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
      }

      l_pEM->LoadShaderData(l_pEffect);

      LPD3DXEFFECT l_pD3DEffect = l_pEffect->GetD3DEffect();

      if(l_pD3DEffect!=NULL)
      {
        UINT l_NumPasses;
        l_pD3DEffect->Begin(&l_NumPasses, 0);
        for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
        {
          l_pD3DEffect->BeginPass(iPass);
          _pRM->DrawColoredTexturedQuad2D(m_iPos,m_iSize.x,m_iSize.y,m_Alignment,CColor(Vect4f(0,0,0,0)));
          l_pD3DEffect->EndPass();
        }
        l_pD3DEffect->End();
      }
    }

    DeactivateInputSamplers();
  }
}

bool CDeferredPostSceneRendererStep::ClipOmniLight(CLight* _pOmni, RECT& _Rect, CCamera* _pCamera)
{
  Vect3f l_vPosition = _pOmni->GetPosition();
  float l_fRange = _pOmni->GetEndRangeAttenuation();

  CEffectManager* l_pEM = CORE->GetEffectManager();

  Vect3f l_vCameraPos = _pCamera->GetEye();

  int l_iWidth = 0;
  int l_iHeight = 0;
  l_pEM->GetTextureWidthHeight(l_iWidth,l_iHeight);

  if(l_vCameraPos.SqDistance(l_vPosition) < l_fRange*l_fRange)
  {
	  _Rect.top    = 0;
	  _Rect.bottom = l_iHeight;
	  _Rect.left   = 0;
	  _Rect.right  = l_iWidth;

	  return true;
  }

	//compute 3D bounding box of light in world space
	D3DXVECTOR3 bbox3D[8];
	bbox3D[0].x = l_vPosition.x - l_fRange;  bbox3D[0].y = l_vPosition.y + l_fRange;  bbox3D[0].z = l_vPosition.z - l_fRange;
	bbox3D[1].x = l_vPosition.x + l_fRange;  bbox3D[1].y = l_vPosition.y + l_fRange;  bbox3D[1].z = l_vPosition.z - l_fRange;
	bbox3D[2].x = l_vPosition.x - l_fRange;  bbox3D[2].y = l_vPosition.y - l_fRange;  bbox3D[2].z = l_vPosition.z - l_fRange;
	bbox3D[3].x = l_vPosition.x + l_fRange;  bbox3D[3].y = l_vPosition.y - l_fRange;  bbox3D[3].z = l_vPosition.z - l_fRange;
	bbox3D[4].x = l_vPosition.x - l_fRange;  bbox3D[4].y = l_vPosition.y + l_fRange;  bbox3D[4].z = l_vPosition.z + l_fRange;
	bbox3D[5].x = l_vPosition.x + l_fRange;  bbox3D[5].y = l_vPosition.y + l_fRange;  bbox3D[5].z = l_vPosition.z + l_fRange;
	bbox3D[6].x = l_vPosition.x - l_fRange;  bbox3D[6].y = l_vPosition.y - l_fRange;  bbox3D[6].z = l_vPosition.z + l_fRange;
	bbox3D[7].x = l_vPosition.x + l_fRange;  bbox3D[7].y = l_vPosition.y - l_fRange;  bbox3D[7].z = l_vPosition.z + l_fRange;

  
	//project coordinates
  D3DXMATRIX viewProjMat = l_pEM->GetViewProjectionMatrix().GetD3DXMatrix();
	D3DXVECTOR2 projBox[8];
  vector<D3DXVECTOR4> l_vProjectedPoints;

	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR4 projPoint;
		D3DXVec3Transform(&projPoint, &bbox3D[i], &viewProjMat);
		projPoint.x /= projPoint.w;  
		projPoint.y /= projPoint.w;

		//go to pixel coordinates
		projPoint.x = ((projPoint.x + 1.0f) / 2.0f) * l_iWidth;
		projPoint.y = ((-projPoint.y + 1.0f) / 2.0f) * l_iHeight;

    l_vProjectedPoints.push_back(projPoint);
	}

	//compute 2D bounding box of projected coordinates
	int minX = 0x0FFFFFFF;
	int maxX = 0x00000000;
	int minY = 0x0FFFFFFF;
	int maxY = 0x00000000;

  bool l_bOutOfFrustum = true;

	for (int i = 0; i < 8; ++i)
	{
		int x = static_cast<int>(l_vProjectedPoints[i].x);
		int y = static_cast<int>(l_vProjectedPoints[i].y);

    if(l_vProjectedPoints[i].z > 0.0f)
    {
      l_bOutOfFrustum = false;
    }

		if (x < minX)
			minX = x;
		if (x > maxX)
			maxX = x;
		if (y < minY)
			minY = y;
		if (y > maxY)
			maxY = y;

    
	}

  if(l_bOutOfFrustum)
  {
    return false;
  }

  if(maxX < 0)
  {
    return false;
  }

  if(maxY < 0)
  {
    return false;
  }

  if(minY > l_iHeight)
  {
    return false;
  }

  if(minX > l_iWidth)
  {
    return false;
  }

  if(maxX > l_iWidth)
  {
    maxX = l_iWidth;
  }

  if(maxY > l_iHeight)
  {
    maxY = l_iHeight;
  }

	_Rect.top    = minY;
	_Rect.bottom = maxY;
	_Rect.left   = minX;
	_Rect.right  = maxX;

	return true;
}