#include "AbilitySystem/Abilities/ShootAbilityBase.h"
#include "AbilitySystem/EC_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Characters/EC_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogWeapon);

UShootAbilityBase::UShootAbilityBase()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(EC_GameplayTags::Ability_Shoot);
	SetAssetTags(Tags);

	ActivationBlockedTags.AddTag(EC_GameplayTags::State_Dead);
}

bool UShootAbilityBase::GetAimData(FAimData& OutAim) const
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

bool UShootAbilityBase::ApplyDamageToTarget(UAbilitySystemComponent* TargetASC) const
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

	const float Multiplier = SourceASC->GetNumericAttribute(
		UEC_AttributeSet::GetOutgoingDamageMultiplierAttribute());
	const float ScaledDamage = BaseDamage * (Multiplier > 0.0f ? Multiplier : 1.0f);

	SpecHandle.Data->SetSetByCallerMagnitude(EC_GameplayTags::Data_Damage, -ScaledDamage);
	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	return true;
}
