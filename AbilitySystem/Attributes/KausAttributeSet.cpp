#include "KausAttributeSet.h"

UKausAttributeSet::UKausAttributeSet()
{
}

UWorld* UKausAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UKausAbilitySystemComponent* UKausAttributeSet::GetKausAbilitySystemComponent() const
{
	return Cast<UKausAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

