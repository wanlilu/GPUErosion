// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPUErosion.h"
#include "GPUErosionEdMode.h"

#define LOCTEXT_NAMESPACE "FGPUErosionModule"

void FGPUErosionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FGPUErosionEdMode>(FGPUErosionEdMode::EM_GPUErosionEdModeId, LOCTEXT("GPUErosionEdModeName", "GPUErosionEdMode"), FSlateIcon(), true);
}

void FGPUErosionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FGPUErosionEdMode::EM_GPUErosionEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGPUErosionModule, GPUErosion)