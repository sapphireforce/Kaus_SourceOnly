#pragma once
#include "GameplayTagContainer.h"
#include "KausUnitInitializationContext.generated.h"

USTRUCT(BlueprintType)
struct FKausUnitInitializationContext
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly)
    FGameplayTag UnitTag;

    UPROPERTY(BlueprintReadOnly)
    float Level = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> AvatarActor = nullptr;

    FKausUnitInitializationContext() {}
    FKausUnitInitializationContext(FGameplayTag InTag, float InLevel, AActor* InAvatar)
        : UnitTag(InTag), Level(InLevel), AvatarActor(InAvatar) {
    }
};