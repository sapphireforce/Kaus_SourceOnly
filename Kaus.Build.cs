using UnrealBuildTool;

public class Kaus : ModuleRules
{
	public Kaus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",

			"InputCore",
			"EnhancedInput",

			"CommonGame",
            "CommonUser", 
            "CommonUI",
            "CommonInput", 
            
            "UMG",
			"Slate",
			"SlateCore",

            "GameplayMessageRuntime",

            "ModularGameplay",
            "ModularGameplayActors",

            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(
            new string[] {
                "Kaus"
            }
        );
    
	}
}
