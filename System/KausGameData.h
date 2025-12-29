/*

*/
#pragma once

#include "Engine/DataAsset.h"
#include "KausGameData.generated.h"

struct FKausUnitStatsRow;
struct FGameplayTag;
/**
 *  Non-mutable data asset that contains global game data.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Kaus Game Data", ShortTooltip = "Data asset containing global game data."))
class KAUS_API UKausGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UKausGameData();
	
	static const UKausGameData& Get();

	const FKausUnitStatsRow* GetUnitStats(FGameplayTag UnitTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|Stats")
	TObjectPtr<const UDataTable> UnitStatsTable;

};
