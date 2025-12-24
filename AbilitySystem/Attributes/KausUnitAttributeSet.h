#pragma once

#include "AbilitySystem/Attributes/KausAttributeSet.h"
#include "KausUnitAttributeSet.generated.h"


/**
 * UKausUnitAttributeSet
 *
 *	Class that defines attributes that are necessary for taking damage.
 *  All Game Living Unit must contain KUausUnitAttributeSet
 *	Attribute examples include: health
 */
UCLASS(BlueprintType)
class KAUS_API UKausUnitAttributeSet : public UKausAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UKausUnitAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UKausUnitAttributeSet, MaxHealth);

public:
	mutable FKausAttributeEvent OnHealthChanged;

	mutable FKausAttributeEvent OnMaxHealthChanged;
	
public:
	UKausUnitAttributeSet();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Kaus|UnitAttribute", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Kaus|UnitAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;

	float HealthBeforeAttributeChange;
};
