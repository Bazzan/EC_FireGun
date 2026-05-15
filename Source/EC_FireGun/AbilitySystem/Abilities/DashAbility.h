#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EC_GameplayAbility.h"
#include "DashAbility.generated.h"

UCLASS()
class EC_FIREGUN_API UEC_DashAbility : public UEC_GameplayAbility
{
	GENERATED_BODY()

public:
	UEC_DashAbility();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dash", meta = (ClampMin = "0", Units = "cm"))
	float DashDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash", meta = (ClampMin = "0.1", Units = "s"))
	float DashDuration = 0.2f;

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

private:
	void DisableGroundFriction(class UCharacterMovementComponent* MoveComp);
	void RestoreGroundFriction(class UCharacterMovementComponent* MoveComp);
	void OnDashFinished();

	FTimerHandle DashTimerHandle;

	float CachedGroundFriction = 0.0f;
	float CachedBrakingDecelerationWalking = 0.0f;
	float CachedBrakingFrictionFactor = 0.0f;
};
