// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FGPUErosionEdModeToolkit : public FModeToolkit
{
public:

	FGPUErosionEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

private:
	
	FCheckBoxStyle CheckBoxStyle;
	FCheckBoxStyle TypeCheckBoxStyle;
	FSliderStyle  SliderStyle;
	FEditableTextStyle EditableTextStyle;
	FButtonStyle ButtonStyle;

	TSharedPtr<SWidget> ToolkitWidget;

private:
	float NeedErosionTime;
	bool bErosioning;
};
