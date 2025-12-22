#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "KausUnitExtensionComponent.generated.h"

class UKausAbilitySystemComponent;

/**
 * Component that adds functionality to all Pawn classes so it can be used for characters/vehicles/etc.
 * This coordinates the initialization of other components.
 */
UCLASS()
class KAUS_API UKausUnitExtensionComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UKausUnitExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Kaus|Pawn")
	static UKausUnitExtensionComponent* FindPawnExtensionComponent(const AActor* Actor);

	/** Should be called by the owning pawn to become the avatar of the ability system. */
	void InitializeAbilitySystem(UKausAbilitySystemComponent* InASC, AActor* InOwnerActor);

	/** Should be called by the owning pawn to remove itself as the avatar of the ability system. */
	void UninitializeAbilitySystem();

	/** Gets the current ability system component, which may be owned by a different actor */
	UFUNCTION(BlueprintPure, Category = "Kaus|Pawn")
	UKausAbilitySystemComponent* GetKausAbilitySystemComponent() const;

	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY(Transient)
	TObjectPtr<UKausAbilitySystemComponent> AbilitySystemComponent;
};
