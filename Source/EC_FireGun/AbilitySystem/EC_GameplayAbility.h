#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EC_GameplayAbility.generated.h"

class AEC_PlayerCharacter;
class UGameplayEffect;

/**
 * Project-wide base GameplayAbility.
 * Sets sensible defaults for the listen-server model and provides convenience accessors
 * that all EC abilities can use. Overrides lifecycle hooks and wraps common BlueprintCallable
 * functions so all ability BPs go through EC-owned nodes.
 */
UCLASS(Abstract)
class EC_FIREGUN_API UEC_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEC_GameplayAbility();

protected:
	// ── Lifecycle overrides (call Super so BP event graphs fire) ──

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	virtual void PreActivate(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool ShouldAbilityRespondToEvent(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayEventData* Payload) const override;

	/** Returns the avatar cast to AEC_PlayerCharacter, or nullptr. */
	AEC_PlayerCharacter* GetEC_PlayerCharacter() const;

public:
	// ── EC wrappers for BlueprintCallable nodes ──

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Apply GameplayEffect To Owner", Meta = (ScriptName = "EC_ApplyGameplayEffectToOwner"))
	FActiveGameplayEffectHandle EC_ApplyGameplayEffectToOwner(TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel = 1, int32 Stacks = 1);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Apply GameplayEffect Spec To Owner", Meta = (ScriptName = "EC_ApplyGameplayEffectSpecToOwner"))
	FActiveGameplayEffectHandle EC_ApplyGameplayEffectSpecToOwner(const FGameplayEffectSpecHandle EffectSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Apply GameplayEffect To Target", Meta = (ScriptName = "EC_ApplyGameplayEffectToTarget"))
	TArray<FActiveGameplayEffectHandle> EC_ApplyGameplayEffectToTarget(const FGameplayAbilityTargetDataHandle TargetData, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel = 1, int32 Stacks = 1);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Apply GameplayEffect Spec To Target", Meta = (ScriptName = "EC_ApplyGameplayEffectSpecToTarget"))
	TArray<FActiveGameplayEffectHandle> EC_ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpecHandle EffectSpecHandle, const FGameplayAbilityTargetDataHandle TargetData);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Remove GameplayEffect From Owner (Handle)", Meta = (ScriptName = "EC_RemoveGameplayEffectFromOwnerWithHandle"))
	void EC_RemoveGameplayEffectFromOwnerWithHandle(FActiveGameplayEffectHandle Handle, int32 StacksToRemove = 1);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Remove GameplayEffect From Owner (Granted Tags)", Meta = (ScriptName = "EC_RemoveGameplayEffectFromOwnerWithGrantedTags"))
	void EC_RemoveGameplayEffectFromOwnerWithGrantedTags(FGameplayTagContainer WithGrantedTags, int32 StacksToRemove = 1);

	UFUNCTION(BlueprintCallable, Category = "EC Ability", DisplayName = "Remove GameplayEffect From Owner (Asset Tags)", Meta = (ScriptName = "EC_RemoveGameplayEffectFromOwnerWithAssetTags"))
	void EC_RemoveGameplayEffectFromOwnerWithAssetTags(FGameplayTagContainer WithAssetTags, int32 StacksToRemove = 1);
};
