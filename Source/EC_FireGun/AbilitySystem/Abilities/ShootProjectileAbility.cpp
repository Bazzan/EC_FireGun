#include "AbilitySystem/Abilities/ShootProjectileAbility.h"
#include "AbilitySystem/Projectile/ECProjectile.h"
#include "AbilitySystem/EC_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

UShootProjectileAbility::UShootProjectileAbility()
{
	AbilityTags.AddTag(EC_GameplayTags::Ability_Shoot_Projectile);
}

void UShootProjectileAbility::ActivateAbility(
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

	FAimData Aim;
	if (!GetAimData(Aim))
	{
		UE_LOG(LogWeapon, Warning, TEXT("ShootProjectileAbility: could not get aim data"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ProjectileClass)
	{
		UE_LOG(LogWeapon, Error, TEXT("ShootProjectileAbility: ProjectileClass is null"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector SpawnLocation = Aim.Start + (Aim.Direction * SpawnOffset);
	const FRotator SpawnRotation = Aim.Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetAvatarActorFromActorInfo();
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());

	AECProjectile* Projectile = GetWorld()->SpawnActor<AECProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
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

		Projectile->InitDamage(DamageEffect, ScaledDamage, SourceASC);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
