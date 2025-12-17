// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestGameMode.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

AServerTestGameMode::AServerTestGameMode()
{
	// 设置默认Pawn类（如果需要）
	// DefaultPawnClass = nullptr;
	
	// 启用网络复制
	bReplicates = true;
	CurrentPlayerCount = 0;
}

void AServerTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 检查是否在服务器端运行
	if (HasAuthority())
	{
		LogServerMessage(TEXT("Dedicated Server Started!"));
		LogServerMessage(FString::Printf(TEXT("Server is running on map: %s"), *GetWorld()->GetMapName()));
	}
}

void AServerTestGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (HasAuthority())
	{
		LogServerMessage(FString::Printf(TEXT("Server initialized with map: %s"), *MapName));
	}
}

void AServerTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		CurrentPlayerCount++;
		LogServerMessage(FString::Printf(TEXT("Player joined! Total players: %d"), CurrentPlayerCount));
		
		// 向所有客户端广播玩家加入消息
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
				FString::Printf(TEXT("Player %s joined the server"), *NewPlayer->GetName()));
		}
	}
}

void AServerTestGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (HasAuthority())
	{
		CurrentPlayerCount = FMath::Max(0, CurrentPlayerCount - 1);
		LogServerMessage(FString::Printf(TEXT("Player left! Remaining players: %d"), CurrentPlayerCount));
	}
}

int32 AServerTestGameMode::GetPlayerCount() const
{
	return CurrentPlayerCount;
}

void AServerTestGameMode::LogServerMessage(const FString& Message)
{
	if (HasAuthority())
	{
		// 在服务器控制台输出
		UE_LOG(LogTemp, Warning, TEXT("[SERVER] %s"), *Message);
		
		// 如果是在编辑器或PIE模式下，也在屏幕上显示
		if (GEngine && GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
				FString::Printf(TEXT("[SERVER] %s"), *Message));
		}
	}
}

void AServerTestGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AServerTestGameMode, CurrentPlayerCount);
}

