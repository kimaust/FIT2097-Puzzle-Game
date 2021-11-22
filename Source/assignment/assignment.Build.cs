// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class assignment : ModuleRules
{
	public assignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "GameplayTasks", "Niagara", "LevelSequence", "MovieScene" });
		PublicIncludePaths.AddRange(new string[] {
			"assignment/",
			Path.Combine(EngineDirectory, "Plugins/FX/Niagara/Source/Niagara/Public"),
			Path.Combine(EngineDirectory, "Plugins/FX/Niagara/Source/Niagara/Classes"),
			Path.Combine(EngineDirectory, "Plugins/FX/Niagara/Source/Niagara/Private")
		});
	}
}
