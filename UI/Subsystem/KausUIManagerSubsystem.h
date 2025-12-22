#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "KausUIManagerSubsystem.generated.h"

struct FKausMessage_UIToggle;

/**
 * 
 */
UCLASS()
class KAUS_API UKausUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:
	UKausUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void HandleUIToggleMessage(FGameplayTag Channel, const FKausMessage_UIToggle& Payload);

	void SetRootLayoutVisibility(bool bVisible);


private:
	FGameplayMessageListenerHandle UIToggleListenerHandle;

};
