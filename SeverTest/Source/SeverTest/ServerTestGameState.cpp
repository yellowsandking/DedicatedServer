// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerTestGameState.h"
#include "Net/UnrealNetwork.h"

AServerTestGameState::AServerTestGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ServerStartTime = 0.0f;
	GameElapsedTime = 0.0f;
}

void AServerTestGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ServerStartTime = GetWorld()->GetTimeSeconds();
		GameElapsedTime = 0.0f;
	}
}

void AServerTestGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		GameElapsedTime = GetWorld()->GetTimeSeconds() - ServerStartTime;
	}
}

float AServerTestGameState::GetServerUptime() const
{
	return GameElapsedTime;
}

void AServerTestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AServerTestGameState, ServerStartTime);
	DOREPLIFETIME(AServerTestGameState, GameElapsedTime);
}

