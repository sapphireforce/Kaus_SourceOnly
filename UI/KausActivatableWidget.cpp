#include "KausActivatableWidget.h"
#include "Editor/WidgetCompilerLog.h"

#define LOCTEXT_NAMESPACE "Kaus"

UKausActivatableWidget::UKausActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UKausActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EKausWidgetInputMode::GameOnMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EKausWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EKausWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EKausWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

#if WITH_EDITOR
void UKausActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (!GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(UKausActivatableWidget, BP_GetDesiredFocusTarget)))
	{
		if (GetParentNativeClass(GetClass()) == UKausActivatableWidget::StaticClass())
		{
			CompileLog.Warning(LOCTEXT("ValidateGetDesiredFocusTarget_Warning", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen."));
		}
		else
		{
			//TODO - Note for now, because we can't guarantee it isn't implemented in a native subclass of this one.
			CompileLog.Note(LOCTEXT("ValidateGetDesiredFocusTarget_Note", "GetDesiredFocusTarget wasn't implemented, you're going to have trouble using gamepads on this screen.  If it was implemented in the native base class you can ignore this message."));
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE