#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EC_GameplayEffect.generated.h"

/**
 * Project-wide base GameplayEffect.
 * All gameplay effect Blueprints should inherit from this class.
 */
UCLASS(Abstract, Blueprintable)
class EC_FIREGUN_API UEC_GameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
};
