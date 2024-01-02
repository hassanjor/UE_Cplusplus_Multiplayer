// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game206_A3 : ModuleRules
{
	public Game206_A3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
