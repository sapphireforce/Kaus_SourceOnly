#include "KausPlayerController.h"

AKausPlayerController::AKausPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AKausPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AKausPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void AKausPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
