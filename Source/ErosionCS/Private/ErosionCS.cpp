// Copyright Epic Games, Inc. All Rights Reserved.

#include "../Public/ErosionCS.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FErosionCSModule"

void FErosionCSModule::StartupModule()
{
	//FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("MeowComputeShader"))->GetBaseDir(),TEXT("Shaders"));
	//AddShaderSourceDirectoryMapping(TEXT("/Plugins/MeowComputeShader/Shaders"), ShaderDir);
	FString ShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GPUErosion"))->GetBaseDir(),TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugins/GPUErosion/Shaders"),ShaderDir);
}

void FErosionCSModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FErosionCSModule, ErosionCS)