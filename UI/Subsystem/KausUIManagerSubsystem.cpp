#include "KausUIManagerSubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "CommonLocalPlayer.h"
#include "Message/KausMessageTypes.h"
#include "Tags/KausGameplayTags.h"

#include "UI/KausUILayerConfig.h"
#include "UI/KausPrimaryGameLayout.h"

UKausUIManagerSubsystem::UKausUIManagerSubsystem()
{
}

void UKausUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetGameInstance());
	UIToggleListenerHandle = MessageSubsystem.RegisterListener(
		KausGameplayTags::TAG_Kaus_Event_UI_Toggle,
		this,
		&UKausUIManagerSubsystem::HandleUIToggleMessage
	);
}

void UKausUIManagerSubsystem::Deinitialize()
{
	if (UIToggleListenerHandle.IsValid())
	{
		UIToggleListenerHandle.Unregister();
	}

	Super::Deinitialize();
}

void UKausUIManagerSubsystem::ApplyLayerConfig(const UKausUILayerConfig* LayerConfig, const UCommonLocalPlayer* LocalPlayer)
{
	if (!LayerConfig || !LocalPlayer) return;

	UGameUIManagerSubsystem* GameUIManager = GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>();
	if (!GameUIManager) return;

	UGameUIPolicy* Policy = Cast<UGameUIPolicy>(GameUIManager->GetCurrentUIPolicy());
	if (!Policy)
	{
		UE_LOG(LogTemp, Error, TEXT("UI Policy is NULL! Check DefaultGame.ini"));
		return;
	}

	UKausPrimaryGameLayout* RootLayout = Cast<UKausPrimaryGameLayout>(Policy->GetRootLayout(LocalPlayer));
	if (!RootLayout)
	{
		UE_LOG(LogTemp, Warning, TEXT("RootLayout not found for Player [%s]"), *LocalPlayer->GetName());
		return;
	}

	for (const FKausLayerEntry& Entry : LayerConfig->LayerEntries)
	{
		if (Entry.LayerTag.IsValid() && Entry.WidgetClass)
		{
			RootLayout->PushWidgetToLayerStack(Entry.LayerTag, Entry.WidgetClass);
		}
	}
}

void UKausUIManagerSubsystem::HandleUIToggleMessage(FGameplayTag Channel, const FKausMessage_UIToggle& Payload)
{
	SetRootLayoutVisibility(Payload.bShow);
}

void UKausUIManagerSubsystem::SetRootLayoutVisibility(bool bVisible)
{
	if (const UGameUIPolicy* Policy = GetCurrentUIPolicy())
	{
		for (const ULocalPlayer* LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			const UCommonLocalPlayer* CommonPlayer = Cast<UCommonLocalPlayer>(LocalPlayer);
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CommonPlayer))
			{
				ESlateVisibility NewVisibility = bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
				RootLayout->SetVisibility(NewVisibility);

				UE_LOG(LogTemp, Log, TEXT("[QA] Set UI Visibility: %d"), bVisible);
			}
		}
	}
}