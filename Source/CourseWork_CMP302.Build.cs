// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CourseWork_CMP302 : ModuleRules
{
	public CourseWork_CMP302(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
