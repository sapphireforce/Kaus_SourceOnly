#include "KausHUD.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "UI/Subsystem/KausUIManagerSubsystem.h"

AKausHUD::AKausHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AKausHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AKausHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UKausUIManagerSubsystem* UIManager = GI->GetSubsystem<UKausUIManagerSubsystem>())
		{
			if (DefaultLayerConfig)
			{
				const APlayerController* PC = GetOwningPlayerController();
				if (PC)
				{
					if (const UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(PC->GetLocalPlayer()))
					{
						UIManager->ApplyLayerConfig(DefaultLayerConfig, LocalPlayer);
					}
				}
			}
		}
	}
}

void AKausHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AKausHUD::GetDebugActorList(TArray<AActor*>& InOutList)
{
	UWorld* World = GetWorld();

	Super::GetDebugActorList(InOutList);

	// Add all actors with an ability system component.
	for (TObjectIterator<UAbilitySystemComponent> It; It; ++It)
	{
		if (UAbilitySystemComponent* ASC = *It)
		{
			if (!ASC->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
			{
				AActor* AvatarActor = ASC->GetAvatarActor();
				AActor* OwnerActor = ASC->GetOwnerActor();

				if (AvatarActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor))
				{
					AddActorToDebugList(AvatarActor, InOutList, World);
				}
				else if (OwnerActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
				{
					AddActorToDebugList(OwnerActor, InOutList, World);
				}
			}
		}
	}
}
