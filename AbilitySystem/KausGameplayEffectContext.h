#pragma once

#include "GameplayEffectTypes.h"
#include "KausGameplayEffectContext.generated.h"

class AActor;
class FArchive;
class IKausAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

USTRUCT()
struct FKausGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FKausGameplayEffectContext();

	FKausGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser);

	/** Returns the wrapped FKausGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static KAUS_API FKausGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const IKausAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const IKausAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override;

	virtual UScriptStruct* GetScriptStruct() const override;

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Returns the physical material from the hit result if there is one */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

protected:
	/** Ability Source object (should implement IKausAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FKausGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FKausGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

