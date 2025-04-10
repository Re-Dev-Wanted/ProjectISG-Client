// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectISGEditorTarget : TargetRules
{
	public ProjectISGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("ProjectISG");
	}
}