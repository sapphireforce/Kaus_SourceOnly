#include "KausSession.h"

AKausSession::AKausSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AKausSession::ProcessAutoLogin()
{
	return false;
}

void AKausSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AKausSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
