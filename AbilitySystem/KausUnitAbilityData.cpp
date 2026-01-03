#include "AbilitySystem/KausUnitAbilityData.h"
#include "AbilitySystem/KausAbilitySystemComponent.h"
#include "Logs/KausLogChannels.h"
#include "Attributes/KausAttributeSet.h"
#include "AbilitySystem/KausUnitInitializationContext.h"

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

void UKausUnitAbilityData::GiveToAbilitySystem(UKausAbilitySystemComponent* KausASC, FKausAbilitySet_GrantedHandles* OutGrantedHandles, const FKausUnitInitializationContext& InitContext) const {
	check(KausASC);

    check(KausASC);

    if (!KausASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    // 1. Attribute Sets 부여 및 초기화
    for (const FKausAbilitySet_AttributeSet& SetToGrant : GrantedAttributes)
    {
        if (!IsValid(SetToGrant.AttributeSet)) continue;

        // A. AttributeSet 객체 생성
        UAttributeSet* NewSet = NewObject<UAttributeSet>(KausASC->GetOwner(), SetToGrant.AttributeSet);
        KausASC->AddAttributeSetSubobject(NewSet);

        // B. 초기화 위임 (Dependency Injection)
        // AttributeSet이 인터페이스를 구현하고 있다면, 데이터 에셋에 설정된 테이블을 넘겨줍니다.
        if (IKausAttributeRowInterface* InterfaceSet = Cast<IKausAttributeRowInterface>(NewSet))
        {
            if (!SetToGrant.InitializationData.IsNull())
            {
                // 여기서 테이블을 로드하여 주입합니다.
                const UDataTable* InitTable = SetToGrant.InitializationData.LoadSynchronous();
                InterfaceSet->InitAttributeData(InitTable, InitContext);
            }
        }

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAttributeSet(NewSet);
        }
    }

    // 2. Gameplay Abilities 부여 (Context의 Level 활용)
    for (const FKausAbilitySet_GameplayAbility& AbilityToGrant : GrantedGameplayAbilities)
    {
        if (!IsValid(AbilityToGrant.Ability)) continue;

        UKausGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UKausGameplayAbility>();

        // 데이터 에셋의 레벨이 0보다 크면 고정값, 아니면 유닛 레벨을 따름
        float AbilityLevel = (AbilityToGrant.AbilityLevel > 0) ? (float)AbilityToGrant.AbilityLevel : InitContext.Level;

        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityLevel);
        AbilitySpec.SourceObject = InitContext.AvatarActor;
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle Handle = KausASC->GiveAbility(AbilitySpec);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(Handle);
        }
    }

    // 3. Gameplay Effects 부여 (Context의 Level 활용)
    for (const FKausAbilitySet_GameplayEffect& EffectToGrant : GrantedGameplayEffects)
    {
        if (!IsValid(EffectToGrant.GameplayEffect)) continue;

        const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();

        float EffectLevel = (EffectToGrant.EffectLevel > 0.0f) ? EffectToGrant.EffectLevel : InitContext.Level;

        const FActiveGameplayEffectHandle Handle = KausASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectLevel, KausASC->MakeEffectContext());

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddGameplayEffectHandle(Handle);
        }
    }
}
