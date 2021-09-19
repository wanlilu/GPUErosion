#include "RunShader.h"
#include "TestShader.h"
#include "RHICommandList.h"
#include "RHIDefinitions.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RenderGraphUtils.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "UnrealClient.h"

void FRunShader::TestCS(ERHIFeatureLevel::Type FeatureLevel,
		int32 sizeX, int32 sizeY,
		FTextureResource* TmpUT,
		uint8 index,
		FTextureRenderTargetResource* resultTmpOut,
		FRHICommandListImmediate& CmdList)
{
	// 传进来的
	// ————————————————————————————————————————————————————
	FTexture2DRHIRef tmpTextureRT = resultTmpOut->GetRenderTargetTexture();
	FUnorderedAccessViewRHIRef tmpTexture = RHICreateUnorderedAccessView(tmpTextureRT);
	// ————————————————————————————————————————————————————
    
	TShaderMapRef<TestShader> TestShaderRef(GetGlobalShaderMap(FeatureLevel));
	{
		TestShader::FParameters TestShaderParameter;
		{
			TestShaderParameter.Index = index;
			TestShaderParameter.tmpTexture_input = TmpUT->GetTexture2DRHI();
			TestShaderParameter.tmpTexture_out = tmpTexture;
		}
		FComputeShaderUtils::Dispatch(CmdList, TestShaderRef, TestShaderParameter, FIntVector(sizeX / 8, sizeY / 8, 1));
	}
}


// 在GameThread获取FTextureRenderTargetResource
void FRunShader::RunTestCS(ERHIFeatureLevel::Type FeatureLevel,
		int32 sizeX, int32 sizeY,
		FTextureResource* TmpUT,
		uint8 index,
		FTextureRenderTargetResource* resultTmpOut)
{
	ENQUEUE_RENDER_COMMAND(TestShaderCmd)
	(
	[=](FRHICommandListImmediate& RHICmdList)
		{
			TestCS(FeatureLevel, sizeX, sizeY, TmpUT, index, resultTmpOut,RHICmdList);
		}
	);
}




// 新建Texture
UTexture2D* FRunShader::MCreatNewTexture(TArray<FColor>& TexColors, uint32 sizex, uint32 sizey)
{
    UTexture2D* Texture = UTexture2D::CreateTransient(sizex, sizey, PF_B8G8R8A8);
 
    uint8* MipData = static_cast<uint8*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
     
    for(size_t y = 0; y < sizey; y++)
    {
        uint8* DestPtr = &MipData[(sizey - 1 - y) * sizex * sizeof(FColor)];
        FColor* SrcPtr = const_cast<FColor*>(&TexColors[(sizey - 1 - y) * sizex]);
        for(size_t x = 0; x < sizex; x++)
        {
            *DestPtr++ = SrcPtr->B;
            *DestPtr++ = SrcPtr->G;
            *DestPtr++ = SrcPtr->R;
            *DestPtr++ = SrcPtr->A;
            SrcPtr++;
        }
    }

    Texture->PlatformData->Mips[0].BulkData.Unlock();
    Texture->UpdateResource();
    
    return Texture;
}

// 导出UTexture
bool FRunShader::MExportUTexture2D(UTexture2D* tex, const FString& path)
{
    FTexture2DMipMap& mipmaps = tex->PlatformData->Mips[0];
    unsigned char* Data = (unsigned char *)mipmaps.BulkData.Lock(LOCK_READ_WRITE);
    int texturex = tex->PlatformData->SizeX;
    int texturey = tex->PlatformData->SizeY;
    TArray<FColor> colors;
    
    
    for (int32 y = 0; y < texturey; y++)
    {
        for (int32 x = 0; x < texturex; x++)
        {
            FColor bColor;
            bColor.B = Data[(y * texturex + x) * 4 + 0];
            bColor.G = Data[(y * texturex + x) * 4 + 1];
            bColor.R = Data[(y * texturex + x) * 4 + 2];
            bColor.A = Data[(y * texturex + x) * 4 + 3];
            colors.Add(bColor);
        }
    }
    mipmaps.BulkData.Unlock();
    tex->UpdateResource();

    TArray<uint8> texData;
    FImageUtils::CompressImageArray(texturex, texturey, colors, texData);
    
    return FFileHelper::SaveArrayToFile(texData, *path);
}

// 新建RT
UTextureRenderTarget2D* FRunShader::MCreatNewTexRT(uint32 sizex, uint32 sizey, FLinearColor clearColor)
{
    UTextureRenderTarget2D* result = NewObject<UTextureRenderTarget2D>(GWorld);
    check(result);
    result->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
    result->ClearColor = clearColor;
    result->bAutoGenerateMips = 1;
    result->InitAutoFormat(sizex, sizey);
    result->UpdateResourceImmediate(true);
    return result;
}

// 获取RT的Texture
UTexture2D* FRunShader::MGetRTTexture(UTextureRenderTarget2D* result)
{
    UTexture2D* resultTex = result->ConstructTexture2D(GWorld,"",EObjectFlags::RF_NoFlags, CTF_DeferCompression);
    resultTex->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    resultTex->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
    resultTex->SRGB = 0;
    resultTex->UpdateResource();
    return resultTex;
}

void FRunShader::MTest()
{
    UE_LOG(LogTemp, Display, TEXT("开始测试"));
    // 设置图片宽高
    int32 sizeX = 64;
    int32 sizeY = 64;
    // 设置input图片颜色数组
    TArray<FColor> TexColors;
    for (size_t i = 0; i < sizeX * sizeY; ++i)
    {
        TexColors.Add(FColor::Red);
    }
    // 新建Texture  红色
    UTexture2D* TmpTexture = MCreatNewTexture(TexColors,sizeX,sizeY);
    // 获取Texture Resource
    FTextureResource* TmpUT = static_cast<FTextureResource*>(TmpTexture->Resource);

    // 新建RT  蓝色
    UTextureRenderTarget2D* resultTmp = MCreatNewTexRT(sizeX,sizeY,FLinearColor::Blue);
    // 获取RT Resource
    FTextureRenderTargetResource* resultTmpOut = resultTmp->GameThread_GetRenderTargetResource();

    // 执行ComputeShader测试
    FRunShader::RunTestCS(GWorld->Scene->GetFeatureLevel(),sizeX,sizeY,TmpUT,1 /* G通道赋值*/, resultTmpOut);
    // 获取返回的RT Texture
    UTexture2D* resultTex =  MGetRTTexture(resultTmp);
    
    // 导出查看结果
    // 红色
    MExportUTexture2D(TmpTexture, "C:/FalconDev/Falcon_DevTools/Workspace/Falcon_Testbed/Plugins/GPUErosion/Intermediate/OutTexture/input.png");
    // 黄色
    MExportUTexture2D(resultTex, "C:/FalconDev/Falcon_DevTools/Workspace/Falcon_Testbed/Plugins/GPUErosion/Intermediate/OutTexture/out.png");
    
    UE_LOG(LogTemp, Display, TEXT("测试完成"));
}