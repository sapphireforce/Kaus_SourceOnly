#include "KausUnitAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Logs/KausLogChannels.h"
#include "DataTable/KausUnitStatsRow.h"
#include "AbilitySystem/KausUnitInitializationContext.h"

UKausUnitAttributeSet::UKausUnitAttributeSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
{
	MaxHealthBeforeAttributeChange = 0.0f;
	HealthBeforeAttributeChange = 0.0f;
}

void UKausUnitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKausUnitAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKausUnitAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UKausUnitAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UKausUnitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UKausUnitAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent();
			check(KausASC);

			KausASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

bool UKausUnitAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	//todo
	//DamageImmunity

	// Save the current health
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();

	return true;
}

void UKausUnitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	/*
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage(); 
		SetDamage(0.0f); 

		if (LocalDamage > 0.0f)
		{
			const float NewHealth = GetHealth() - LocalDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if (GetHealth() <= 0.0f)
			{
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > GetMaxHealth())
		{
			SetHealth(GetMaxHealth());
		}
	}
	*/
}

void UKausUnitAttributeSet::InitAttributeData(const UDataTable* InitDataTable, const FKausUnitInitializationContext& InitContext)
{
	if (!InitDataTable || !InitContext.UnitTag.IsValid())
	{
		return;
	}

	//todo: make row name cache like KausGameData
	const FKausUnitStatsRow* Stats = InitDataTable->FindRow<FKausUnitStatsRow>(InitContext.UnitTag.GetTagName(), TEXT("InitAttributeData"));

	if (Stats)
	{
		const float NewMaxHealth = Stats->BaseHealthCurve.GetValueAtLevel(InitContext.Level);

		InitMaxHealth(NewMaxHealth);
		InitHealth(NewMaxHealth);

		UE_LOG(LogKaus, Log, TEXT("Unit [%s] Attributes Initialized. Level: %.1f, MaxHealth: %.1f"),
			*InitContext.UnitTag.ToString(), InitContext.Level, NewMaxHealth);
	}
	else
	{
		UE_LOG(LogKaus, Warning, TEXT("Failed to find row for Unit [%s] in provided table."), *InitContext.UnitTag.ToString());

		InitMaxHealth(123456789.0f);
		InitHealth(123456789.0f);
	}
}

void UKausUnitAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UKausUnitAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKausUnitAttributeSet, Health, OldValue);

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();

	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);


}

void UKausUnitAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKausUnitAttributeSet, MaxHealth, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxHealth());
}
