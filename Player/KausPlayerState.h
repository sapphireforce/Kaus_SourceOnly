#pragma once

#include "GameFramework/PlayerState.h"
#include "KausPlayerState.generated.h"

/**
 * AKausPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class KAUS_API AKausPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AKausPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

};
