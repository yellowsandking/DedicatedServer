// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ServerTestPlayerController.generated.h"

/**
 * 玩家控制器类 - 处理玩家输入和服务器通信
 * 客户端可以发送RPC到服务器，服务器可以同步数据到客户端
 */
UCLASS()
class SEVERTEST_API AServerTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AServerTestPlayerController();

	virtual void BeginPlay() override;

	// 客户端连接到服务器后调用
	virtual void OnPossess(APawn* InPawn) override;

	// 服务器端：处理客户端请求
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestServerTime();

	// 客户端：接收服务器时间
	UFUNCTION(Client, Reliable)
	void ClientReceiveServerTime(float ServerTime);

	// 服务器端：处理玩家消息
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMessage(const FString& Message);

	// 客户端：接收广播消息
	UFUNCTION(Client, Reliable)
	void ClientReceiveMessage(const FString& Message, const FString& PlayerName);

	// 客户端：发送消息到服务器
	UFUNCTION(BlueprintCallable, Category = "Server")
	void SendMessageToServer(const FString& Message);

	// 客户端：请求服务器时间
	UFUNCTION(BlueprintCallable, Category = "Server")
	void RequestServerTime();

	// 判断是否是服务器端（同时检查HasAuthority和NetMode）
	UFUNCTION(BlueprintCallable, Category = "Server")
	bool IsServer() const;

protected:
	// 输入绑定
	virtual void SetupInputComponent() override;

	// 处理输入
	void OnJumpPressed();
	void OnJumpReleased();

	// 服务器时间同步
	void UpdateServerTime();

	// 初始化服务器时间同步（延迟调用）
	void InitializeServerTimeSync();

	FTimerHandle ServerTimeUpdateTimer;
	FTimerHandle InitialDelayTimer;
};

