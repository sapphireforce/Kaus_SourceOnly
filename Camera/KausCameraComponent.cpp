#include "KausCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"

UKausCameraComponent::UKausCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
	PrimaryComponentTick.bStartWithTickEnabled = true;

	FKausCameraProfile ExplorationProfile;
	ExplorationProfile.TargetArmLength = 400.0f;
	ExplorationProfile.TargetRotation = FRotator(-20.0f, 0.0f, 0.0f);
	ExplorationProfile.FieldOfView = 90.0f;
	ExplorationProfile.InterpSpeed = 4.0f;
	ExplorationProfile.bUsePawnControlRotation = true;
	CameraProfiles.Add(EKausCameraMode::Exploration, ExplorationProfile);

	FKausCameraProfile BattleProfile;
	BattleProfile.TargetArmLength = 800.0f;
	BattleProfile.TargetRotation = FRotator(-50.0f, 0.0f, 0.0f);
	BattleProfile.FieldOfView = 85.0f;
	BattleProfile.InterpSpeed = 3.0f;
	BattleProfile.bUsePawnControlRotation = false;
	CameraProfiles.Add(EKausCameraMode::Battle, BattleProfile);

	CurrentTargetProfile = ExplorationProfile;
	CurrentMode = EKausCameraMode::Exploration;
}

void UKausCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = Cast<USpringArmComponent>(GetAttachParent());
	if (!SpringArm)
	{
		SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	}

	if (SpringArm)
	{
		SnapToTarget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UKausCameraComponent: Could not find USpringArmComponent on Owner [%s]"), *GetNameSafe(GetOwner()));
	}
}

void UKausCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!FMath::IsNearlyEqual(FieldOfView, CurrentTargetProfile.FieldOfView, 0.1f))
	{
		FieldOfView = FMath::FInterpTo(FieldOfView, CurrentTargetProfile.FieldOfView, DeltaTime, CurrentTargetProfile.InterpSpeed);
	}

	if (SpringArm)
	{
		if (!FMath::IsNearlyEqual(SpringArm->TargetArmLength, CurrentTargetProfile.TargetArmLength, 1.0f))
		{
			SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CurrentTargetProfile.TargetArmLength, DeltaTime, CurrentTargetProfile.InterpSpeed);
		}

		if (!CurrentTargetProfile.bUsePawnControlRotation)
		{
			FRotator CurrentRot = SpringArm->GetRelativeRotation();
			if (!CurrentRot.Equals(CurrentTargetProfile.TargetRotation, 0.1f))
			{
				FRotator NewRot = FMath::RInterpTo(CurrentRot, CurrentTargetProfile.TargetRotation, DeltaTime, CurrentTargetProfile.InterpSpeed);
				SpringArm->SetRelativeRotation(NewRot);
			}
		}
	}
}

void UKausCameraComponent::SetCameraMode(EKausCameraMode NewMode)
{
	if (CurrentMode == NewMode) return;

	if (const FKausCameraProfile* Profile = CameraProfiles.Find(NewMode))
	{
		CurrentMode = NewMode;
		CurrentTargetProfile = *Profile;

		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = CurrentTargetProfile.bUsePawnControlRotation;

			if (!SpringArm->bUsePawnControlRotation)
			{
				//todo
			}
		}

		UE_LOG(LogTemp, Log, TEXT("[KausCamera] Mode Changed to: %d"), (uint8)NewMode);
	}
}

void UKausCameraComponent::SnapToTarget()
{
	FieldOfView = CurrentTargetProfile.FieldOfView;

	if (SpringArm)
	{
		SpringArm->TargetArmLength = CurrentTargetProfile.TargetArmLength;
		SpringArm->bUsePawnControlRotation = CurrentTargetProfile.bUsePawnControlRotation;

		if (!SpringArm->bUsePawnControlRotation)
		{
			SpringArm->SetRelativeRotation(CurrentTargetProfile.TargetRotation);
		}
	}
}