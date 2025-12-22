#include "KausGameSession.h"

AKausGameSession::AKausGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AKausGameSession::ProcessAutoLogin()
{
	return false;
}

void AKausGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AKausGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
