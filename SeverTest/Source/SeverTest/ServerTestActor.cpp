// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

AServerTestActor::AServerTestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true); // 启用移动复制

	// 创建根组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetMobility(EComponentMobility::Movable);

	// 创建静态网格组件
	// MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	// MeshComponent->SetupAttachment(RootComponent);

	// 设置默认网格（使用引擎的基础形状）
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeMesh.Object);
	}

	// 设置默认材质（使用引擎的基础材质）
	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (BaseMaterial.Succeeded())
	{
		MeshComponent->SetMaterial(0, BaseMaterial.Object);
	}

	// 设置网格大小
	MeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void AServerTestActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	/*ReplicatedLocation = InitialLocation;
	ReplicatedRotation = GetActorRotation();*/

	if (HasAuthority())
	{
		// 服务器端日志 - 蓝色
		UE_LOG(LogTemp, Warning, TEXT("[SERVER] ServerTestActor spawned on server at location: %s"), *InitialLocation.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
				FString::Printf(TEXT("[SERVER] ServerTestActor spawned at location: %s"), *InitialLocation.ToString()));
		}
	}
	else
	{
		// 客户端日志 - 绿色
		UE_LOG(LogTemp, Warning, TEXT("[CLIENT] ServerTestActor spawned on client at location: %s"), *InitialLocation.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
				FString::Printf(TEXT("[CLIENT] ServerTestActor spawned at location: %s"), *InitialLocation.ToString()));
		}
	}
}

void AServerTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 只在服务器端执行移动逻辑
	if (HasAuthority())
	{
		UpdateServerSide(DeltaTime);
	}
}

void AServerTestActor::UpdateServerSide(float DeltaTime)
{
	// 服务器端逻辑 - 蓝色日志
	static float LogInterval = 0.0f;
	LogInterval += DeltaTime;

	if (LogInterval >= 2.0f) // 每2秒输出一次日志，避免日志过多
	{
		LogInterval = 0.0f;
		UE_LOG(LogTemp, Log, TEXT("[SERVER] ServerTestActor executing server-side update logic"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,
				TEXT("[SERVER] Executing server-side update logic"));
		}
	}

	TimeAccumulator += DeltaTime;

	// 圆形移动
	float Angle = TimeAccumulator * MoveSpeed / MoveRadius;
	FVector NewLocation = InitialLocation;
	NewLocation.X += FMath::Cos(Angle) * MoveRadius;
	NewLocation.Y += FMath::Sin(Angle) * MoveRadius;
	NewLocation.Z = InitialLocation.Z + FMath::Sin(Angle * 2.0f) * 100.0f; // 上下浮动

	// 旋转
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotationSpeed * DeltaTime;
	NewRotation.Pitch = FMath::Sin(TimeAccumulator) * 30.0f;
	NewRotation.Roll = FMath::Cos(TimeAccumulator * 0.7f) * 20.0f;

	// 更新位置和旋转
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);

	// 更新复制属性
	// ReplicatedLocation = NewLocation;
	// ReplicatedRotation = NewRotation;
}

// void AServerTestActor::OnRep_Location()
// {
// 	// 客户端接收到位置更新 - 绿色日志
// 	if (!HasAuthority())
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("[CLIENT] ServerTestActor received location update: %s"), *ReplicatedLocation.ToString());
// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
// 				FString::Printf(TEXT("[CLIENT] Received location update: %s"), *ReplicatedLocation.ToString()));
// 		}
// 		SetActorLocation(ReplicatedLocation);
// 	}
// }

// void AServerTestActor::OnRep_Rotation()
// {
// 	// 客户端接收到旋转更新 - 绿色日志
// 	if (!HasAuthority())
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("[CLIENT] ServerTestActor received rotation update: %s"), *ReplicatedRotation.ToString());
// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
// 				FString::Printf(TEXT("[CLIENT] Received rotation update: %s"), *ReplicatedRotation.ToString()));
// 		}
// 		SetActorRotation(ReplicatedRotation);
// 	}
// }

//void AServerTestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	// 复制位置和旋转到所有客户端
//	DOREPLIFETIME(AServerTestActor, ReplicatedLocation);
//	DOREPLIFETIME(AServerTestActor, ReplicatedRotation);
//}

