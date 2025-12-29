#pragma once

#include "KausInteractionDurationMessage.generated.h"

USTRUCT(BlueprintType)
struct KAUS_API FKausInteractionDurationMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float Duration = 0;
};
