// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectISG : ModuleRules
{
	public ProjectISG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"NetCore"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			"OnlineSubsystem", "OnlineSubsystemNull",
			"OnlineSubsystemSteam", "OnlineSubsystemUtils",
			"Json", "JsonUtilities", "HTTP", "UMG", "Slate",
			"SlateCore", "ProceduralMeshComponent", "MovieScene",
			"LevelSequence", "MovieSceneTracks", "CableComponent",
			"ImageWrapper"
		});
	}
}