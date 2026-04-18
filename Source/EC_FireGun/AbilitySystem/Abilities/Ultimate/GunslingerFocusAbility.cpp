#include "AbilitySystem/Abilities/Ultimate/GunslingerFocusAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "TimerManager.h"

UGunslingerFocusAbility::UGunslingerFocusAbility()
{
	FGameplayTagContainer Tags = GetAssetTags();
	Tags.AddTag(EC_GameplayTags::Ability_Ultimate_Gunslinger_Focus);
	SetAssetTags(Tags);
}

void UGunslingerFocusAbility::ActivateAbility(
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

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC || !BuffEffect)
	{
		UE_LOG(LogClassAbility, Warning, TEXT("GunslingerFocusAbility: missing ASC or BuffEffect"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	float Duration = FallbackDuration;
	if (const UGameplayEffect* DefaultGE = BuffEffect->GetDefaultObject<UGameplayEffect>())
	{
		// EGameplayEffectDurationType::HasDuration uses DurationMagnitude; fall back if not a simple ScalableFloat.
		float ResolvedDuration = 0.0f;
		if (DefaultGE->DurationMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), ResolvedDuration)
			&& ResolvedDuration > 0.0f)
		{
			Duration = ResolvedDuration;
		}
	}

	if (HasAuthority(&ActivationInfo))
	{
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(GetAvatarActorFromActorInfo());

		const FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(BuffEffect, GetAbilityLevel(), Context);
		if (Spec.IsValid())
		{
			ActiveBuffHandle = SourceASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
		}
	}

	BP_OnFocusStart(Duration);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			EndTimerHandle, this, &UGunslingerFocusAbility::OnFocusTimerExpired, Duration, false);
	}
}

void UGunslingerFocusAbility::OnFocusTimerExpired()
{
	if (const FGameplayAbilitySpecHandle SpecHandle = GetCurrentAbilitySpecHandle();
		SpecHandle.IsValid() && IsActive())
	{
		EndAbility(SpecHandle, GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UGunslingerFocusAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(EndTimerHandle);
	}

	if (HasAuthority(&ActivationInfo) && ActiveBuffHandle.IsValid())
	{
		if (UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo())
		{
			SourceASC->RemoveActiveGameplayEffect(ActiveBuffHandle);
		}
	}
	ActiveBuffHandle = FActiveGameplayEffectHandle();

	BP_OnFocusEnd();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
