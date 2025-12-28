#include "KausGameViewportClient.h"
#include "CommonUISettings.h"
#include "ICommonUIModule.h"
#include "Tags/KausGameplayTags.h"

UKausGameViewportClient::UKausGameViewportClient()
	: Super(FObjectInitializer::Get())
{
}

void UKausGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

	const bool UseHardwareCursor = ICommonUIModule::GetSettings().GetPlatformTraits().HasTag(KausGameplayTags::TAG_Platform_Trait_Input_HardwareCursor);
	SetUseSoftwareCursorWidgets(!UseHardwareCursor);
}
