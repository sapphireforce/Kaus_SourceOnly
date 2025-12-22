// Source/Kaus/Camera/KausCameraComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "KausCameraComponent.generated.h"

class USpringArmComponent;

UENUM(BlueprintType)
enum class EKausCameraMode : uint8
{
	Exploration,
	Battle,      
	Focus,       
	None
};

USTRUCT(BlueprintType)
struct FKausCameraProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float TargetArmLength = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	FRotator TargetRotation = FRotator(-20.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float FieldOfView = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	bool bUsePawnControlRotation = true;
};

/*
* todo: 생성자에서 Profile 생성을 제거, BP로 CameraProfile을 세팅하고 Mapping 후 사용
* 필요에 따라 ECameraMode도 추가 가능하도록 만들어 두는 것도 좋다.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KAUS_API UKausCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UKausCameraComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Kaus|Camera")
	void SetCameraMode(EKausCameraMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "Kaus|Camera")
	void SnapToTarget();

	UFUNCTION(BlueprintPure, Category = "Kaus|Camera")
	EKausCameraMode GetCurrentCameraMode() const { return CurrentMode; }

	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kaus|Config")
	TMap<EKausCameraMode, FKausCameraProfile> CameraProfiles;

	FKausCameraProfile CurrentTargetProfile;

private:
	UPROPERTY(Transient)
	TObjectPtr<USpringArmComponent> SpringArm;

	EKausCameraMode CurrentMode = EKausCameraMode::None;
};