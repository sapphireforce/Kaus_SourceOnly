#pragma once

#include "Components/ActorComponent.h"
#include "KausUnitStatusComponent.generated.h"

class UKausAbilitySystemComponent;
class UKausUnitAttributeSet;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FKausHealth_AttributeChanged, UKausUnitStatusComponent*, UnitStatusComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS( ClassGroup=(Custom), Blueprintable)
class KAUS_API UKausUnitStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKausUnitStatusComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Kaus|UnitStatus")
	static UKausUnitStatusComponent* FindUnitStatusComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "Kaus|UnitStatus")
	void InitializeWithAbilitySystem(UKausAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category = "Kaus|UnitStatus")
	void UninitializeFromAbilitySystem();

public:
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "Kaus|UnitStatus")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Kaus|UnitStatus")
	float GetMaxHealth() const;


protected:
	virtual void OnUnregister() override;

	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);


public:
	UPROPERTY(BlueprintAssignable)
	FKausHealth_AttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FKausHealth_AttributeChanged OnMaxHealthChanged;

	/** Default Unit Status*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kaus|Attribute")
	TObjectPtr<UKausUnitAttributeSet> UnitAttrSet;

private:
	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY()
	TObjectPtr<UKausAbilitySystemComponent> AbilitySystemComponent;

};
