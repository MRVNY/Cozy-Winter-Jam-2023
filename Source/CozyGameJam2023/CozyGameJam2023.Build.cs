// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CozyGameJam2023 : ModuleRules
{
	public CozyGameJam2023(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
