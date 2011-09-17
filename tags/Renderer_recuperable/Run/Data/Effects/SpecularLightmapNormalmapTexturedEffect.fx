#define NS_LIGHTING
#define NS_TEX0
#define NS_LIGHTMAP
#define NS_NORMALMAP
#define NS_SPECULAR

#include "NewShaders.fx"

technique SpecularLightmapNormalmapTexturedTechnique {
	TECHNIQUE_BODY
}

technique SpecularLightmapNormalmapTexturedAlphaTechnique {
	ALPHA_TECHNIQUE_BODY
}
