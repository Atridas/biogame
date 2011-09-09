#define NS_LIGHTING
#define NS_TEX0
#define NS_LIGHTMAP
#define NS_NORMALMAP
#define NS_RADIOSITY_NORMALMAP
#define NS_SPECULAR

#include "NewShaders.fx"

technique SpecularRadiosityNormalmapTexturedTechnique {
	TECHNIQUE_BODY
}

technique SpecularRadiosityNormalmapTexturedAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}
