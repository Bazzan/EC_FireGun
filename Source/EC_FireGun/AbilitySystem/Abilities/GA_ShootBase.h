#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EC_GameplayAbility.h"
#include "GA_ShootBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, Log, All);

class UGameplayEffect;

/**
 * Shared base for all shooting abilities.
 * Provides aim calculation, a configurable damage effect, and common tags.
 * Children (hitscan / projectile) override ActivateAbility to implement
 * the actual hit-detection strategy.
 */
UCLASS(Abstract)
class EC_FIREGUN_API UGA_ShootBase : public UEC_GameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ShootBase();

protected:
	/** GameplayEffect applied to targets on hit (should use SetByCaller for Data.Damage). */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Base damage value passed to the DamageEffect via SetByCaller. */
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = "0"))
	float BaseDamage = 25.0f;

	/** Max distance for aim traces. */
	UPROPERTY(EditDefaultsOnly, Category = "Aim", meta = (ClampMin = "0", Units = "cm"))
	float MaxRange = 10000.0f;

	struct FAimData
	{
		FVector Start;
		FVector Direction;
		FVector End;
	};

	/**
	 * Reads the avatar pawn's camera to produce aim origin + direction + end point.
	 * Returns false if the avatar is invalid.
	 */
	bool GetAimData(FAimData& OutAim) const;

	/**
	 * Builds and applies the DamageEffect to a target ASC with BaseDamage as
	 * the SetByCaller magnitude (keyed by Data.Damage tag).
	 * Returns true if the effect was successfully applied.
	 */
	bool ApplyDamageToTarget(UAbilitySystemComponent* TargetASC) const;
};
