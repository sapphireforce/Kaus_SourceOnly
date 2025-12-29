#include "KausUnitStatusComponent.h"
#include "AbilitySystem/Attributes/KausUnitAttributeSet.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "Logs/KausLogChannels.h"
#include "KausCharacter.h"
#include "System/KausGameData.h"
#include "Data/KausUnitStatsRowData.h"


UKausUnitStatusComponent::UKausUnitStatusComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;

	UnitAttrSet = CreateDefaultSubobject<UKausUnitAttributeSet>(TEXT("UnitStatusAttributeSet"));
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
		UE_LOG(LogKausAbilitySystem, Error, TEXT("KausUnitStatusComponent: UnitStatus component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogKausAbilitySystem, Error, TEXT("KausUnitStatusComponent: Cannot initialize UnitStatus component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	if (!UnitAttrSet)
	{
		UE_LOG(LogKausAbilitySystem, Error, TEXT("KausUnitStatusComponent: Cannot initialize UnitStatus component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent->AddAttributeSetSubobject(Cast<UAttributeSet>(UnitAttrSet));

	ApplyUnitStatsFromData();

	// Register to listen for attribute changes.
	UnitAttrSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	UnitAttrSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);

	HandleMaxHealthChanged(nullptr, nullptr, nullptr, 0.0f, 0.0f, UnitAttrSet->GetMaxHealth());
	HandleHealthChanged(nullptr, nullptr, nullptr, 0.0f, 0.0f, UnitAttrSet->GetHealth());
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

void UKausUnitStatusComponent::ApplyUnitStatsFromData()
{
	AKausCharacter* OwnerCharacter = Cast<AKausCharacter>(GetOwner());
	if (!OwnerCharacter || !UnitAttrSet)
	{
		return;
	}

	const UKausGameData& GameData = UKausGameData::Get();
	const FKausUnitStatsRow* Stats = GameData.GetUnitStats(OwnerCharacter->UnitID);

	if (Stats)
	{
		UnitAttrSet->InitMaxHealth(Stats->BaseHealth);
		UnitAttrSet->InitHealth(Stats->BaseHealth);

		UE_LOG(LogKaus, Log, TEXT("Initialized stats for Unit [%s]: MaxHealth %.1f"), *OwnerCharacter->UnitID.ToString(), Stats->BaseHealth);
	}
	else
	{
		UE_LOG(LogKaus, Warning, TEXT("Failed to find stats for UnitID [%s]"), *OwnerCharacter->UnitID.ToString());
		UnitAttrSet->InitMaxHealth(123.0f);
		UnitAttrSet->InitHealth(123.0f);
	}
}
