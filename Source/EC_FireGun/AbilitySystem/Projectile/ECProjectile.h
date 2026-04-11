#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ECProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Replicated traveling projectile spawned by GA_ShootProjectile.
 * Applies GAS-based damage on collision via a pre-built GameplayEffectSpec.
 */
UCLASS(Abstract)
class EC_FIREGUN_API AECProjectile : public AActor
{
	GENERATED_BODY()

public:
	AECProjectile();

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

	/** Initial / max speed of the projectile. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "0", Units = "cm/s"))
	float Speed = 5000.0f;

	/** Sphere collision radius. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = "1", Units = "cm"))
	float CollisionRadius = 16.0f;

	/** Physics impulse applied to hit actors with simulating physics. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit", meta = (ClampMin = "0"))
	float PhysicsForce = 100.0f;

	/** Seconds to wait after a hit before destroying. Zero destroys immediately. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit", meta = (ClampMin = "0", Units = "s"))
	float DestroyDelay = 3.0f;

	/** If true, the projectile can damage the actor that spawned it. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Hit")
	bool bCanDamageInstigator = false;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	/** Blueprint hook for cosmetic effects on hit (VFX, sound, etc.). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile", meta = (DisplayName = "On Projectile Impact"))
	void BP_OnImpact(const FHitResult& Hit);

private:
	bool bHasHit = false;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	float DamageValue = 0.0f;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> SourceASC;

	FTimerHandle DestroyTimerHandle;

	void ApplyDamageToActor(AActor* Target);
	void OnDestroyTimer();
};
