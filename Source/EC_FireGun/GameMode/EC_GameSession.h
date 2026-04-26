// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "EC_GameSession.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogECSession, Log, All);

/**
 * EC session class. Owns login policy for the listen-server host.
 * Caps party size at 3 per Design.md §4 (Session Structure).
 * Spawned by AEC_FireGunGameModeBase via GameSessionClass; both Hub and Run modes inherit.
 */
UCLASS()
class EC_FIREGUN_API AEC_GameSession : public AGameSession
{
	GENERATED_BODY()

public:
	AEC_GameSession();

	//~Begin AGameSession
	virtual FString ApproveLogin(const FString& Options) override;
	//~End AGameSession
};
