#pragma once

#include "CommonActivatableWidget.h"
#include "KausActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class EKausWidgetInputMode : uint8
{
	Default,
	GameOnMenu,
	Game,
	Menu
};

// An activatable widget that automatically drives the desired input config when activated
UCLASS(Abstract, Blueprintable)
class KAUS_API UKausActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UKausActivatableWidget(const FObjectInitializer& ObjectInitializer);

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	/** The desired input mode to use while this UI is activated */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EKausWidgetInputMode InputConfig = EKausWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
};
