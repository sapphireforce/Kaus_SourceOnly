#include "KausGameMode.h"
#include "System/KausAssetManager.h"

AKausGameMode::AKausGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AKausGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UKausAssetManager::Get().GetGameData();

	UE_LOG(LogTemp, Log, TEXT("KausGameMode: InitGame Complete. Global Data Loaded. Map: %s"), *MapName);
}

void AKausGameMode::StartPlay()
{
	Super::StartPlay();
}

UClass* AKausGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
