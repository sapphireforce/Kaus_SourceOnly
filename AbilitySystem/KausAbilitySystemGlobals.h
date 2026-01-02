#pragma once

#include "AbilitySystemGlobals.h"
#include "KausAbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

UCLASS(Config = Game)
class KAUS_API UKausAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	UKausAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
	
};
