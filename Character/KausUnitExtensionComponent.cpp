#include "KausUnitExtensionComponent.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"

UKausUnitExtensionComponent::UKausUnitExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UKausAbilitySystemComponent* UKausUnitExtensionComponent::GetKausAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* UKausUnitExtensionComponent::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UKausUnitExtensionComponent* UKausUnitExtensionComponent::FindPawnExtensionComponent(const AActor* Actor)
{
	return (Actor ? Actor->FindComponentByClass<UKausUnitExtensionComponent>() : nullptr);
}

void UKausUnitExtensionComponent::InitializeAbilitySystem(UKausAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		if (AbilitySystemComponent->IsReady()) return;
	}

	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	AbilitySystemComponent = InASC;

	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, InOwnerActor);

	if (AbilitySystemComponent->IsReady())
	{
		OnAbilitySystemInitialized.Broadcast();
	}
}

void UKausUnitExtensionComponent::UninitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent = nullptr;
	}
}