// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

/**
 * Dedicated Server Target - 用于构建专用服务器
 * 这个target会构建一个没有渲染的服务器版本
 */
public class SeverTestServerTarget : TargetRules
{
	public SeverTestServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server; // 设置为Server类型
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("SeverTest");
		
		// 服务器不需要渲染
		bBuildDeveloperTools = false;
	}
}

