// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/EC_GameSession.h"
#include "GameMode/EC_FireGunGameModeBase.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

DEFINE_LOG_CATEGORY(LogECSession);

AEC_GameSession::AEC_GameSession()
{
	MaxPlayers = 3;
}

FString AEC_GameSession::ApproveLogin(const FString& Options)
{
	FString Reason = Super::ApproveLogin(Options);
	if (!Reason.IsEmpty())
	{
		const UWorld* World = GetWorld();
		const AEC_FireGunGameModeBase* GameMode = World ? World->GetAuthGameMode<AEC_FireGunGameModeBase>() : nullptr;
		const int32 CurrentPlayers = GameMode ? const_cast<AEC_FireGunGameModeBase*>(GameMode)->GetNumPlayers() : -1;

		UE_LOG(LogECSession, Warning,
			TEXT("Login rejected (Players=%d / Max=%d): %s"),
			CurrentPlayers, MaxPlayers, *Reason);
	}
	return Reason;
}
