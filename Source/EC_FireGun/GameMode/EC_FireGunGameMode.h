// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/EC_FireGunGameModeBase.h"
#include "EC_FireGunGameMode.generated.h"

/**
 * FireGun gameplay game mode: active run / combat session (rooms, encounters, rewards — Design.md §3–4, §8.1).
 * Assign to run maps or travel here when the party leaves the hub.
 */
UCLASS(abstract)
class EC_FIREGUN_API AEC_FireGunGameMode : public AEC_FireGunGameModeBase
{
	GENERATED_BODY()

public:
	AEC_FireGunGameMode();
};


