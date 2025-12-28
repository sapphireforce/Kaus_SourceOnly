#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "KausPlayerUnitComponent.generated.h"

class UKausInputConfig;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KAUS_API UKausPlayerUnitComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UKausPlayerUnitComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	bool IsReadyToBindInputs() const;

protected:
	//Ability Input
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	//Native Input
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kaus|Input")
	TObjectPtr<UKausInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kaus|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

private:
	bool bReadyToBindInputs;
};
