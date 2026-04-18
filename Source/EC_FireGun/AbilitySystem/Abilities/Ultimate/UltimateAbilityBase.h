#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EC_GameplayAbility.h"
#include "UltimateAbilityBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClassAbility, Log, All);

/**
 * Shared base for class Ultimate abilities.
 * Sets common asset/blocked tags so concrete ultimates only define their own behavior.
 */
UCLASS(Abstract)
class EC_FIREGUN_API UUltimateAbilityBase : public UEC_GameplayAbility
{
	GENERATED_BODY()

public:
	UUltimateAbilityBase();
};
