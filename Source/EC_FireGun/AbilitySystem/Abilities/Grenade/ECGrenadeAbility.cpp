#include "AbilitySystem/Abilities/Grenade/ECGrenadeAbility.h"
#include "AbilitySystem/Grenade/ECGrenade.h"
#include "AbilitySystem/Abilities/Ultimate/UltimateAbilityBase.h"
#include "AbilitySystem/EC_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/EC_PlayerCharacter.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"

UECGrenadeAbility::UECGrenadeAbility()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(EC_GameplayTags::Ability_Grenade);
	SetAssetTags(Tags);

	ActivationBlockedTags.AddTag(EC_GameplayTags::State_Dead);
	ActivationBlockedTags.AddTag(EC_GameplayTags::Cooldown_Grenade);
}

void UECGrenadeAbility::ActivateAbility(
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

	if (!HasAuthority(&ActivationInfo))
	{
		// Spawn happens server-side; client predicts only the cooldown commit.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (!GrenadeClass)
	{
		UE_LOG(LogClassAbility, Error, TEXT("ECGrenadeAbility: GrenadeClass is null"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AEC_PlayerCharacter* Avatar = GetEC_PlayerCharacter();
	const UCameraComponent* Camera = Avatar ? Avatar->GetFirstPersonCameraComponent() : nullptr;
	if (!Avatar || !Camera)
	{
		UE_LOG(LogClassAbility, Warning, TEXT("ECGrenadeAbility: missing avatar or camera"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector Origin = Camera->GetComponentLocation();
	FRotator AimRotation = Camera->GetComponentRotation();
	AimRotation.Pitch += UpwardPitchDegrees;
	const FVector Direction = AimRotation.Vector();
	const FVector SpawnLocation = Origin + (Direction * SpawnOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = const_cast<AEC_PlayerCharacter*>(Avatar);
	SpawnParams.Instigator = const_cast<AEC_PlayerCharacter*>(Avatar);

	AECGrenade* Grenade = GetWorld()->SpawnActor<AECGrenade>(
		GrenadeClass, SpawnLocation, AimRotation, SpawnParams);

	if (Grenade)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

		float ScaledDamage = BaseDamage;
		if (SourceASC)
		{
			const float Multiplier = SourceASC->GetNumericAttribute(
				UEC_AttributeSet::GetOutgoingDamageMultiplierAttribute());
			if (Multiplier > 0.0f)
			{
				ScaledDamage *= Multiplier;
			}
		}

		Grenade->InitDamage(DamageEffect, ScaledDamage, SourceASC);

		if (UProjectileMovementComponent* PMC = Grenade->FindComponentByClass<UProjectileMovementComponent>())
		{
			PMC->Velocity = Direction * ThrowSpeed;
		}
	}
	else
	{
		UE_LOG(LogClassAbility, Warning, TEXT("ECGrenadeAbility: failed to spawn grenade"));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
