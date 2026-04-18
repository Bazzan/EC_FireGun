#include "AbilitySystem/Abilities/Ultimate/UltimateAbilityBase.h"
#include "EC_GameplayTags.h"

DEFINE_LOG_CATEGORY(LogClassAbility);

UUltimateAbilityBase::UUltimateAbilityBase()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(EC_GameplayTags::Ability_Ultimate);
	SetAssetTags(Tags);

	ActivationBlockedTags.AddTag(EC_GameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(EC_GameplayTags::Cooldown_Ultimate);
}
