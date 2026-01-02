#include "KausAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Logs/KausLogChannels.h"
#include "KausAbilityTagRelationshipMapping.h"

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


bool UKausAbilitySystemComponent::IsReady() const
{
	return AbilityActorInfo.IsValid();
}

bool UKausAbilitySystemComponent::IsActivationGroupBlocked(EKausAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EKausAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EKausAbilityActivationGroup::Exclusive_Replaceable:
	case EKausAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		bBlocked = (ActivationGroupCounts[(uint8)EKausAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void UKausAbilitySystemComponent::AddAbilityToActivationGroup(EKausAbilityActivationGroup Group, UKausGameplayAbility* KausAbility)
{
	check(KausAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;

	switch (Group)
	{
	case EKausAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.
		break;

	case EKausAbilityActivationGroup::Exclusive_Replaceable:
	case EKausAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(EKausAbilityActivationGroup::Exclusive_Replaceable, KausAbility, bReplicateCancelAbility);
		break;

	default:
		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)EKausAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)EKausAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		UE_LOG(LogKausAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
	}
}

void UKausAbilitySystemComponent::RemoveAbilityFromActivationGroup(EKausAbilityActivationGroup Group, UKausGameplayAbility* KausAbility)
{
	check(KausAbility);
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void UKausAbilitySystemComponent::CancelActivationGroupAbilities(EKausAbilityActivationGroup Group, UKausGameplayAbility* IgnoreKausAbility, bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this, Group, IgnoreKausAbility](const UKausGameplayAbility* KausAbility, FGameplayAbilitySpecHandle Handle)
		{
			return ((KausAbility->GetActivationGroup() == Group) && (KausAbility != IgnoreKausAbility));
		};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UKausAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		UKausGameplayAbility* KausAbilityCDO = Cast<UKausGameplayAbility>(AbilitySpec.Ability);
		if (!KausAbilityCDO)
		{
			UE_LOG(LogKausAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Non-KausGameplayAbility %s was Granted to ASC. Skipping."), *AbilitySpec.Ability.GetName());
			continue;
		}

		PRAGMA_DISABLE_DEPRECATION_WARNINGS
			ensureMsgf(AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("CancelAbilitiesByFunc: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Cancel all the spawned instances.
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		for (UGameplayAbility* AbilityInstance : Instances)
		{
			UKausGameplayAbility* KausAbilityInstance = CastChecked<UKausGameplayAbility>(AbilityInstance);

			if (ShouldCancelFunc(KausAbilityInstance, AbilitySpec.Handle))
			{
				if (KausAbilityInstance->CanBeCanceled())
				{
					KausAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), KausAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
				}
				else
				{
					UE_LOG(LogKausAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *KausAbilityInstance->GetName());
				}
			}
		}
	}
}

void UKausAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

void UKausAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}
