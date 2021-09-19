#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"


class EROSIONCS_API FRunShader
{
private:
	static void TestCS(ERHIFeatureLevel::Type FeatureLevel,
		int32 sizeX, int32 sizeY,
		FTextureResource* TmpUT,
		uint8 index,
		FTextureRenderTargetResource* resultTmpOut,
		FRHICommandListImmediate& CmdList);
public:
	static  void RunTestCS(ERHIFeatureLevel::Type FeatureLevel,
		int32 sizeX, int32 sizeY,
		FTextureResource* TmpUT,
		uint8 index,
		FTextureRenderTargetResource* resultTmpOut);
private:
	static UTexture2D* MGetRTTexture(UTextureRenderTarget2D* result);
	static UTexture2D* MCreatNewTexture(TArray<FColor>& mdata, uint32 sizex, uint32 sizey);
	static UTextureRenderTarget2D* MCreatNewTexRT(uint32 sizex, uint32 sizey, FLinearColor clearColor);
	static bool MExportUTexture2D(UTexture2D* tex, const FString& path);
public:
	static void MTest();
};