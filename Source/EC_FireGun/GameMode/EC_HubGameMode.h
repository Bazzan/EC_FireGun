// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/EC_FireGunGameModeBase.h"
#include "EC_HubGameMode.generated.h"

/**
 * Hub / between-runs flow: meta progression, party, class select staging (Design.md §3, §8.2).
 * Use on hub maps; switch to AEC_FireGunGameMode (or a BP child) when starting a run.
 */
UCLASS()
class EC_FIREGUN_API AEC_HubGameMode : public AEC_FireGunGameModeBase
{
	GENERATED_BODY()
};
