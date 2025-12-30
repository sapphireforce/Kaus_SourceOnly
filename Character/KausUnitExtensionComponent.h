#pragma once

#include "Components/PawnComponent.h"
#include "AbilitySystemInterface.h"
#include "KausUnitExtensionComponent.generated.h"

class UKausAbilitySystemComponent;
class UAbilitySystemComponent;

/**
 * Component that adds functionality to all Pawn classes so it can be used for characters/vehicles/etc.
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KAUS_API UKausUnitExtensionComponent : public UPawnComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	UKausUnitExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Kaus|Pawn")
	static UKausUnitExtensionComponent* FindPawnExtensionComponent(const AActor* Actor);

	/** Gets the current ability system component, which may be owned by a different actor */
	UFUNCTION(BlueprintPure, Category = "Kaus|Pawn")
	UKausAbilitySystemComponent* GetKausAbilitySystemComponent() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitializeAbilitySystem(UKausAbilitySystemComponent* InASC, AActor* InOwnerActor);

	void UninitializeAbilitySystem();

	FSimpleMulticastDelegate OnAbilitySystemInitialized;

protected:
	UPROPERTY()
	TObjectPtr<UKausAbilitySystemComponent> AbilitySystemComponent;
};
