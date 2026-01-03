#include "KausUnitExtensionComponent.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "Logs/KausLogChannels.h"

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

void UKausUnitExtensionComponent::InitializeAbilitySystem(UKausAbilitySystemComponent* InASC, AActor* InOwnerActor, FGameplayTag UnitID)
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

	if (GetOwner()->HasAuthority() && IsValid(UnitAbilityData))
	{
		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);

		UnitAbilityData->GiveToAbilitySystem(UnitID, AbilitySystemComponent, &GrantedHandles, GetOwner());

		UE_LOG(LogKaus, Log, TEXT("[%s] Initialized Ability Data: %s"), *GetName(), *UnitAbilityData->GetName());
	}

	if (AbilitySystemComponent->IsReady())
	{
		OnAbilitySystemInitialized.Broadcast();
	}
}

void UKausUnitExtensionComponent::UninitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		if (GetOwner()->HasAuthority())
		{
			GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
		}

		AbilitySystemComponent = nullptr;
	}
}