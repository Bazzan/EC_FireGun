// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/EC_FireGunGameModeBase.h"
#include "GameMode/EC_GameSession.h"
#include "Player/EC_PlayerState.h"

AEC_FireGunGameModeBase::AEC_FireGunGameModeBase()
{
	PlayerStateClass = AEC_PlayerState::StaticClass();
	GameSessionClass = AEC_GameSession::StaticClass();
}
