#pragma once

#include "AbilitySystemComponent.h"
#include "Tags/KausGameplayTags.h"
#include "KausAbilitySystemComponent.generated.h"

/**
 * UKausAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS()
class KAUS_API UKausAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UKausAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void ClearAbilityInput();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(const FGameplayTag& InputTag, bool bPressed);

};
