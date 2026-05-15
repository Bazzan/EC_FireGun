#include "AbilitySystem/Abilities/DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "TimerManager.h"

UEC_DashAbility::UEC_DashAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTagContainer Tags;
	Tags.AddTag(EC_GameplayTags::Ability_Dash);
	SetAssetTags(Tags);

	ActivationBlockedTags.AddTag(EC_GameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(EC_GameplayTags::Cooldown_Dash);
}

void UEC_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (!MoveComp)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Determine dash direction from current movement input
	FVector DashDir = MoveComp->GetCurrentAcceleration().GetSafeNormal();

	// Fall back to last input vector if no current acceleration
	if (DashDir.IsNearlyZero())
	{
		DashDir = MoveComp->GetLastInputVector().GetSafeNormal();
	}

	// Final fallback to character forward
	if (DashDir.IsNearlyZero())
	{
		DashDir = Character->GetActorForwardVector();
	}

	const float DashSpeed = FMath::Max(1.0f, DashDistance / FMath::Max(0.016f, DashDuration));
	MoveComp->Velocity = DashDir * DashSpeed;

	// Call Super first so parent chain fires BP events (K2_ActivateAbility)
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// BP may have called EndAbility — respect that
	if (!IsActive())
	{
		return;
	}

	// Disable ground friction for the dash duration so velocity isn't eaten on the ground
	DisableGroundFriction(MoveComp);

	// End ability after the dash window so friction is restored
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(DashTimerHandle, this, &UEC_DashAbility::OnDashFinished, DashDuration, false);
	}
	else
	{
		OnDashFinished();
	}
}

void UEC_DashAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	// Safety restore if we're being cancelled early (e.g. stun)
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DashTimerHandle);
	}

	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
		{
			RestoreGroundFriction(MoveComp);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEC_DashAbility::DisableGroundFriction(UCharacterMovementComponent* MoveComp)
{
	CachedGroundFriction = MoveComp->GroundFriction;
	CachedBrakingDecelerationWalking = MoveComp->BrakingDecelerationWalking;
	CachedBrakingFrictionFactor = MoveComp->BrakingFrictionFactor;

	MoveComp->GroundFriction = 0.0f;
	MoveComp->BrakingDecelerationWalking = 0.0f;
	MoveComp->BrakingFrictionFactor = 0.0f;
}

void UEC_DashAbility::RestoreGroundFriction(UCharacterMovementComponent* MoveComp)
{
	MoveComp->GroundFriction = CachedGroundFriction;
	MoveComp->BrakingDecelerationWalking = CachedBrakingDecelerationWalking;
	MoveComp->BrakingFrictionFactor = CachedBrakingFrictionFactor;
}

void UEC_DashAbility::OnDashFinished()
{
	if (const FGameplayAbilitySpecHandle SpecHandle = GetCurrentAbilitySpecHandle();
		SpecHandle.IsValid() && IsActive())
	{
		if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
			{
				RestoreGroundFriction(MoveComp);
			}
		}

		EndAbility(SpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
