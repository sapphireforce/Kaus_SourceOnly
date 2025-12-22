#pragma once

#include "CoreMinimal.h"
#include "GameUIPolicy.h"
#include "KausGameUIPolicy.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API UKausGameUIPolicy : public UGameUIPolicy
{
	GENERATED_BODY()
	
public:
	UKausGameUIPolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
