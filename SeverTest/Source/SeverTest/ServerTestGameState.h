// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ServerTestGameState.generated.h"

/**
 * 游戏状态类 - 在服务器和客户端之间同步游戏状态
 * 所有客户端都可以访问这个状态
 */
UCLASS()
class SEVERTEST_API AServerTestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AServerTestGameState();

	// 服务器启动时间
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Server")
	float ServerStartTime;

	// 游戏运行时间
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Server")
	float GameElapsedTime;

	// 获取服务器运行时间
	UFUNCTION(BlueprintCallable, Category = "Server")
	float GetServerUptime() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

