#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ShootAbilityBase.h"
#include "ShootProjectileAbility.generated.h"

class AEC_Projectile;

/**
 * Projectile shooting ability.
 * Spawns an AEC_Projectile on the server and passes damage info to it.
 * The projectile handles its own travel, collision, and damage application.
 */
UCLASS()
class EC_FIREGUN_API UEC_ShootProjectileAbility : public UEC_ShootAbilityBase
{
	GENERATED_BODY()

public:
	UEC_ShootProjectileAbility();

protected:
	/** Projectile class to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AEC_Projectile> ProjectileClass;

	/** Distance ahead of the aim origin to spawn the projectile, avoiding self-collision. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0", Units = "cm"))
	float SpawnOffset = 50.0f;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
