#include "KausInputConfig.h"

UKausInputConfig::UKausInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

void UKausInputConfig::PostLoad()
{
	Super::PostLoad();

	MappingInputMaps();
}

const UInputAction* UKausInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const UInputAction* const* FoundAction = NativeInputTagMap.Find(InputTag))
	{
		return *FoundAction;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UKausInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (const UInputAction* const* FoundAction = AbilityInputTagMap.Find(InputTag))
	{
		return *FoundAction;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

void UKausInputConfig::MappingInputMaps()
{
	NativeInputTagMap.Reset();
	for (const FKausInputAction& Action : NativeInputActions)
	{
		if (Action.InputTag.IsValid() && Action.InputAction)
		{
			if (NativeInputTagMap.Contains(Action.InputTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("Duplicated InputTag [%s] in NativeInputActions of [%s]. Overwriting."), *Action.InputTag.ToString(), *GetName());
			}
			NativeInputTagMap.Add(Action.InputTag, Action.InputAction);
		}
	}

	AbilityInputTagMap.Reset();
	for (const FKausInputAction& Action : AbilityInputActions)
	{
		if (Action.InputTag.IsValid() && Action.InputAction)
		{
			if (AbilityInputTagMap.Contains(Action.InputTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("Duplicated InputTag [%s] in AbilityInputActions of [%s]. Overwriting."), *Action.InputTag.ToString(), *GetName());
			}
			AbilityInputTagMap.Add(Action.InputTag, Action.InputAction);
		}
	}
}
