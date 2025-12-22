#pragma once

#include "GameFramework/GameSession.h"
#include "KausGameSession.generated.h"

/*
* 
*/
UCLASS(Config = Game)
class KAUS_API AKausGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AKausGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	
	
};
