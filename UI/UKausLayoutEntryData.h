#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "UKausLayoutEntryData.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct FKausHUDElementEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", Meta = (Categories = "UI.Slot"))
	FGameplayTag SlotTag;
};

/**
 *  HUD DataAsset
 *  Container of Widget
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Kaus Layout Entry Data", ShortTooltip = "Defines the widgets to be created in the HUD Layout."))
class KAUS_API UKausLayoutEntryData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Kaus|UI")
	TArray<FKausHUDElementEntry> Widgets;
};
