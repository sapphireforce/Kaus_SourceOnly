#include "KausGameMode.h"
#include "System/KausAssetManager.h"
#include "KausGameState.h"
#include "Player/KausPlayerController.h"
#include "Player/KausReplayPlayerController.h"
#include "Player/KausPlayerState.h"
#include "Character/KausCharacter.h"
#include "UI/KausHUD.h"


AKausGameMode::AKausGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AKausGameState::StaticClass();
	GameSessionClass = AKausGameSession::StaticClass();
	PlayerControllerClass = AKausPlayerController::StaticClass();
	ReplaySpectatorPlayerControllerClass = AKausReplayPlayerController::StaticClass();
	PlayerStateClass = AKausPlayerState::StaticClass();
	DefaultPawnClass = AKausCharacter::StaticClass();
	HUDClass = AKausHUD::StaticClass();
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
