#include "KausUnitStatusComponent.h"
#include "AbilitySystem/Attributes/KausUnitAttributeSet.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"

UKausUnitStatusComponent::UKausUnitStatusComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	UnitAttrSet = nullptr;
}

UKausUnitStatusComponent* UKausUnitStatusComponent::FindUnitStatusComponent(const AActor* Actor)
{
	return (Actor ? Actor->FindComponentByClass<UKausUnitStatusComponent>() : nullptr);
}

void UKausUnitStatusComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();

	Super::OnUnregister();
}

void UKausUnitStatusComponent::InitializeWithAbilitySystem(UKausAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("KausUnitStatusComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("KausUnitStatusComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	UnitAttrSet = AbilitySystemComponent->GetSet<UKausUnitAttributeSet>();
	if (!UnitAttrSet)
	{
		UE_LOG(LogTemp, Error, TEXT("KausUnitStatusComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	// Register to listen for attribute changes.
	UnitAttrSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	UnitAttrSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);

	//temp
	//Init Unit Status (if you need, you can make unit init data from other component(ex UnitSpawnComponent)
}

void UKausUnitStatusComponent::UninitializeFromAbilitySystem()
{
	if (UnitAttrSet)
	{
		UnitAttrSet->OnHealthChanged.RemoveAll(this);
		UnitAttrSet->OnMaxHealthChanged.RemoveAll(this);
	}

	UnitAttrSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UKausUnitStatusComponent::GetHealth() const
{
	return (UnitAttrSet ? UnitAttrSet->GetHealth() : 0.0f);
}

float UKausUnitStatusComponent::GetMaxHealth() const
{
	return (UnitAttrSet ? UnitAttrSet->GetMaxHealth() : 0.0f);
}

void UKausUnitStatusComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UKausUnitStatusComponent::HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}
