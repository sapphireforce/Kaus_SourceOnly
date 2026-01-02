#include "KausGameplayEffectContext.h"

#include "AbilitySystem/KausAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

class FArchive;

FKausGameplayEffectContext::FKausGameplayEffectContext()
	: FGameplayEffectContext()
{
}

FKausGameplayEffectContext::FKausGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
	: FGameplayEffectContext(InInstigator, InEffectCauser)
{
}

FKausGameplayEffectContext* FKausGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FKausGameplayEffectContext::StaticStruct()))
	{
		return (FKausGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FKausGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	return true;
}

void FKausGameplayEffectContext::SetAbilitySource(const IKausAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}

const IKausAbilitySourceInterface* FKausGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IKausAbilitySourceInterface>(AbilitySourceObject.Get());
}

FGameplayEffectContext* FKausGameplayEffectContext::Duplicate() const
{
	FKausGameplayEffectContext* NewContext = new FKausGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

UScriptStruct* FKausGameplayEffectContext::GetScriptStruct() const
{
	return FKausGameplayEffectContext::StaticStruct();
}

const UPhysicalMaterial* FKausGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

