#pragma once

#include "Components/PawnComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/KausUnitAbilityData.h"
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

	void InitializeAbilitySystem(UKausAbilitySystemComponent* InASC, AActor* InOwnerActor, FGameplayTag UnitID);

	void UninitializeAbilitySystem();

	FSimpleMulticastDelegate OnAbilitySystemInitialized;

protected:
	UPROPERTY()
	TObjectPtr<UKausAbilitySystemComponent> AbilitySystemComponent;

	// List of ability sets to grant to actors of the specified class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kaus|Ability")
	TObjectPtr<const UKausUnitAbilityData> UnitAbilityData;

	UPROPERTY(Transient)
	FKausAbilitySet_GrantedHandles GrantedHandles;

	//TagRelationshipMapping
	

};
