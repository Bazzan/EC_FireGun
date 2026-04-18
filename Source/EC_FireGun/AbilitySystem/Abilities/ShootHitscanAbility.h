#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ShootAbilityBase.h"
#include "ShootHitscanAbility.generated.h"

/**
 * Hitscan shooting ability.
 * Performs an instant server-side line trace and applies the damage effect
 * to the first valid target hit.
 */
UCLASS()
class EC_FIREGUN_API UShootHitscanAbility : public UShootAbilityBase
{
	GENERATED_BODY()

public:
	UShootHitscanAbility();

protected:
	/** Collision channel used for the hitscan trace. */
	UPROPERTY(EditDefaultsOnly, Category = "Hitscan")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	/** Blueprint hook for spawning cosmetic effects (tracer, impact VFX, etc.). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hitscan", meta = (DisplayName = "On Hitscan Hit"))
	void BP_OnHitscanHit(const FHitResult& Hit);

	/** Blueprint hook called when the trace misses everything. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hitscan", meta = (DisplayName = "On Hitscan Miss"))
	void BP_OnHitscanMiss(const FVector& TraceEnd);
};
