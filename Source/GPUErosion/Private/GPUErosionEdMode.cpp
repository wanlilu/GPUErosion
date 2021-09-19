// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPUErosionEdMode.h"
#include "GPUErosionEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "ErosionCS/Public/RunShader.h"

const FEditorModeID FGPUErosionEdMode::EM_GPUErosionEdModeId = TEXT("EM_GPUErosionEdMode");

FGPUErosionEdMode::FGPUErosionEdMode()
{

}

FGPUErosionEdMode::~FGPUErosionEdMode()
{

}

void FGPUErosionEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FGPUErosionEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FGPUErosionEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FGPUErosionEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	//Erosion Tick
	if(bErosioning==true)
	{
		if(ErosionTime>0)
		{
			bErosioning=true;
			ErosionTime -= DeltaTime;
			
			TimeCurrent+=DeltaTime;
			if (ErosionSpeed == 0.0f || ErosionSpeed == NULL)
			{
				ErosionSpeed = 1;
			}
			if(TimeCurrent-TimeLastRun>=(1/ErosionSpeed))
			{
				TimeLastRun=TimeCurrent;
				DoOnceErosion();
			}
		}
		else
		{
			bErosioning=false;
			//begin 5
			ErosionTime=0;
			
			TimeLastRun=0;
			TimeCurrent=0;
		}
	}
	
	FEdMode::Tick(ViewportClient, DeltaTime);
}

bool FGPUErosionEdMode::UsesToolkits() const
{
	return true;
}

//控件测试
void FGPUErosionEdMode::DoOnceErosion()
{
	FRunShader::MTest();
	UE_LOG(LogTemp, Warning, TEXT("Erosion Once"));
}

bool FGPUErosionEdMode::BeginErosion()
{
	FRunShader::MTest();
	//GetWorld()->GetTimerManager().SetTimer(ErosionTimerHandle,this,&FGPUErosionEdMode::DoOnceErosion,5.0f);
	return false;
}




