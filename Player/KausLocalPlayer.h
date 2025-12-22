#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "KausLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API UKausLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
	
public:
	UKausLocalPlayer();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface
	
	
};
