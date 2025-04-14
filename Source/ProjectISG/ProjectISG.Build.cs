// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectISG : ModuleRules
{
	public ProjectISG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"UMG", "GameplayAbilities", "GameplayTags",
			"GameplayTasks", "OnlineSubsystem", "OnlineSubsystemNull",
			"OnlineSubsystemSteam", "OnlineSubsystemUtils",
			"ProceduralMeshComponent"
		});
	}
}