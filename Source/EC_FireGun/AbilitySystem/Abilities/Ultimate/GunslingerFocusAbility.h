#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Ultimate/UltimateAbilityBase.h"
#include "ActiveGameplayEffectHandle.h"
#include "GunslingerFocusAbility.generated.h"

class UGameplayEffect;

/**
 * Gunslinger Ultimate: enters a short Focus mode.
 * Applies a duration GameplayEffect to self that scales OutgoingDamageMultiplier
 * and grants the State.Focus tag for VFX/SFX hooks. Ends when the buff expires.
 */
UCLASS()
class EC_FIREGUN_API UGunslingerFocusAbility : public UUltimateAbilityBase
{
	GENERATED_BODY()

public:
	UGunslingerFocusAbility();

protected:
	/** Duration GameplayEffect applied to self for the focus window (e.g. GE_FocusMode). */
	UPROPERTY(EditDefaultsOnly, Category = "Focus")
	TSubclassOf<UGameplayEffect> BuffEffect;

	/** Fallback duration if BuffEffect duration cannot be resolved. */
	UPROPERTY(EditDefaultsOnly, Category = "Focus", meta = (ClampMin = "0.1", Units = "s"))
	float FallbackDuration = 5.0f;

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

	/** Called on the avatar when Focus starts (cosmetic hook for HUD/sound/cue). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Focus", meta = (DisplayName = "On Focus Start"))
	void BP_OnFocusStart(float Duration);

	/** Called on the avatar when Focus ends (cosmetic hook). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Focus", meta = (DisplayName = "On Focus End"))
	void BP_OnFocusEnd();

private:
	FActiveGameplayEffectHandle ActiveBuffHandle;
	FTimerHandle EndTimerHandle;

	void OnFocusTimerExpired();
};
