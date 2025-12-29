#include "KausGameplayTags.h"

namespace KausGameplayTags
{
	//Message - Event
	//UI
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Kaus_Event_UI_Toggle, "Kaus.Event.UI.Toggle", "HUD on/off Event");

	//UI
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_Layer_Game, "UI.Layer.Game", "Primary Layout Game Layer");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_Layer_GameOnMenu, "UI.Layer.GameOnMenu", "Primary Layout GameOnMenu Layer");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_Layer_Menu, "UI.Layer.Menu", "Primary Layout Menu Layer");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_Layer_Modal, "UI.Layer.Modal", "Primary Layout Modal Layer");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_HUD_MainCanvas, "UI.HUD.MainCanvas", "MainCanvas Widget");

	//Phase
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Kaus_Event_Phase_Changed, "Kaus.Event.Phase.Changed", "Phase Change Event");

	//Input - Native
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");

	//Game State
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_None, "Phase.None", "Init State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Loading, "Phase.Loading", "Loading State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Title, "Phase.Title", "Title(Main Menu) State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Exploration, "Phase.Exploration", "Main World State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Battle, "Phase.Battle", "Battle State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Battle_PlayerTurn, "Phase.Battle.PlayerTurn", "Battle PlayerTurn State");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Phase_Battle_EnemyTurn, "Phase.Battle.EnemyTurn", "Battle EnemyTurn State");

	//Gameplay
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Gameplay_MovementStopped, "Gameplay.MovementStopped", "Movement Stopped");

	//Interaction
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_INTERACTION_DURATION_MESSAGE, "Ability.Interaction.Duration.Message", "Interaction System, Duration Mesage");


	//Hardware
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Platform_Trait_Input_HardwareCursor, "Platform.Trait.Input.HardwareCursor", "HardwareCursor");

}
