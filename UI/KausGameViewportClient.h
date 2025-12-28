#pragma once

#include "CommonGameViewportClient.h"
#include "KausGameViewportClient.generated.h"

class UGameInstance;

UCLASS(BlueprintType)
class KAUS_API UKausGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()
	
public:
	UKausGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
	
};
