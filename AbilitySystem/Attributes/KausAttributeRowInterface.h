#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "KausAttributeRowInterface.generated.h"

struct FKausUnitStatsRow;

class UAttributeSet;

UINTERFACE(MinimalAPI)
class UKausAttributeRowInterface : public UInterface
{
	GENERATED_BODY()
};


class KAUS_API IKausAttributeRowInterface
{
	GENERATED_BODY()

public:
	virtual void ApplyDataRowToAttribute(FGameplayTag UnitID) = 0;
	
};
