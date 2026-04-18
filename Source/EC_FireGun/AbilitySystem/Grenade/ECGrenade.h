#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ECGrenade.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Replicated thrown grenade spawned by UECGrenadeAbility.
 * Detonates on first impact (with a safety MaxLifetime fallback) and applies
 * a GAS GameplayEffect to every pawn-with-ASC inside ExplosionRadius.
 *
 * Listen-server: detonation, overlap detection, and effect application all run on the host.
 */
UCLASS(Abstract)
class EC_FIREGUN_API AECGrenade : public AActor
{
	GENERATED_BODY()

public:
	AECGrenade();

	/**
	 * Called by the spawning ability to pass damage context.
	 * Must be called immediately after spawn, before the first tick.
	 */
	void InitDamage(
		TSubclassOf<UGameplayEffect> InDamageEffect,
		float InDamage,
		UAbilitySystemComponent* InSourceASC);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Initial throw speed applied by the spawning ability via the projectile movement component. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0", Units = "cm/s"))
	float InitialSpeed = 1800.0f;

	/** Sphere collision radius. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "1", Units = "cm"))
	float CollisionRadius = 8.0f;

	/** Radius of the explosion overlap query. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "1", Units = "cm"))
	float ExplosionRadius = 350.0f;

	/** If true, damage at the edge of ExplosionRadius linearly falls to MinDamageScale. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	bool bUseDamageFalloff = true;

	/** Fraction of full damage applied at the very edge of ExplosionRadius (only when bUseDamageFalloff). */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0", ClampMax = "1", EditCondition = "bUseDamageFalloff"))
	float MinDamageScale = 0.25f;

	/** Safety fuse: if the grenade somehow never collides, detonate after this many seconds. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0.1", Units = "s"))
	float MaxLifetime = 5.0f;

	/** Seconds to wait after detonation before destroying (lets cosmetic effects play). */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0", Units = "s"))
	float DestroyDelay = 2.0f;

	/** If true, the explosion can damage the actor that spawned the grenade. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	bool bCanDamageInstigator = false;

	/** Physics impulse applied to overlapping primitives that simulate physics. */
	UPROPERTY(EditDefaultsOnly, Category = "Grenade", meta = (ClampMin = "0"))
	float PhysicsForce = 600.0f;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	/** Cosmetic Blueprint hook fired on detonation (for VFX/SFX/decals). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Grenade", meta = (DisplayName = "On Explode"))
	void BP_OnExplode(const FVector& Location);

private:
	bool bHasExploded = false;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	float DamageValue = 0.0f;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> SourceASC;

	FTimerHandle LifetimeTimerHandle;
	FTimerHandle DestroyTimerHandle;

	void Explode();
	void OnLifetimeExpired();
	void OnDestroyTimer();
	void ApplyExplosionDamageToActor(AActor* Target, const FVector& ExplosionLocation);
};
