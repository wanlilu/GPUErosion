// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class FGPUErosionEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_GPUErosionEdModeId;
public:
	FGPUErosionEdMode();
	virtual ~FGPUErosionEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

private:
	float ErosionTime;
	float ErosionDuration;
	float ErosionSpeed;
	bool bErosioning=false;
	FTimerHandle ErosionTimerHandle;

	float TimeLastRun;
	float TimeCurrent;

public:
	//Erosion Temp
	UFUNCTION()
	void DoOnceErosion();
	void SetErosionSpeed(float Speed){ErosionSpeed=Speed;}
	float GetErosionTime(){return ErosionTime;}
	void SetErosionTime(float time){ErosionTime=time;}
	bool GetIsErosioning(){return  bErosioning;}
	void SetIsErosioning(bool bIng){bErosioning=bIng;}
	bool BeginErosion();


};
