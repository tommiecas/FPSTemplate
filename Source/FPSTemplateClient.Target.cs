using UnrealBuildTool;
using System.Collections.Generic;

public class FPSTemplateClientTarget : TargetRules
{
	public FPSTemplateClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("FPSTemplate");
		ExtraModuleNames.Add("DedicatedServers");
	}
}