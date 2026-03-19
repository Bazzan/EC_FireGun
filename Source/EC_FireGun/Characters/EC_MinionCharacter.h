// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EC_Character.h"
#include "EC_MinionCharacter.generated.h"

/**
 * AI/minion character.
 * Defaults to orienting rotation to movement for navigation-driven motion.
 */
UCLASS()
class EC_FIREGUN_API AEC_MinionCharacter : public AEC_Character
{
	GENERATED_BODY()

public:
	AEC_MinionCharacter();
};

