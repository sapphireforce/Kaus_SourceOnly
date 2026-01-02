#pragma once

#include "AbilitySystemComponent.h"
#include "Tags/KausGameplayTags.h"
#include "AbilitySystem/Abilities/KausGameplayAbility.h"
#include "KausAbilitySystemComponent.generated.h"

class UKausAbilityTagRelationshipMapping;

/**
 * UKausAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS(ClassGroup = (Custom), Blueprintable)
class KAUS_API UKausAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UKausAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void ClearAbilityInput();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(const FGameplayTag& InputTag, bool bPressed);

	bool IsReady() const;

	bool IsActivationGroupBlocked(EKausAbilityActivationGroup Group) const;
	void AddAbilityToActivationGroup(EKausAbilityActivationGroup Group, UKausGameplayAbility* KausAbility);
	void RemoveAbilityFromActivationGroup(EKausAbilityActivationGroup Group, UKausGameplayAbility* KausAbility);
	void CancelActivationGroupAbilities(EKausAbilityActivationGroup Group, UKausGameplayAbility* IgnoreKausAbility, bool bReplicateCancelAbility);

	typedef TFunctionRef<bool(const UKausGameplayAbility* KausAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	/** Gets the ability target data associated with the given ability handle and activation info */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

	/** Looks at ability tags and gathers additional required and blocking tags */
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

protected:
	int32 ActivationGroupCounts[(uint8)EKausAbilityActivationGroup::MAX];

	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UKausAbilityTagRelationshipMapping> TagRelationshipMapping;

};
