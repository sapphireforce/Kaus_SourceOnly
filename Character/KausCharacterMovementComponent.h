#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KausCharacterMovementComponent.generated.h"

/**
 * FKausCharacterGroundInfo
 *
 *	Information about the ground under the character.  It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FKausCharacterGroundInfo
{
	GENERATED_BODY()

	FKausCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{
	}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * UKausCharacterMovementComponent
 *
 *	The base character movement component class used by this project.
 */
UCLASS(ClassGroup = (Custom), Blueprintable)
class KAUS_API UKausCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UKausCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual bool CanAttemptJump() const override;

	//~UMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface

	// Returns the current ground info.  Calling this will update the ground info if it's out of date.
	UFUNCTION(BlueprintCallable, Category = "Kaus|CharacterMovement")
	const FKausCharacterGroundInfo& GetGroundInfo();

	void SetReplicatedAcceleration(const FVector& InAcceleration);

protected:
	virtual void InitializeComponent() override;

protected:
	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FKausCharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};
