// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ServerTestActorSpawner.generated.h"

class AServerTestActor;

/**
 * ServerTestActor生成器类
 * 用于在服务器端动态创建ServerTestActor对象
 */
UCLASS()
class SEVERTEST_API AServerTestActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerTestActorSpawner();

	// 动态创建ServerTestActor对象
	// @param SpawnLocation 生成位置
	// @param SpawnRotation 生成旋转（可选，默认为零旋转）
	// @return 成功创建的ServerTestActor指针，失败返回nullptr
	UFUNCTION(BlueprintCallable, Category = "Server")
	AServerTestActor* SpawnServerTestActor(const FVector& SpawnLocation, const FRotator& SpawnRotation = FRotator::ZeroRotator);

	// 动态创建ServerTestActor对象（使用当前Actor的位置和旋转）
	// @return 成功创建的ServerTestActor指针，失败返回nullptr
	UFUNCTION(BlueprintCallable, Category = "Server")
	AServerTestActor* SpawnServerTestActorAtSelf();

	// 判断是否是服务器端
	UFUNCTION(BlueprintCallable, Category = "Server")
	bool IsServer() const;

protected:
	virtual void BeginPlay() override;

	// 已生成的Actor列表（用于管理）
	UPROPERTY()
	TArray<AServerTestActor*> SpawnedActors;
};

