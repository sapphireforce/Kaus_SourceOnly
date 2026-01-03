#include "AbilitySystem/KausUnitAbilityData.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "Logs/KausLogChannels.h"
#include "Attributes/KausAttributeSet.h"

void FKausAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FKausAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FKausAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FKausAbilitySet_GrantedHandles::TakeFromAbilitySystem(UKausAbilitySystemComponent* KausASC)
{
	check(KausASC);

	if (!KausASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			KausASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			KausASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		KausASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UKausUnitAbilityData::UKausUnitAbilityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UKausUnitAbilityData::GiveToAbilitySystem(FGameplayTag UnitID, UKausAbilitySystemComponent* KausASC, FKausAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(KausASC);

	if (!KausASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the attribute sets.
	const int32 AttrSize = GrantedAttributes.Num();
	for (int32 SetIndex = 0; SetIndex < AttrSize; ++SetIndex)
	{
		const FKausAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogKausAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UKausAttributeSet* NewSet = NewObject<UKausAttributeSet>(KausASC->GetOwner(), SetToGrant.AttributeSet);
		NewSet->ApplyDataRowToAttribute(UnitID);
		KausASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}

	// Grant the gameplay abilities.
	const int32 AbilitySize = GrantedGameplayAbilities.Num();
	for (int32 AbilityIndex = 0; AbilityIndex < AbilitySize; ++AbilityIndex)
	{
		const FKausAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogKausAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UKausGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UKausGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = KausASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	const int32 EffectSize = GrantedGameplayEffects.Num();
	for (int32 EffectIndex = 0; EffectIndex < EffectSize; ++EffectIndex)
	{
		const FKausAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogKausAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = KausASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, KausASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
