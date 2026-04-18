#include "AbilitySystem/Abilities/ShootHitscanAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"

UShootHitscanAbility::UShootHitscanAbility()
{
	AbilityTags.AddTag(EC_GameplayTags::Ability_Shoot_Hitscan);
}

void UShootHitscanAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, /*bReplicateEndAbility=*/ true, /*bWasCancelled=*/ true);
		return;
	}

	FAimData Aim;
	if (!GetAimData(Aim))
	{
		UE_LOG(LogWeapon, Warning, TEXT("ShootHitscanAbility: could not get aim data"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AActor* AvatarActor = GetAvatarActorFromActorInfo();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Aim.Start, Aim.End, TraceChannel, QueryParams);

	if (bHit && Hit.GetActor())
	{
		if (const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Hit.GetActor()))
		{
			if (UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent())
			{
				ApplyDamageToTarget(TargetASC);
			}
		}

		BP_OnHitscanHit(Hit);
	}
	else
	{
		BP_OnHitscanMiss(Aim.End);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
