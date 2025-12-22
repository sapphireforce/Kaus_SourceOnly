/*

*/

#pragma once

#include "GameFramework/GameSession.h"
#include "KausSession.generated.h"

/*
* 
*/
UCLASS(Config = Game)
class KAUS_API AKausSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AKausSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	
	
};
