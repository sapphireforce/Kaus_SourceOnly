#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KausMessageTypes.generated.h"

USTRUCT(BlueprintType)
struct FKausMessage_UIToggle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShow = false;
};

USTRUCT(BlueprintType)
struct FKausMessage_PhaseChanged
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Phase")
	FGameplayTag NewPhase;

	UPROPERTY(BlueprintReadOnly, Category = "Phase")
	FGameplayTag PreviousPhase;
};