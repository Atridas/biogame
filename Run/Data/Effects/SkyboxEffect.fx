#include "Globals.fx"
#include "Functions.fx"

sampler SkyboxTextureSampler : register(s0) = sampler_state
{
  MipFilter = LINEAR;
  MinFilter = LINEAR;  
  MagFilter = LINEAR;
  AddressU  = WRAP;
  AddressV  = WRAP;
};

float4 SkyboxPS(float2 _vpos: VPOS) : COLOR
{

  float2 _UV = _vpos - float2(g_ViewportX, g_ViewportY);
  _UV.x /= g_ViewportWidth ;
  _UV.y /= g_ViewportHeight;
  
  _UV.x += 0.5 / g_ViewportWidth;
  _UV.y += 0.5 / g_ViewportHeight;
  
	float3 l_vSkydirection = PositionFromZ(500,_UV);
  
  l_vSkydirection = mul( float4(l_vSkydirection, 0.0), g_InvViewMatrix).xyz;
  
  return texCUBE(SkyboxTextureSampler, normalize(l_vSkydirection));
}


technique SkyboxTechnique
{
	pass p0
	{
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;	
		CullMode = CCW;
		VertexShader = null;
		PixelShader  = compile ps_3_0 SkyboxPS();
	}
}
