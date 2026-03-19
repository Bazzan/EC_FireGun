// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EC_FireGunGameModeBase.generated.h"

/**
 * Shared game-mode defaults for EC_FireGun (listen-server co-op): player state, future shared rules.
 * Derive hub and run/gameplay modes from this rather than duplicating setup.
 */
UCLASS(Abstract)
class EC_FIREGUN_API AEC_FireGunGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEC_FireGunGameModeBase();
};
