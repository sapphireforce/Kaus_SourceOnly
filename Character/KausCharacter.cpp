#include "KausCharacter.h"

#include "KausCharacterMovementComponent.h"
#include "KausPlayerUnitComponent.h"
#include "KausUnitStatusComponent.h"
#include "Camera/KausCameraComponent.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/KausPlayerState.h"
#include "Character/KausUnitExtensionComponent.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Tags/KausGameplayTags.h" 
#include "Message/KausMessageTypes.h"

static FName NAME_KausCharacterCollisionProfile_Capsule(TEXT("KausPawnCapsule"));
static FName NAME_KausCharacterCollisionProfile_Mesh(TEXT("KausPawnMesh"));

AKausCharacter::AKausCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKausCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//todo: ÀÌ°Ô ¹¹Áö
	SetNetCullDistanceSquared(900000000.0f);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_KausCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_KausCharacterCollisionProfile_Mesh);

	UKausCharacterMovementComponent* KausMoveComp = CastChecked<UKausCharacterMovementComponent>(GetCharacterMovement());
	KausMoveComp->GravityScale = 1.0f;
	KausMoveComp->MaxAcceleration = 2400.0f;
	KausMoveComp->BrakingFrictionFactor = 1.0f;
	KausMoveComp->BrakingFriction = 6.0f;
	KausMoveComp->GroundFriction = 8.0f;
	KausMoveComp->BrakingDecelerationWalking = 1400.0f;
	KausMoveComp->bUseControllerDesiredRotation = false;
	KausMoveComp->bOrientRotationToMovement = false;
	KausMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	KausMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	KausMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	KausMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	KausMoveComp->SetCrouchedHalfHeight(65.0f);


	//todo:  Init and register Ability System(Store in PlayerState)
	UnitExtComponent = CreateDefaultSubobject<UKausUnitExtensionComponent>(TEXT("PawnExtensionComponent"));
	//UnitExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	//UnitExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	PlayerUnitComponent = CreateDefaultSubobject<UKausPlayerUnitComponent>(TEXT("PlayerUnitComponent"));
	UnitStatusComponent = CreateDefaultSubobject<UKausUnitStatusComponent>(TEXT("UnitStatusComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UKausCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

AKausPlayerController* AKausCharacter::GetKausPlayerController() const
{
	return CastChecked<AKausPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AKausPlayerState* AKausCharacter::GetKausPlayerState() const
{
	return CastChecked<AKausPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UKausAbilitySystemComponent* AKausCharacter::GetKausAbilitySystemComponent() const
{
	return Cast<UKausAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AKausCharacter::GetAbilitySystemComponent() const
{
	if (UnitExtComponent == nullptr)
	{
		return nullptr;
	}

	return UnitExtComponent->GetKausAbilitySystemComponent();
}

void AKausCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent())
	{
		KausASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool AKausCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent())
	{
		return KausASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool AKausCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent())
	{
		return KausASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool AKausCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent())
	{
		return KausASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void AKausCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AKausCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AKausCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AKausCharacter::Reset()
{
	//DisableMovementAndCollision();

	K2_OnReset();

	//UninitAndDestroy();
}

void AKausCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AKausCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void AKausCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
}

void AKausCharacter::OnAbilitySystemInitialized()
{
	UKausAbilitySystemComponent* KausASC = GetKausAbilitySystemComponent();
	check(KausASC);

	UnitStatusComponent->InitializeWithAbilitySystem(KausASC);

	//InitializeGameplayTags();
}

void AKausCharacter::OnAbilitySystemUninitialized()
{
	UnitStatusComponent->UninitializeFromAbilitySystem();
}

void AKausCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AKausCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AKausCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerUnitComponent)
	{
		PlayerUnitComponent->InitializePlayerInput(PlayerInputComponent);
	}

}

void AKausCharacter::HandlePhaseChangedMessage(FGameplayTag Channel, const FKausMessage_PhaseChanged& Payload)
{
	if (CameraComponent)
	{
		if (Payload.NewPhase.MatchesTag(KausGameplayTags::TAG_Phase_Battle))
		{
			CameraComponent->SetCameraMode(EKausCameraMode::Battle);
		}
		else if (Payload.NewPhase.MatchesTag(KausGameplayTags::TAG_Phase_Exploration))
		{
			CameraComponent->SetCameraMode(EKausCameraMode::Exploration);
		}
	}
}

void AKausCharacter::OnHealthChanged(UKausUnitStatusComponent* StatusComp, float OldValue, float NewValue, AActor* InstigatorActor)
{
	if (NewValue <= 0.0f && OldValue > 0.0f)
	{
		//OnDeathStarted(InstigatorActor);
	}
}

void AKausCharacter::OnRep_ReplicatedAcceleration()
{
	if (UKausCharacterMovementComponent* KausMovementComponent = Cast<UKausCharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel = KausMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		KausMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}
