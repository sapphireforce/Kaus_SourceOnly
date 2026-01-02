#pragma once
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "KausUnitStatsRow.generated.h"

USTRUCT(BlueprintType)
struct FKausUnitStatsRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    FGameplayTag UnitTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float BaseHealth = 100.0f;
};
