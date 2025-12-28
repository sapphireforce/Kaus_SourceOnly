#include "KausHUDLayoutWidget.h"
#include "GameplayTagContainer.h"
#include "UKausLayoutEntryData.h"
#include "CommonUIExtensions.h"
#include "UIExtensionSystem.h"
#include "CommonLocalPlayer.h"

UKausHUDLayoutWidget::UKausHUDLayoutWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UKausHUDLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterLayoutExtensions();
}

void UKausHUDLayoutWidget::NativeDestruct()
{
	UnregisterLayoutExtensions();

	Super::NativeDestruct();
}

void UKausHUDLayoutWidget::RegisterLayoutExtensions()
{
	if (!HUDData)
	{
		return;
	}

	UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
	if (!ExtensionSubsystem)
	{
		return;
	}

	UObject* ContextObject = GetOwningLocalPlayer<UCommonLocalPlayer>();
	if (!ContextObject)
	{
		ContextObject = GetOwningPlayer(); // Fallback
	}

	ExtensionHandles.Reset();

	for (const FKausHUDElementEntry& Entry : HUDData->Widgets)
	{
		if (Entry.WidgetClass && Entry.SlotTag.IsValid())
		{
			FUIExtensionHandle Handle = ExtensionSubsystem->RegisterExtensionAsWidgetForContext(
				Entry.SlotTag,
				ContextObject,
				Entry.WidgetClass,
				-1
			);

			ExtensionHandles.Add(Handle);
		}
	}
}

void UKausHUDLayoutWidget::UnregisterLayoutExtensions()
{
	for (FUIExtensionHandle& Handle : ExtensionHandles)
	{
		Handle.Unregister();
	}
	ExtensionHandles.Reset();
}