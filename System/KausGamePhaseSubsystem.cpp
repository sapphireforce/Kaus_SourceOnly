#include "KausGamePhaseSubsystem.h"
#include "Tags/KausGameplayTags.h"
#include "Message/KausMessageTypes.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Logs/KausLogChannels.h"

void UKausGamePhaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PhaseStack.Reset();
	PhaseStack.Push(KausGameplayTags::TAG_Phase_None);
}

void UKausGamePhaseSubsystem::SetPhase(FGameplayTag NewPhase)
{
	if (!NewPhase.IsValid()) return;

	FGameplayTag OldPhase = GetCurrentPhase();

	PhaseStack.Reset();
	PhaseStack.Push(NewPhase);

	BroadcastPhaseChange(OldPhase, NewPhase);
}

void UKausGamePhaseSubsystem::PushPhase(FGameplayTag NewPhase)
{
	if (!NewPhase.IsValid()) return;

	FGameplayTag OldPhase = GetCurrentPhase();

	if (OldPhase == NewPhase)
	{
		UE_LOG(LogKaus, Warning, TEXT("Attempted to push same phase [%s]. Ignoring."), *NewPhase.ToString());
		return;
	}

	PhaseStack.Push(NewPhase);

	BroadcastPhaseChange(OldPhase, NewPhase);
}

void UKausGamePhaseSubsystem::PopPhase()
{
	if (PhaseStack.Num() <= 1)
	{
		UE_LOG(LogKaus, Warning, TEXT("Cannot PopPhase. Stack is empty or at base level."));
		return;
	}

	FGameplayTag OldPhase = PhaseStack.Pop();
	FGameplayTag NewPhase = GetCurrentPhase();

	BroadcastPhaseChange(OldPhase, NewPhase);
}

FGameplayTag UKausGamePhaseSubsystem::GetCurrentPhase() const
{
	return (PhaseStack.Num() > 0) ? PhaseStack.Last() : KausGameplayTags::TAG_Phase_None;
}

bool UKausGamePhaseSubsystem::IsPhase(FGameplayTag PhaseTag) const
{
	if (!PhaseTag.IsValid()) return false;

	return GetCurrentPhase().MatchesTag(PhaseTag);
}

void UKausGamePhaseSubsystem::BroadcastPhaseChange(FGameplayTag PreviousPhase, FGameplayTag NewPhase)
{
	if (PreviousPhase == NewPhase) return;

	UE_LOG(LogKaus, Log, TEXT("[Phase] Changed: [%s] -> [%s] (StackDepth: %d)"),
		*PreviousPhase.ToString(),
		*NewPhase.ToString(),
		PhaseStack.Num());

	FKausMessage_PhaseChanged Message;
	Message.PreviousPhase = PreviousPhase;
	Message.NewPhase = NewPhase;

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetGameInstance());
	MessageSubsystem.BroadcastMessage(KausGameplayTags::TAG_Kaus_Event_Phase_Changed, Message);
}

