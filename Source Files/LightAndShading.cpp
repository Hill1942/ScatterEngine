#include <Windows.h>
#include <ddraw.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "3DPipeLine.h"
#include "LightAndShading.h"

MATERIALV1 materials[MAX_MATERIALV1];
int        numMaterials;

LIGHTV1    lights[MAX_LIGHTS];
int        numLights;
