#include "DeferredPostSceneRendererStep.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Core.h"
#include "LightManager.h"
#include "Camera.h"
#include <XML\XMLTreeNode.h>
#include "RenderableVertexs.h"


bool CDeferredPostSceneRendererStep::Init(CXMLTreeNode& _treePostSceneRenderer, const string& _szDefaultRenderTarget)
{
  if(!CPostSceneRendererStep::Init(_treePostSceneRenderer, _szDefaultRenderTarget))
  {
    return false;
  }
  m_szGeometryLightShader = _treePostSceneRenderer.GetPszISOProperty("geometry_light_shader");

  return true;
}

void CDeferredPostSceneRendererStep::Release()
{
  CPostSceneRendererStep::Release();
}

void CDeferredPostSceneRendererStep::Render(CRenderManager* _pRM, CCamera* _pCamera)
{
  CLightManager* l_pLightManager = CORE->GetLightManager();
  CEffectManager* l_pEM = CORE->GetEffectManager();
  
  CEffect* l_pEffect = l_pEM->GetResource(m_szEffect);

  CEffect* l_pGeometryEffect = l_pEM->GetResource(m_szGeometryLightShader);
  //CEffect* l_pGeometryEffect = l_pEM->GetResource("White");

  int l_iStencilBits = _pRM->GetStencilBits();
  int l_iLastLight = l_iStencilBits - 1;
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  if(l_pEffect)
  {
    ActivateInputSamplers();

    const vector<CLight*>& l_vLights = l_pLightManager->GetLights();

    for(uint32 i = 0; i < l_vLights.size(); i++)
    {
      if(!l_vLights[i]->IsActive())
        continue;

      l_pEM->SetLight(l_vLights[i]);

      if(l_vLights[i]->UsesGeometryInDeferred())
      {
        l_iLastLight++;
        if(l_iLastLight == l_iStencilBits)
        {
          l_pDevice->Clear(0,0,D3DCLEAR_STENCIL,0,0,0);
          l_iLastLight = 0;
        }
        uint32 mask = 1 << l_iLastLight;
        l_pDevice->SetRenderState(D3DRS_STENCILMASK, mask);
        l_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, mask);

        l_vLights[i]->RenderDeferredLight(_pRM, l_pGeometryEffect);
      }
      else
      {
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


      /*
      if(l_vLights[i]->GetType() == CLight::OMNI)
      {
        RECT rect;

        if(l_pGeometryEffect)
        {
          l_iLastLight++;
          if(l_iLastLight == l_iStencilBits)
          {
            l_pDevice->Clear(0,0,D3DCLEAR_STENCIL,0,0,0);
            l_iLastLight = 0;
          }
          uint32 mask = 1 << l_iLastLight;
          l_pDevice->SetRenderState(D3DRS_STENCILMASK, mask);
          l_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, mask);

          Vect3f l_vPosition = l_vLights[i]->GetPosition();
          float l_fRange = l_vLights[i]->GetEndRangeAttenuation();

          //Vect3f l_fCamPosition = _pCamera->GetEye();
          //if(l_fCamPosition.SqDistance(l_vPosition) + _pCamera->GetZn() > 1.75f*1.75f*l_fRange*l_fRange)
          {
            _pRM->DrawShadedSphere(l_vPosition, l_fRange* 1.5f, l_pGeometryEffect);

            continue;
          }
          //else if(l_fCamPosition.SqDistance(l_vPosition) < l_fRange*l_fRange)
          //{
          //  _pRM->DrawShadedSphere(l_vPosition, l_fRange* 1.5f, l_pGeometryInsidelEffect);
          //
          //  continue;
          //}
          //else
          //{
          //  _pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
          //  //if(!ClipOmniLight(l_vLights[i], rect, _pCamera))
          //  //  continue;
          //  //
          //  //_pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
          //  //_pRM->GetDevice()->SetScissorRect(&rect);
          //}
        }
        else
        {
          if(!ClipOmniLight(l_vLights[i], rect, _pCamera))
            continue;
        
          _pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
          _pRM->GetDevice()->SetScissorRect(&rect);
        }
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
      */
    }

    DeactivateInputSamplers();
    //_pRM->GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    l_pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
  }
}
/*
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

  Vect3f ac = (l_vPosition - l_vCameraPos);
  float len = ac.Length();
  ac.Normalize();
  
  Vect3f up = _pCamera->GetVecUp();
  Vect3f lookAt = _pCamera->GetLookAt();
  Vect3f cameraDir = (lookAt - _pCamera->GetEye()).GetNormalized();
  Vect3f right = (up^cameraDir).GetNormalized();

  float ah = asin(right*ac);
  float av = asin(up*ac);
  //float ar = asin(l_fRange/len);
  float arh;// = asin(l_fRange/len);
  float arv;// = asin(l_fRange/len);
  
  float cV = ac * right;
  float sV = sqrt( 1 - cV*cV);
  float lenV = sV * len;
  if(lenV > l_fRange)
    arv = asin(l_fRange/lenV);
  else
    arv = asin(1.f);
  float cH = ac * up;
  float sH = sqrt( 1 - cH*cH);
  float lenH = sH * len;
  if(lenH > l_fRange)
    arh = asin(l_fRange/lenH);
  else
    arh = asin(1.f);

  float a1 = ah-arh;
  float a2 = ah+arh;
  float a3 = av-arv;
  float a4 = av+arv;

  float l_fFov = _pCamera->GetFov();
  
  a1 = max(min(a1, 7*FLOAT_PI_VALUE/16),-7*FLOAT_PI_VALUE/16);
  a2 = max(min(a2, 7*FLOAT_PI_VALUE/16),-7*FLOAT_PI_VALUE/16);
  a3 = max(min(a3, l_fFov*0.5f),-l_fFov*0.5f);
  a4 = max(min(a4, l_fFov*0.5f),-l_fFov*0.5f);

  //float l_fZn =  1/atan(_pCamera->GetFov()*0.5f);
  float l_fZn = _pCamera->GetZn();
  float l_fTop =  l_fZn * tan(a4);
  float l_fBottom =  l_fZn * tan(a3);
  float l_fLeft =  l_fZn * tan(a1);
  float l_fRight =  l_fZn * tan(a2);
  
  float cameratop  = l_fZn * tan(l_fFov*0.5f);
  float cameraleft = cameratop * _pCamera->GetAspectRatio();

  l_fTop = (l_fTop + cameratop) / (cameratop*2);
  l_fBottom = (l_fBottom + cameratop) / (cameratop*2);
  l_fTop = 1 - l_fTop;
  l_fBottom = 1 - l_fBottom;
  l_fLeft = (l_fLeft + cameraleft) / (cameraleft*2);
  l_fRight = (l_fRight + cameraleft) / (cameraleft*2);
  
  int l_iTop    = (int) (l_fTop    * l_iHeight);
  int l_iBottom = (int) (l_fBottom * l_iHeight);
  int l_iLeft   = (int) (l_fLeft   * l_iWidth );
  int l_iRight  = (int) (l_fRight  * l_iWidth );

  //RENDER_MANAGER->DrawQuad2D(Vect2i(l_iLeft,l_iTop), l_iRight - l_iLeft, l_iBottom - l_iTop, UPPER_LEFT, colCYAN);

  Vect3f p1 = l_vPosition + l_fRange*Vect3f(1.0f,0.0f,0.0f).GetRotatedZ(a4);
  Vect3f p2 = l_vPosition + l_fRange*Vect3f(1.0f,0.0f,0.0f).GetRotatedZ(a3);
  Vect3f p3 = l_vPosition + l_fRange*Vect3f(1.0f,0.0f,0.0f).GetRotatedZ(a1);
  Vect3f p4 = l_vPosition + l_fRange*Vect3f(1.0f,0.0f,0.0f).GetRotatedZ(a2);

  Mat44f t;
  t.SetIdentity();
  RENDER_MANAGER->SetTransform(t);
  RENDER_MANAGER->DrawLine(p1,l_vPosition,CColor(0,1,0,0));
  RENDER_MANAGER->DrawLine(p2,l_vPosition,CColor(0,0,1,0));
  RENDER_MANAGER->DrawLine(p3,l_vPosition,CColor(0,1,1,0));
  RENDER_MANAGER->DrawLine(p4,l_vPosition,CColor(1,0,1,0));
  RENDER_MANAGER->DrawSphere(l_vPosition,l_fRange,CColor(1,0,0,0),30);
  t.SetPos(l_vPosition);
  RENDER_MANAGER->SetTransform(t);
  RENDER_MANAGER->DrawCube(Vect3f(2*l_fRange,2*l_fRange,2*l_fRange),colMAGENTA);
  
  t.SetIdentity();
  t.Translate(p1);
  RENDER_MANAGER->SetTransform(t);
  RENDER_MANAGER->DrawAxis();
  
  t.SetIdentity();
  t.Translate(p2);
  RENDER_MANAGER->SetTransform(t);
  RENDER_MANAGER->DrawAxis();

  return false;

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
*/


bool CDeferredPostSceneRendererStep::ClipOmniLight(CLight* _pOmni, RECT& _Rect, CCamera* _pCamera)
{
  Vect3f l_vPosition = _pOmni->GetPosition();
  float l_fRange = _pOmni->GetEndRangeAttenuation();
  Vect3f l_vCameraPos = _pCamera->GetEye();

  CEffectManager* l_pEM = CORE->GetEffectManager();

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

  {
    float l_fMinX = l_vPosition.x - l_fRange;
    float l_fMaxX = l_vPosition.x + l_fRange;
    float l_fMinY = l_vPosition.y - l_fRange;
    float l_fMaxY = l_vPosition.y + l_fRange;
    float l_fMinZ = l_vPosition.z - l_fRange;
    float l_fMaxZ = l_vPosition.z + l_fRange;
    
    bool l_bInsideBox = true;
    if(l_vCameraPos.x > l_fMaxX) l_bInsideBox = false;
    if(l_vCameraPos.y > l_fMaxY) l_bInsideBox = false;
    if(l_vCameraPos.z > l_fMaxZ) l_bInsideBox = false;
    if(l_vCameraPos.x < l_fMinX) l_bInsideBox = false;
    if(l_vCameraPos.y < l_fMinY) l_bInsideBox = false;
    if(l_vCameraPos.z < l_fMinZ) l_bInsideBox = false;

    if(l_bInsideBox)
    {
	    _Rect.top    = 0;
	    _Rect.bottom = l_iHeight;
	    _Rect.left   = 0;
	    _Rect.right  = l_iWidth;

	    return true;
    }
  }

  
	//project coordinates
  D3DXMATRIX viewProjMat = l_pEM->GetViewProjectionMatrix().GetD3DXMatrix();
	D3DXVECTOR2 projBox[8];
  vector<D3DXVECTOR4> l_vProjectedPoints;

  for (int i = 0; i < 8; ++i)
  {
    D3DXVECTOR4 projPoint;
    D3DXVec3Transform(&projPoint, &bbox3D[i], &viewProjMat);

    if(projPoint.w >= 1.0f)
    {
      projPoint.x /= projPoint.w;  
      projPoint.y /= projPoint.w;

      //go to pixel coordinates
      projPoint.x = ((projPoint.x + 1.0f) / 2.0f) * l_iWidth;
      projPoint.y = ((-projPoint.y + 1.0f) / 2.0f) * l_iHeight;

      l_vProjectedPoints.push_back(projPoint);
    }
  }

  //compute 2D bounding box of projected coordinates
  int minX = 0x0FFFFFFF;
  int maxX = 0x00000000;
  int minY = 0x0FFFFFFF;
  int maxY = 0x00000000;

  bool l_bOutOfFrustum = true;

  for (int i = 0; i < (int)l_vProjectedPoints.size(); ++i)
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

  if(maxX <= 0)
  {
    return false;
  }

  if(maxY <= 0)
  {
    return false;
  }

  if(minY >= l_iHeight)
  {
    return false;
  }

  if(minX >= l_iWidth)
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

  //RENDER_MANAGER->DrawQuad2D(Vect2i(_Rect.left,_Rect.top), _Rect.right - _Rect.left, _Rect.bottom - _Rect.top, UPPER_LEFT, CColor(0,1,0,0.5f));

  return true;
}

enum ProjectionResult
{
    kProjectionEmpty,
    kProjectionPartial,
    kProjectionFull
};

/*bool CDeferredPostSceneRendererStep::ClipOmniLight(CLight* _pOmni, RECT& _Rect, CCamera* _pCamera)
{
  CEffectManager* l_pEM = CORE->GetEffectManager();
  Vect4f center = l_pEM->GetViewMatrix() * Vect4f(_pOmni->GetPosition(),1.0);
  float radius = _pOmni->GetEndRangeAttenuation();
  float focalLength =  1/atan(_pCamera->GetFov()*0.5f);

  
  int l_iWidth = 0;
  int l_iHeight = 0;
  l_pEM->GetTextureWidthHeight(l_iWidth,l_iHeight);

  float aspectRatio = _pCamera->GetAspectRatio();

	float cx = center.x;
	float cy = center.y;
	float cz = center.z;
	float r2 = radius * radius;
	
	float cx2 = cx * cx;
	float cy2 = cy * cy;
	float cz2 = cz * cz;
	float cxz2 = cx2 + cz2;
	if (cxz2 + cy2 > r2)
	{
		float left = -1.0F;
		float right = 1.0F;
		float bottom = -1.0F;
		float top = 1.0F;
		
		float rcz = 1.0F / cz;
		
		float dx = r2 * cx2 - cxz2 * (r2 - cz2);
		if (dx > 0.0F)
		{
			dx = sqrt(dx);
			float ax = 1.0F / cxz2;
			float bx = radius * cx;
			
			float nx1 = (bx + dx) * ax;
			float nx2 = (bx - dx) * ax;
			
			float nz1 = (radius - nx1 * cx) * rcz;
			float nz2 = (radius - nx2 * cx) * rcz;
			
			float pz1 = cz - radius * nz1;
			float pz2 = cz - radius * nz2;
			
			if (pz1 < 0.0F)
			{
				float x = nz1 * focalLength / nx1;
				if (nx1 > 0.0F) left = max(left, x);
				else right = min(right, x);
			}
			
			if (pz2 < 0.0F)
			{
				float x = nz2 * focalLength / nx2;
				if (nx2 > 0.0F) left = max(left, x);
				else right = min(right, x);
			}
		}
		
		float cyz2 = cy2 + cz2;
		float dy = r2 * cy2 - cyz2 * (r2 - cz2);
		if (dy > 0.0F)
		{
			dy = sqrt(dy);
			float ay = 1.0F / cyz2;
			float by = radius * cy;
			
			float ny1 = (by + dy) * ay;
			float ny2 = (by - dy) * ay;
			
			float nz1 = (radius - ny1 * cy) * rcz;
			float nz2 = (radius - ny2 * cy) * rcz;
			
			float pz1 = cz - radius * nz1;
			float pz2 = cz - radius * nz2;
			
			if (pz1 < 0.0F)
			{
				float y = nz1 * focalLength / (ny1 * aspectRatio);
				if (ny1 > 0.0F) bottom = max(bottom, y);
				else top = min(top, y);
			}
			
			if (pz2 < 0.0F)
			{
				float y = nz2 * focalLength / (ny2 * aspectRatio);
				if (ny2 > 0.0F) bottom = max(bottom, y);
				else top = min(top, y);
			}
		}
		
		if ((!(left < right)) || (!(bottom < top))) return false;

		_Rect.left = (int)left;
		_Rect.right = (int)right;
		_Rect.bottom = (int)bottom;
		_Rect.top = (int)top;
		
		return true;
	}
	
  _Rect.left = 0;
	_Rect.right = l_iWidth;
	_Rect.bottom = l_iHeight;
	_Rect.top = 0;

	return true;
}*/