#include "KausQASubsystem.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Message/KausMessageTypes.h"  
#include "Engine/GameInstance.h"
#include "Tags/KausGameplayTags.h"

UKausQASubsystem::UKausQASubsystem()
{
}

void UKausQASubsystem::Kaus_ShowUI(bool bShow)
{
	FKausMessage_UIToggle Message;
	Message.bShow = bShow;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetGameInstance());

	MessageSubsystem.BroadcastMessage(KausGameplayTags::TAG_Kaus_Event_UI_Toggle, Message);

	UE_LOG(LogTemp, Log, TEXT("[QA] Broadcast UI Toggle Message: %d"), bShow);
}


