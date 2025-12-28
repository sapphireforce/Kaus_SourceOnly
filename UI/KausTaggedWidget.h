#pragma once

#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "KausTaggedWidget.generated.h"

struct FGameplayTag;

/**
 * An widget in a layout that has been tagged and responsive 
 * 
 */
UCLASS(Abstract, Blueprintable)
class KAUS_API UKausTaggedWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Kaus|UI")
	void OnTagStateChanged(bool bHasTag, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Kaus|UI")
	bool HasTag(FGameplayTag TagToCheck) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	FGameplayTagContainer WidgetTags;

	
};
