#include "TestShader.h"

// IMPLEMENT_SHADER_TYPE 这个宏一定要写cpp里面
IMPLEMENT_SHADER_TYPE(, TestShader, TEXT("/Plugins/GPUErosion/Shaders/TestCS.usf"), TEXT("CSTest"), SF_Compute)