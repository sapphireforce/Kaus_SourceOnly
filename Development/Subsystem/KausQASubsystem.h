#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KausQASubsystem.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API UKausQASubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UKausQASubsystem();

	UFUNCTION(Exec)
	void Kaus_ShowUI(bool bShow);
	
	
};
