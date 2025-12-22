#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "KausGamePhaseSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API UKausGamePhaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Kaus|Phase")
	void SetPhase(FGameplayTag NewPhase);

	UFUNCTION(BlueprintCallable, Category = "Kaus|Phase")
	void PushPhase(FGameplayTag NewPhase);

	UFUNCTION(BlueprintCallable, Category = "Kaus|Phase")
	void PopPhase();

	UFUNCTION(BlueprintPure, Category = "Kaus|Phase")
	FGameplayTag GetCurrentPhase() const;

	UFUNCTION(BlueprintPure, Category = "Kaus|Phase")
	bool IsPhase(FGameplayTag PhaseTag) const;

private:
	void BroadcastPhaseChange(FGameplayTag PreviousPhase, FGameplayTag NewPhase);

private:
	UPROPERTY(Transient)
	TArray<FGameplayTag> PhaseStack;
};