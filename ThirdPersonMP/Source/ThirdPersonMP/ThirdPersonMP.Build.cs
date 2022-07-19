// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThirdPersonMP : ModuleRules
{
	public ThirdPersonMP(ReadOnlyTargetRules Target) : base(Target)
	{
		//PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemUtils" });

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// PublicDefinitions.Add("ThirdPersonMP_PACKAGE=1");
		// PrivateIncludePaths.AddRange(
		// 	new string[] {
		// 	}
		// 	);
		//
		// PrivateDependencyModuleNames.AddRange(new string[]
		// {
		// 	"Core", 
		// 	"CoreUObject", 
		// 	"Engine", 
		// 	"InputCore", 
		// 	"HeadMountedDisplay",
		// 	"UMG",
		// 	"OnlineSubsystem",
		// 	"OnlineSubsystemUtils"
		// });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG" , "OnlineSubsystem", "OnlineSubsystemUtils" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate","SlateCore" });
	}
}
