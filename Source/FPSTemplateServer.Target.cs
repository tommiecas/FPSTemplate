// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSTemplateServerTarget : TargetRules
{
    public FPSTemplateServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("FPSTemplate");
    }
}
