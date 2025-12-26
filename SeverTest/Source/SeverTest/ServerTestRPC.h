// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ServerTestRPC.generated.h"

/**
 * RPC测试类
 * 演示各种RPC（Remote Procedure Call）的使用方法
 * - Server RPC: 客户端调用，在服务器执行
 * - Client RPC: 服务器调用，在客户端执行
 * - NetMulticast RPC: 服务器调用，在所有客户端执行
 */
UCLASS()
class SEVERTEST_API AServerTestRPC : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerTestRPC();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ========== Server RPC (客户端调用，服务器执行) ==========
	
	// 客户端发送消息到服务器
	// @param Message 要发送的消息
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "RPC|Server")
	void ServerSendMessage(const FString& Message);

	// 客户端请求服务器执行计算
	// @param Value1 第一个值
	// @param Value2 第二个值
	// @return 计算结果（通过Client RPC返回）
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "RPC|Server")
	void ServerRequestCalculation(float Value1, float Value2);

	// 客户端请求服务器时间
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "RPC|Server")
	void ServerRequestServerTime();

	// ========== Client RPC (服务器调用，客户端执行) ==========
	
	// 服务器发送消息到特定客户端
	// @param Message 要发送的消息
	// @param PlayerName 玩家名称
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "RPC|Client")
	void ClientReceiveMessage(const FString& Message, const FString& PlayerName);

	// 服务器发送计算结果到客户端
	// @param Result 计算结果
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "RPC|Client")
	void ClientReceiveCalculationResult(float Result);

	// 服务器发送时间到客户端
	// @param ServerTime 服务器时间
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "RPC|Client")
	void ClientReceiveServerTime(float ServerTime);

	// ========== NetMulticast RPC (服务器调用，所有客户端执行) ==========
	
	// 服务器广播消息到所有客户端
	// @param Message 要广播的消息
	// @param SenderName 发送者名称
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "RPC|Multicast")
	void MulticastBroadcastMessage(const FString& Message, const FString& SenderName);

	// 服务器广播游戏事件到所有客户端
	// @param EventName 事件名称
	// @param EventData 事件数据
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "RPC|Multicast")
	void MulticastGameEvent(const FString& EventName, const FString& EventData);

	// ========== 测试方法 ==========
	
	// 测试所有RPC功能（在服务器端调用）
	UFUNCTION(BlueprintCallable, Category = "RPC|Test")
	void TestAllRPCs();

	// 客户端测试方法（从客户端调用）
	UFUNCTION(BlueprintCallable, Category = "RPC|Test")
	void ClientTestRPCs();

	// 判断是否是服务器端
	UFUNCTION(BlueprintCallable, Category = "RPC")
	bool IsServer() const;

protected:
	// 消息计数（用于测试）
	UPROPERTY(Replicated)
	int32 MessageCount = 0;

	// 最后接收到的消息
	UPROPERTY(Replicated)
	FString LastReceivedMessage;

	// 测试定时器
	FTimerHandle TestTimerHandle;
	
	// 执行测试
	void ExecuteTest();

	// 获取生命周期复制属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

