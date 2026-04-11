#include "AbilitySystem/Abilities/GA_ShootBase.h"
#include "AbilitySystemComponent.h"
#include "Characters/EC_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogWeapon);

UGA_ShootBase::UGA_ShootBase()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(EC_GameplayTags::Ability_Shoot);
	SetAssetTags(Tags);

	ActivationBlockedTags.AddTag(EC_GameplayTags::State_Dead);
}

bool UGA_ShootBase::GetAimData(FAimData& OutAim) const
{
	const AEC_PlayerCharacter* PC = GetEC_PlayerCharacter();
	if (!PC)
	{
		return false;
	}

	const UCameraComponent* Camera = PC->GetFirstPersonCameraComponent();
	if (!Camera)
	{
		return false;
	}

	OutAim.Start     = Camera->GetComponentLocation();
	OutAim.Direction = Camera->GetForwardVector();
	OutAim.End       = OutAim.Start + (OutAim.Direction * MaxRange);
	return true;
}

bool UGA_ShootBase::ApplyDamageToTarget(UAbilitySystemComponent* TargetASC) const
{
	if (!TargetASC || !DamageEffect)
	{
		return false;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC)
	{
		return false;
	}

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), SourceASC->MakeEffectContext());
	if (!SpecHandle.IsValid())
	{
		return false;
	}

	SpecHandle.Data->SetSetByCallerMagnitude(EC_GameplayTags::Data_Damage, -BaseDamage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	return true;
}
