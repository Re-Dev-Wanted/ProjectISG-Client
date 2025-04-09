// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectISG : ModuleRules
{
	public ProjectISG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Common", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks",
			"OnlineSubsystem", "OnlineSubsystemNull",
			"OnlineSubsystemSteam", "OnlineSubsystemUtils"
		});
	}
}
