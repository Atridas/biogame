
#define NS_LIGHTING
#define NS_TEX0
//#define NS_LIGHTMAP
//#define NS_NORMALMAP
//#define NS_CAL3D
//#define NS_RADIOSITY_NORMALMAP
//#define NS_SPECULAR
#define NS_ENVIRONMENT

#include "NewShaders.fx"

technique NormalTexturedEnvironmentTechnique {
	TECHNIQUE_BODY
}

technique NormalTexturedEnvironmentAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}