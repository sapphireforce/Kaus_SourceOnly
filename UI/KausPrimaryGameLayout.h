#pragma once

#include "PrimaryGameLayout.h"
#include "KausPrimaryGameLayout.generated.h"

/** 
 *  Stacked Layer Mamager Layout
 */
UCLASS(Abstract, Meta = (DisplayName = "Kaus Primary Layout"))
class KAUS_API UKausPrimaryGameLayout : public UPrimaryGameLayout
{
	GENERATED_BODY()
	
public:
	UKausPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerTag, UClass* WidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerTag, WidgetClass, [](ActivatableWidgetT&) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerTag, UClass* WidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerTag))
		{
			return Layer->AddWidget<ActivatableWidgetT>(WidgetClass, InitInstanceFunc);
		}
		return nullptr;
	}
	
};
