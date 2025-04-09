using UnrealBuildTool;

public class Common : ModuleRules
{
    public Common(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject", "Engine", "Slate", "SlateCore",
                "OnlineSubsystem", "OnlineSubsystemNull",
                "OnlineSubsystemSteam", "OnlineSubsystemUtils"
            }
        );
    }
}