// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HTTPModule : ModuleRules
{
	public HTTPModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "Json", "JsonUtilities" });
	}
}
