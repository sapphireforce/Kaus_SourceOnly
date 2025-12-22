#pragma once

#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "KausPlayerState.generated.h"

class UKausAbilitySystemComponent;

/**
 * AKausPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class KAUS_API AKausPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AKausPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, Category = "Kaus|PlayerState")
	UKausAbilitySystemComponent* GetKausAbilitySystemComponent() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY()
	TObjectPtr<const class UKausUnitAttributeSet> UnitAttributeSet;

	UPROPERTY(VisibleAnywhere, Category = "Kaus|PlayerState")
	TObjectPtr<UKausAbilitySystemComponent> AbilitySystemComponent;

};
