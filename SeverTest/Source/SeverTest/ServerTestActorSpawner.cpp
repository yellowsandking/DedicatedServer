// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestActorSpawner.h"
#include "ServerTestActor.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

AServerTestActorSpawner::AServerTestActorSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false; // Spawner本身不需要复制
}

void AServerTestActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 输出生成器信息
	ENetMode NetMode = GetNetMode();
	FString NetModeString = (NetMode == NM_DedicatedServer) ? TEXT("DedicatedServer") : 
	                        (NetMode == NM_ListenServer) ? TEXT("ListenServer") : 
	                        (NetMode == NM_Client) ? TEXT("Client") : 
	                        (NetMode == NM_Standalone) ? TEXT("Standalone") : TEXT("Unknown");
	
	UE_LOG(LogTemp, Warning, TEXT("[ServerTestActorSpawner] BeginPlay - NetMode: %s, HasAuthority: %d"), 
		*NetModeString, HasAuthority() ? 1 : 0);
}

AServerTestActor* AServerTestActorSpawner::SpawnServerTestActor(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	// 检查是否在服务器端
	if (!IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestActorSpawner] SpawnServerTestActor called on client! Only server can spawn actors."));
		return nullptr;
	}

	// 检查World是否有效
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[ServerTestActorSpawner] World is null! Cannot spawn actor."));
		return nullptr;
	}

	// 设置生成参数
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 生成Actor
	AServerTestActor* SpawnedActor = World->SpawnActor<AServerTestActor>(
		AServerTestActor::StaticClass(),
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (SpawnedActor)
	{
		// 添加到管理列表
		SpawnedActors.Add(SpawnedActor);
		
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestActorSpawner] Successfully spawned ServerTestActor at location: %s"), 
			*SpawnLocation.ToString());
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
				FString::Printf(TEXT("[Spawner] Spawned ServerTestActor at: %s"), *SpawnLocation.ToString()));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[ServerTestActorSpawner] Failed to spawn ServerTestActor at location: %s"), 
			*SpawnLocation.ToString());
	}

	return SpawnedActor;
}

AServerTestActor* AServerTestActorSpawner::SpawnServerTestActorAtSelf()
{
	// 使用当前Actor的位置和旋转
	return SpawnServerTestActor(GetActorLocation(), GetActorRotation());
}

bool AServerTestActorSpawner::IsServer() const
{
	// 同时检查HasAuthority和NetMode，确保是服务器端
	return HasAuthority() && GetNetMode() != NM_Client;
}

