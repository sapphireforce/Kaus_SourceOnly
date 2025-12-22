/*
*/
#pragma once

#include "Engine/GameEngine.h"
#include "KausGameEngine.generated.h"

/**
 * 
 */
UCLASS()
class KAUS_API UKausGameEngine : public UGameEngine
{
	GENERATED_BODY()
	
public:
	UKausGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Init(IEngineLoop* InEngineLoop) override;
	
};
