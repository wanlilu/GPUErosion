// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPUErosionEdModeToolkit.h"
#include "GPUErosionEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "Widgets/Input/SSlider.h"

#include "EditorStyleSet.h"
#include "Styling/SlateTypes.h"
#include "SlateFwd.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "FGPUErosionEdModeToolkit"

FGPUErosionEdModeToolkit::FGPUErosionEdModeToolkit()
{
}

void FGPUErosionEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	const float SlotHeight = 40.0f;

	const FButtonStyle& EditorButtonStyle = FEditorStyle::GetWidgetStyle<FButtonStyle>("Button");
	const FCheckBoxStyle& EditorCheckBoxStyle = FEditorStyle::GetWidgetStyle<FCheckBoxStyle>("ToggleButtonCheckbox");
	const FSliderStyle& EditorSliderStyle = FEditorStyle::GetWidgetStyle<FSliderStyle>("ErosionSlider");
	const FEditableTextStyle& EditorEditableTextStyle = FEditorStyle::GetWidgetStyle<FEditableTextStyle>("EditableTextStyle");

	CheckBoxStyle = FCheckBoxStyle(EditorCheckBoxStyle);
	TypeCheckBoxStyle = FCheckBoxStyle(EditorCheckBoxStyle);
	SliderStyle = FSliderStyle(EditorSliderStyle);
	ButtonStyle = FButtonStyle(EditorButtonStyle);

	EditableTextStyle = FEditableTextStyle(EditorEditableTextStyle);
	

	SAssignNew(ToolkitWidget,SBorder)
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	.Padding(FMargin(10,10))
	[

		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[

			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.85)
			[
				SNew(SCheckBox)
				.HAlign(HAlign_Fill)
				.Style(&CheckBoxStyle)
				.IsEnabled_Lambda([this]()
				{
					if(((FGPUErosionEdMode*)GetEditorMode())->GetIsErosioning())
					{
						return false;
					}
					return true;
				})
				[
					SNew(SBorder)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.VAlign(EVerticalAlignment::VAlign_Center)
					.BorderBackgroundColor(FLinearColor(0, 0, 0, 0))
					.Padding(FMargin(10, 10))
					[
						SNew(STextBlock)
						.AutoWrapText(false)
						.Text(FText::FromString("Erosion"))
						//.Text_Raw(this, &FVolumetricCloudsPainterEdModeToolkit::PainterCheckBoxText)
						//.ToolTipText(NSLOCTEXT("CloudsPaintSettings", "RedChannelToolTip", "Colors Channels which should be influenced during Painting."))
					]
				]
				.OnCheckStateChanged_Lambda([this](ECheckBoxState newState)
				{
					if(newState==ECheckBoxState::Unchecked)
					{
						if((FGPUErosionEdMode*)GetEditorMode()!=nullptr)
						{
							((FGPUErosionEdMode*)GetEditorMode())->SetIsErosioning(true);
							((FGPUErosionEdMode*)GetEditorMode())->SetErosionTime(NeedErosionTime);
							
						}
					}
				})
				.IsChecked_Lambda([this]()
				{
					if((FGPUErosionEdMode*)GetEditorMode()!=nullptr)
					{
						if(((FGPUErosionEdMode*)GetEditorMode())->GetIsErosioning())
						{
							return ECheckBoxState::Unchecked;
						}
						else
						{
							return ECheckBoxState::Checked;
						}
					}
					return ECheckBoxState::Unchecked;
				})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.15)
			[
				SNew(SBorder)
				//.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(HAlign_Fill)
				.BorderBackgroundColor(FLinearColor(0, 0, 0, 0))
				[
					SNew(SEditableText)
					.Style(&EditableTextStyle)
					.OnTextChanged_Lambda([this](const FText& InFilterText)
					{
						((FGPUErosionEdMode*)GetEditorMode())->SetErosionSpeed(FCString::Atof(*InFilterText.ToString()));
					})
				]
			]
		]
		
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SNew(SSpacer)
			.Size(FVector2D(5.0f, 5.0f))
		]

		+SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.85)
			[
				SNew(SBorder)
				//.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(HAlign_Fill)
				.BorderBackgroundColor(FLinearColor(0, 0, 0, 0))
				[
					SNew(SSlider)
					.Style(&SliderStyle)
					.MaxValue(1.0)
					.MinValue(0.0)
					.StepSize(0.1)
					.Value_Lambda([this]()
					{
						if((FGPUErosionEdMode*)GetEditorMode()!=nullptr)
						{
							return (((FGPUErosionEdMode*)GetEditorMode())->GetErosionTime())/NeedErosionTime;
						}
						return 0.0f;
					})
					.OnValueChanged_Lambda([this](float time)
					{
						if((FGPUErosionEdMode*)GetEditorMode()!=nullptr)
						{
							((FGPUErosionEdMode*)GetEditorMode())->SetErosionTime(time);
						}
					})
				]
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.15)
			[
				SNew(SBorder)
				//.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(HAlign_Fill)
				.BorderBackgroundColor(FLinearColor(0, 0, 0, 0))
				[
					SNew(SEditableText)
					.Style(&EditableTextStyle)
					.OnTextChanged_Lambda([this](const FText& InFilterText)
					{
						this->NeedErosionTime = FCString::Atof(*InFilterText.ToString());
					})
				]
			]
			
		]
		
	];
	
	FModeToolkit::Init(InitToolkitHost);
}

FName FGPUErosionEdModeToolkit::GetToolkitFName() const
{
	return FName("GPUErosionEdMode");
}

FText FGPUErosionEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("GPUErosionEdModeToolkit", "DisplayName", "GPUErosionEdMode Tool");
}

class FEdMode* FGPUErosionEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FGPUErosionEdMode::EM_GPUErosionEdModeId);
}

#undef LOCTEXT_NAMESPACE
