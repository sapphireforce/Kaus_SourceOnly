/*

*/
#pragma once
#include "Engine/DataAsset.h"
#include "DataTable/KausUnitStatsRow.h"
#include "GameplayTagContainer.h"
#include "KausGameData.generated.h"

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

	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void HandleDataTableChanged();
#endif

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData|Stats")
	TObjectPtr<UDataTable> UnitStatsTable;

private:
	void BuildUnitStatsCache();

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, FKausUnitStatsRow> UnitStatsCache;

};
