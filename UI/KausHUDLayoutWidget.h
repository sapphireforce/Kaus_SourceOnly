#pragma once

#include "UI/KausActivatableWidget.h"
#include "KausHUDLayoutWidget.generated.h"

struct FGameplayTag;
struct FUIExtensionHandle;
class UKausLayoutEntryData;

/**
 * UKausHUDLayoutWidget
 *
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Kaus HUD Layout", Category = "Kaus|HUD"))
class KAUS_API UKausHUDLayoutWidget : public UKausActivatableWidget
{
	GENERATED_BODY()

public:
	UKausHUDLayoutWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	
	virtual void NativeDestruct() override;

protected:
	void RegisterLayoutExtensions();

	void UnregisterLayoutExtensions();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kaus|UI")
	TObjectPtr<const UKausLayoutEntryData> HUDData;

private:
	UPROPERTY(Transient)
	TArray<FUIExtensionHandle> ExtensionHandles;
};