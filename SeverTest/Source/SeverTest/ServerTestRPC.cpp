// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestRPC.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AServerTestRPC::AServerTestRPC()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	MessageCount = 0;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AServerTestRPC::BeginPlay()
{
	Super::BeginPlay();

	ENetMode NetMode = GetNetMode();
	FString NetModeString = (NetMode == NM_DedicatedServer) ? TEXT("DedicatedServer") :
		(NetMode == NM_ListenServer) ? TEXT("ListenServer") :
		(NetMode == NM_Client) ? TEXT("Client") :
		(NetMode == NM_Standalone) ? TEXT("Standalone") : TEXT("Unknown");

	UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] BeginPlay - NetMode: %s, HasAuthority: %d"),
		*NetModeString, HasAuthority() ? 1 : 0);

	if (IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] Running on server"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
				TEXT("[ServerTestRPC] Running on server"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] Running on client"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
				TEXT("[ServerTestRPC] Running on client"));
		}
	}
}

void AServerTestRPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ========== Server RPC 实现 ==========

void AServerTestRPC::ServerSendMessage_Implementation(const FString& Message)
{
	// 服务器端执行
	MessageCount++;
	LastReceivedMessage = Message;

	FString PlayerName = TEXT("Unknown");
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PlayerName = PC->GetName();
	}

	UE_LOG(LogTemp, Warning, TEXT("[SERVER RPC] Received message from client: %s (Player: %s, Count: %d)"),
		*Message, *PlayerName, MessageCount);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			FString::Printf(TEXT("[SERVER RPC] Message: %s (Count: %d)"), *Message, MessageCount));
	}

	// 服务器收到消息后，广播给所有客户端
	MulticastBroadcastMessage(Message, PlayerName);
}

bool AServerTestRPC::ServerSendMessage_Validate(const FString& Message)
{
	// 验证：消息不能为空且长度不超过256
	return !Message.IsEmpty() && Message.Len() <= 256;
}

void AServerTestRPC::ServerRequestCalculation_Implementation(float Value1, float Value2)
{
	// 服务器端执行计算
	float Result = Value1 + Value2;

	UE_LOG(LogTemp, Warning, TEXT("[SERVER RPC] Calculation: %.2f + %.2f = %.2f"), Value1, Value2, Result);

	// 将结果发送回客户端
	ClientReceiveCalculationResult(Result);
}

bool AServerTestRPC::ServerRequestCalculation_Validate(float Value1, float Value2)
{
	// 验证：值必须在合理范围内
	return FMath::Abs(Value1) < 1000000.0f && FMath::Abs(Value2) < 1000000.0f;
}

void AServerTestRPC::ServerRequestServerTime_Implementation()
{
	// 服务器端返回当前时间
	float ServerTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Log, TEXT("[SERVER RPC] Server time requested: %.2f"), ServerTime);

	// 发送时间到客户端
	ClientReceiveServerTime(ServerTime);
}

bool AServerTestRPC::ServerRequestServerTime_Validate()
{
	return true;
}

// ========== Client RPC 实现 ==========

void AServerTestRPC::ClientReceiveMessage_Implementation(const FString& Message, const FString& PlayerName)
{
	// 客户端执行
	UE_LOG(LogTemp, Warning, TEXT("[CLIENT RPC] Received message from server: %s (From: %s)"),
		*Message, *PlayerName);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("[CLIENT RPC] Message: %s (From: %s)"), *Message, *PlayerName));
	}
}

void AServerTestRPC::ClientReceiveCalculationResult_Implementation(float Result)
{
	// 客户端执行
	UE_LOG(LogTemp, Warning, TEXT("[CLIENT RPC] Received calculation result: %.2f"), Result);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("[CLIENT RPC] Calculation result: %.2f"), Result));
	}
}

void AServerTestRPC::ClientReceiveServerTime_Implementation(float ServerTime)
{
	// 客户端执行
	float ClientTime = GetWorld()->GetTimeSeconds();
	float Latency = ClientTime - ServerTime;

	UE_LOG(LogTemp, Warning, TEXT("[CLIENT RPC] Server time: %.2f, Client time: %.2f, Latency: %.2f"),
		ServerTime, ClientTime, Latency);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
			FString::Printf(TEXT("[CLIENT RPC] Server: %.2f | Latency: %.2f"), ServerTime, Latency));
	}
}

// ========== NetMulticast RPC 实现 ==========

void AServerTestRPC::MulticastBroadcastMessage_Implementation(const FString& Message, const FString& SenderName)
{
	// 所有客户端和服务器都执行
	UE_LOG(LogTemp, Warning, TEXT("[MULTICAST RPC] Broadcast message: %s (From: %s)"),
		*Message, *SenderName);

	if (GEngine)
	{
		FColor DisplayColor = IsServer() ? FColor::Magenta : FColor::Orange;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, DisplayColor,
			FString::Printf(TEXT("[MULTICAST] %s: %s"), *SenderName, *Message));
	}
}

void AServerTestRPC::MulticastGameEvent_Implementation(const FString& EventName, const FString& EventData)
{
	// 所有客户端和服务器都执行
	UE_LOG(LogTemp, Warning, TEXT("[MULTICAST RPC] Game event: %s - %s"),
		*EventName, *EventData);

	if (GEngine)
	{
		FColor DisplayColor = IsServer() ? FColor::Magenta : FColor::Orange;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, DisplayColor,
			FString::Printf(TEXT("[MULTICAST] Event: %s - %s"), *EventName, *EventData));
	}
}

// ========== 测试方法 ==========

void AServerTestRPC::TestAllRPCs()
{
	if (!IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] TestAllRPCs can only be called on server!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] Starting RPC tests..."));

	// 测试1: Multicast广播消息
	MulticastBroadcastMessage(TEXT("Test message from server"), TEXT("Server"));

	// 测试2: Multicast游戏事件
	MulticastGameEvent(TEXT("TestEvent"), TEXT("TestData123"));

	// 测试3: 发送消息到所有客户端
	if (UWorld* World = GetWorld())
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				FString PlayerName = PC->GetName();
				ClientReceiveMessage(FString::Printf(TEXT("Hello %s!"), *PlayerName), TEXT("Server"));
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] RPC tests completed!"));
}

void AServerTestRPC::ClientTestRPCs()
{
	if (IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] ClientTestRPCs should be called from client!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] Client starting RPC tests..."));

	// 测试1: 发送消息到服务器
	ServerSendMessage(TEXT("Hello from client!"));

	// 测试2: 请求计算
	ServerRequestCalculation(10.5f, 20.3f);

	// 测试3: 请求服务器时间
	ServerRequestServerTime();

	UE_LOG(LogTemp, Warning, TEXT("[ServerTestRPC] Client RPC tests sent!"));
}

bool AServerTestRPC::IsServer() const
{
	// 同时检查HasAuthority和NetMode，确保是服务器端
	return HasAuthority() && GetNetMode() != NM_Client;
}

void AServerTestRPC::ExecuteTest()
{
	if (IsServer())
	{
		TestAllRPCs();
	}
	else
	{
		ClientTestRPCs();
	}
}

void AServerTestRPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AServerTestRPC, MessageCount);
	DOREPLIFETIME(AServerTestRPC, LastReceivedMessage);
}

