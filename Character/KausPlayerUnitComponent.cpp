#include "KausPlayerUnitComponent.h"
#include "Player/KausLocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Input/KausInputComponent.h"
#include "Tags/KausGameplayTags.h"

namespace KausPlayerUnit
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

UKausPlayerUnitComponent::UKausPlayerUnitComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bReadyToBindInputs(false)
{
}

void UKausPlayerUnitComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	check(DefaultMappingContext);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UKausLocalPlayer* LP = Cast<UKausLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	UKausInputComponent* KausIC = Cast<UKausInputComponent>(PlayerInputComponent);
	if (ensureMsgf(KausIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UKausInputComponent or a subclass of it.")))
	{
		TArray<uint32> BindHandles;
		KausIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		KausIC->BindNativeAction(InputConfig, KausGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
		KausIC->BindNativeAction(InputConfig, KausGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
		KausIC->BindNativeAction(InputConfig, KausGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
}

bool UKausPlayerUnitComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UKausPlayerUnitComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	//todo
}

void UKausPlayerUnitComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	//todo
}

void UKausPlayerUnitComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UKausPlayerUnitComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UKausPlayerUnitComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * KausPlayerUnit::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * KausPlayerUnit::LookPitchRate * World->GetDeltaSeconds());
	}
}
