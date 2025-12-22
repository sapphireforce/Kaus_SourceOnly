#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KausGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API AKausGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AKausGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase Interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	//~End of AGameModeBase Interface
	
	
};
