#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "KausUILayerConfig.generated.h"

class UCommonActivatableWidget;

USTRUCT(BlueprintType)
struct FKausLayerEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "UI", Meta = (Categories = "UI.Layer"))
	FGameplayTag LayerTag;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> WidgetClass;
};

/**
 *  Kaus UI Layer Config
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Kaus UI Layer Config"))
class KAUS_API UKausUILayerConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TArray<FKausLayerEntry> LayerEntries;
	
};
