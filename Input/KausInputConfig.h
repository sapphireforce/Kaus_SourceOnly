/*

*/
#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "KausInputConfig.generated.h"

class UInputAction;

/**
 * FKausInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FKausInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class KAUS_API UKausInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UKausInputConfig(const FObjectInitializer& ObjectInitializer);

	//~UObject interface
	virtual void PostLoad() override;
	//~End of UObject interface
	
	UFUNCTION(BlueprintCallable, Category = "Kaus|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Kaus|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FKausInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FKausInputAction> AbilityInputActions;

private:
	void MappingInputMaps();

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, const UInputAction*> NativeInputTagMap;

	UPROPERTY(Transient)
	TMap<FGameplayTag, const UInputAction*> AbilityInputTagMap;

};
