// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ServerTestGameMode.generated.h"

/**
 * 游戏模式类 - 支持Dedicated Server和客户端
 * 这个类在服务器端和客户端都会存在，但服务器端负责主要的游戏逻辑
 */
UCLASS()
class SEVERTEST_API AServerTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AServerTestGameMode();

	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 服务器端初始化
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	// 当玩家加入时调用
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 当玩家离开时调用
	virtual void Logout(AController* Exiting) override;

	// 获取当前连接的玩家数量
	UFUNCTION(BlueprintCallable, Category = "Server")
	int32 GetPlayerCount() const;

	// 判断是否是服务器端（同时检查HasAuthority和NetMode）
	UFUNCTION(BlueprintCallable, Category = "Server")
	bool IsServer() const;

protected:
	// 服务器端日志输出
	UFUNCTION(BlueprintCallable, Category = "Server")
	void LogServerMessage(const FString& Message);

	// 当前玩家数量
	UPROPERTY(Replicated)
	int32 CurrentPlayerCount;
};

