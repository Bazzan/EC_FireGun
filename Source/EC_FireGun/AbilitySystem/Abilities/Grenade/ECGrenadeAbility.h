#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EC_GameplayAbility.h"
#include "ECGrenadeAbility.generated.h"

class AECGrenade;
class UGameplayEffect;

/**
 * Generic grenade ability: spawns a grenade actor on the server and applies a launch velocity
 * along the camera aim direction. Class-specific flavor is handled by the GrenadeClass and
 * Cooldown GameplayEffect set on each Blueprint subclass (e.g. BP_GrenadeAbility_GunslingerFrag).
 */
UCLASS()
class EC_FIREGUN_API UECGrenadeAbility : public UEC_GameplayAbility
{
	GENERATED_BODY()

public:
	UECGrenadeAbility();

protected:
	/** Grenade actor class to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	TSubclassOf<AECGrenade> GrenadeClass;

	/** GameplayEffect applied by the grenade on detonation. Should use SetByCaller for Data.Damage. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Base damage value passed to the grenade for SetByCaller. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0"))
	float BaseDamage = 80.0f;

	/** Initial throw speed of the grenade (cm/s). */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0", Units = "cm/s"))
	float ThrowSpeed = 1800.0f;

	/** Distance ahead of the camera to spawn the grenade, avoiding self-collision. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0", Units = "cm"))
	float SpawnOffset = 75.0f;

	/** Pitch added to the throw direction so the grenade arcs upward (degrees). */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "-45", ClampMax = "45", Units = "deg"))
	float UpwardPitchDegrees = 5.0f;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
