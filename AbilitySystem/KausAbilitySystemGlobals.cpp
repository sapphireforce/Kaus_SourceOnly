#include "AbilitySystem/KausAbilitySystemGlobals.h"
#include "KausAbilitySystemGlobals.h"
#include "KausGameplayEffectContext.h"

UKausAbilitySystemGlobals::UKausAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FGameplayEffectContext* UKausAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FKausGameplayEffectContext();
}
