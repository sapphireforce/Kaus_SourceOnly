#include "KausAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UKausAbilitySystemComponent::UKausAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Replication ¼³Á¤
	SetIsReplicatedByDefault(true);
	SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void UKausAbilitySystemComponent::ClearAbilityInput()
{
	//todo: Spec Handler Clear
}

void UKausAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		ProcessAbilityInput(InputTag, true);
	}
}

void UKausAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		ProcessAbilityInput(InputTag, false);
	}
}

void UKausAbilitySystemComponent::ProcessAbilityInput(const FGameplayTag& InputTag, bool bPressed)
{
	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
		{
			if (bPressed)
			{
				AbilitySpecInputPressed(AbilitySpec);
			}
			else
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}
