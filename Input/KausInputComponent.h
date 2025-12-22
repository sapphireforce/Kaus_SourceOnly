#pragma once

#include "EnhancedInputComponent.h"
#include "KausInputConfig.h"
#include "KausInputComponent.generated.h"

/**
 * UKausInputComponent
 * 
 * Component used to manage input mappings and bindings using an input config data asset.
 */
UCLASS(Config = Input)
class KAUS_API UKausInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UKausInputComponent(const FObjectInitializer& ObjectInitializer);
	
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UKausInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UKausInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

};

template<class UserClass, typename FuncType>
inline void UKausInputComponent::BindNativeAction(const UKausInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void UKausInputComponent::BindAbilityActions(const UKausInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FKausInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
