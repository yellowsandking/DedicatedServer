// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestPlayerController.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

AServerTestPlayerController::AServerTestPlayerController()
{
	bReplicates = true;
}

void AServerTestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 客户端连接到服务器后，请求服务器时间
	if (IsLocalController())
	{
		// 延迟一点时间确保连接已建立
		GetWorld()->GetTimerManager().SetTimer(InitialDelayTimer, this, 
			&AServerTestPlayerController::InitializeServerTimeSync, 1.0f, false);
	}
}

void AServerTestPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsServer())
	{
		UE_LOG(LogTemp, Log, TEXT("Server: Player %s possessed pawn"), *GetName());
	}
	else if (IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("Client: Successfully connected to server"));
	}
}

void AServerTestPlayerController::ServerRequestServerTime_Implementation()
{
	float ServerTime = GetWorld()->GetTimeSeconds();
	ClientReceiveServerTime(ServerTime);
}

bool AServerTestPlayerController::ServerRequestServerTime_Validate()
{
	return true;
}

void AServerTestPlayerController::ClientReceiveServerTime_Implementation(float ServerTime)
{
	if (GEngine)
	{
		float ClientTime = GetWorld()->GetTimeSeconds();
		float Latency = ClientTime - ServerTime;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan,
			FString::Printf(TEXT("Server Time: %.2f | Latency: %.2f"), ServerTime, Latency));
	}
}

void AServerTestPlayerController::ServerSendMessage_Implementation(const FString& Message)
{
	// 服务器接收消息后，广播给所有客户端
	FString PlayerName = GetName();
	
	// 在服务器端也显示
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White,
			FString::Printf(TEXT("[%s]: %s"), *PlayerName, *Message));
	}

	UE_LOG(LogTemp, Log, TEXT("[%s]: %s"), *PlayerName, *Message);

	// 广播给所有客户端
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (AServerTestPlayerController* TestPC = Cast<AServerTestPlayerController>(PC))
			{
				TestPC->ClientReceiveMessage(Message, PlayerName);
			}
		}
	}
}

bool AServerTestPlayerController::ServerSendMessage_Validate(const FString& Message)
{
	return !Message.IsEmpty() && Message.Len() < 256; // 简单验证
}

void AServerTestPlayerController::ClientReceiveMessage_Implementation(const FString& Message, const FString& PlayerName)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
			FString::Printf(TEXT("[%s]: %s"), *PlayerName, *Message));
	}
}

void AServerTestPlayerController::SendMessageToServer(const FString& Message)
{
	if (IsLocalController())
	{
		ServerSendMessage(Message);
	}
}

void AServerTestPlayerController::RequestServerTime()
{
	if (IsLocalController())
	{
		ServerRequestServerTime();
	}
}

void AServerTestPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		// 绑定输入
		InputComponent->BindAction("Jump", IE_Pressed, this, &AServerTestPlayerController::OnJumpPressed);
		InputComponent->BindAction("Jump", IE_Released, this, &AServerTestPlayerController::OnJumpReleased);
	}
}

void AServerTestPlayerController::OnJumpPressed()
{
	// 发送消息到服务器
	SendMessageToServer(TEXT("Player pressed Jump!"));
}

void AServerTestPlayerController::OnJumpReleased()
{
	// 发送消息到服务器
	SendMessageToServer(TEXT("Player released Jump!"));
}

void AServerTestPlayerController::InitializeServerTimeSync()
{
	if (IsLocalController())
	{
		// 立即请求一次服务器时间
		RequestServerTime();
		
		// 每5秒同步一次服务器时间
		GetWorld()->GetTimerManager().SetTimer(ServerTimeUpdateTimer, this, 
			&AServerTestPlayerController::UpdateServerTime, 5.0f, true);
	}
}

bool AServerTestPlayerController::IsServer() const
{
	// 同时检查HasAuthority和NetMode，确保是服务器端
	return HasAuthority() && GetNetMode() != NM_Client;
}

void AServerTestPlayerController::UpdateServerTime()
{
	if (IsLocalController())
	{
		RequestServerTime();
	}
}

