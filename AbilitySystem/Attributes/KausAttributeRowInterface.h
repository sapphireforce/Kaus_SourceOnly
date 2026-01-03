#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "KausAttributeRowInterface.generated.h"

class UDataTable;
struct FKausUnitInitializationContext;

UINTERFACE(MinimalAPI)
class UKausAttributeRowInterface : public UInterface
{
	GENERATED_BODY()
};


class KAUS_API IKausAttributeRowInterface
{
	GENERATED_BODY()

public:
	virtual void InitAttributeData(const UDataTable* InitDataTable, const FKausUnitInitializationContext& InitContext) = 0;
	
};
