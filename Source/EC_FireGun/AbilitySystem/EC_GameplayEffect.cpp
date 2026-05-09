#include "AbilitySystem/EC_GameplayEffect.h"
#include "ActiveGameplayEffectsContainer.h"
#include "GameplayEffectSpec.h"
#include "GameplayPrediction.h"

void UEC_GameplayEffect::PostInitProperties()
{
	Super::PostInitProperties();
}

bool UEC_GameplayEffect::CanApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	return Super::CanApply(ActiveGEContainer, GESpec);
}

bool UEC_GameplayEffect::OnAddedToActiveContainer(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	return Super::OnAddedToActiveContainer(ActiveGEContainer, ActiveGE);
}

void UEC_GameplayEffect::OnExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnExecuted(ActiveGEContainer, GESpec, PredictionKey);
}

void UEC_GameplayEffect::OnApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnApplied(ActiveGEContainer, GESpec, PredictionKey);
}
