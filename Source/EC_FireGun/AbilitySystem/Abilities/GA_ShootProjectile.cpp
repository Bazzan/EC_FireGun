#include "AbilitySystem/Abilities/GA_ShootProjectile.h"
#include "AbilitySystem/Projectile/ECProjectile.h"
#include "AbilitySystemComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

UGA_ShootProjectile::UGA_ShootProjectile()
{
	AbilityTags.AddTag(EC_GameplayTags::Ability_Shoot_Projectile);
}

void UGA_ShootProjectile::ActivateAbility(
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
		UE_LOG(LogWeapon, Warning, TEXT("GA_ShootProjectile: could not get aim data"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ProjectileClass)
	{
		UE_LOG(LogWeapon, Error, TEXT("GA_ShootProjectile: ProjectileClass is null"));
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
		Projectile->InitDamage(
			DamageEffect,
			BaseDamage,
			GetAbilitySystemComponentFromActorInfo());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
