#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EC_GameplayAbility.generated.h"

class AEC_PlayerCharacter;

/**
 * Project-wide base GameplayAbility.
 * Sets sensible defaults for the listen-server model and provides convenience accessors
 * that all EC abilities can use.
 */
UCLASS(Abstract)
class EC_FIREGUN_API UEC_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEC_GameplayAbility();

protected:
	/** Returns the avatar cast to AEC_PlayerCharacter, or nullptr. */
	AEC_PlayerCharacter* GetEC_PlayerCharacter() const;
};
