#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "EC_GameplayEffect.generated.h"

struct FActiveGameplayEffectsContainer;
struct FActiveGameplayEffect;
struct FGameplayEffectSpec;
struct FPredictionKey;

/**
 * Project-wide base GameplayEffect.
 * All gameplay effect Blueprints should inherit from this class.
 */
UCLASS(Abstract, Blueprintable)
class EC_FIREGUN_API UEC_GameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

protected:
	/** Notify — called before this GE is applied. Return false to inhibit application. */
	virtual bool CanApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;

	/** Notify — called after this GE is added to an active container. Return false to inhibit activation. */
	virtual bool OnAddedToActiveContainer(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

	/** Notify — called when an instant GE is executed (not added to the container). */
	virtual void OnExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	/** Notify — called after this GE is applied (added to container or executed, including on-stack re-application). */
	virtual void OnApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

public:
	virtual void PostInitProperties() override;
};
