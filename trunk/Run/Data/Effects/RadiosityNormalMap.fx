

#define NS_LIGHTING
#define NS_TEX0
#define NS_LIGHTMAP
#define NS_NORMALMAP
//#define NS_CAL3D
#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULAR

#include "NewShaders.fx"

technique RadiosityNormalmapTechnique {
	TECHNIQUE_BODY
}

technique RadiosityNormalmapAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}