#include "KausPlayerState.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/KausUnitAttributeSet.h"

AKausPlayerState::AKausPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UKausAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	UnitAttributeSet = CreateDefaultSubobject<UKausUnitAttributeSet>(TEXT("HealthSet"));

	SetNetUpdateFrequency(100.0f);
}

UKausAbilitySystemComponent* AKausPlayerState::GetKausAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* AKausPlayerState::GetAbilitySystemComponent() const
{
	return nullptr;
}
