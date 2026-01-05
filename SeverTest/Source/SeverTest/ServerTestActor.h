// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ServerTestActor.generated.h"

/**
 * 位置和旋转的同步数据结构
 */
USTRUCT(BlueprintType)
struct SEVERTEST_API FServerTestTransform
{
	GENERATED_BODY()

	// 位置
	UPROPERTY(BlueprintReadOnly)
	FVector Location;

	// 旋转
	UPROPERTY(BlueprintReadOnly)
	FRotator Rotation;

	// 默认构造函数
	FServerTestTransform()
		: Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
	{
	}

	// 带参数的构造函数
	FServerTestTransform(const FVector& InLocation, const FRotator& InRotation)
		: Location(InLocation)
		, Rotation(InRotation)
	{
	}

	// 相等比较运算符
	bool operator==(const FServerTestTransform& Other) const
	{
		return Location.Equals(Other.Location, 0.1f) && Rotation.Equals(Other.Rotation, 0.1f);
	}

	bool operator!=(const FServerTestTransform& Other) const
	{
		return !(*this == Other);
	}
};

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

	// 网络复制属性（合并的位置和旋转）
	UPROPERTY(ReplicatedUsing = OnRep_Transform, BlueprintReadOnly, Category = "Server")
	FServerTestTransform ReplicatedTransform;

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

	// 复制属性变化回调（合并的位置和旋转）
	UFUNCTION()
	void OnRep_Transform();

	// 服务器端更新逻辑
	void UpdateServerSide(float DeltaTime);

	// 获取生命周期复制属性
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

