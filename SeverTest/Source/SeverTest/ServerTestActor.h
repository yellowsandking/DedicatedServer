// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ServerTestActor.generated.h"

/**
 * 演示Actor类 - 展示服务器-客户端同步
 * 这个Actor在服务器端控制，客户端会看到同步的位置和旋转
 */
UCLASS()
class SEVERTEST_API AServerTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerTestActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 网络复制属性
	UPROPERTY(ReplicatedUsing = OnRep_Location, BlueprintReadOnly, Category = "Server")
	FVector ReplicatedLocation;

	UPROPERTY(ReplicatedUsing = OnRep_Rotation, BlueprintReadOnly, Category = "Server")
	FRotator ReplicatedRotation;

	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server")
	float MoveSpeed = 100.0f;

	// 旋转速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server")
	float RotationSpeed = 45.0f;

	// 移动范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server")
	float MoveRadius = 500.0f;

protected:
	// 静态网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// 初始位置
	FVector InitialLocation;

	// 时间累积
	float TimeAccumulator = 0.0f;

	// 复制属性变化回调
	UFUNCTION()
	void OnRep_Location();

	UFUNCTION()
	void OnRep_Rotation();

	// 服务器端更新逻辑
	void UpdateServerSide(float DeltaTime);

	// 获取生命周期复制属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

