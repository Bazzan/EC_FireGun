#include "AbilitySystem/EC_AbilitySet.h"
#include "AbilitySystemComponent.h"
#include "EC_FireGun.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"

void UEC_GameplayAbilitySet::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	for (int32 i = 0; i < Abilities.Num(); ++i)
	{
		if (!Abilities[i].AbilityClass)
		{
			UE_LOG(LogEC_FireGun, Warning, TEXT("%s: Abilities[%d] has null AbilityClass"), *GetName(), i);
		}
	}
	for (int32 i = 0; i < PassiveEffects.Num(); ++i)
	{
		if (!PassiveEffects[i].EffectClass)
		{
			UE_LOG(LogEC_FireGun, Warning, TEXT("%s: PassiveEffects[%d] has null EffectClass"), *GetName(), i);
		}
	}
}

void UEC_GameplayAbilitySet::GrantToASC(
	UAbilitySystemComponent* ASC,
	UObject* SourceObject,
	TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles,
	TArray<FActiveGameplayEffectHandle>& OutPassiveHandles) const
{
	if (!ASC)
	{
		return;
	}

	for (const FECAbilityGrant& Grant : Abilities)
	{
		if (!Grant.AbilityClass)
		{
			continue;
		}

		FGameplayAbilitySpec Spec(Grant.AbilityClass, Grant.Level, INDEX_NONE, SourceObject);
		const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
		if (Handle.IsValid())
		{
			OutAbilityHandles.Add(Handle);
		}
	}

	for (const FECPassiveEffectGrant& Grant : PassiveEffects)
	{
		if (!Grant.EffectClass)
		{
			continue;
		}

		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(SourceObject);
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Grant.EffectClass, Grant.Level, EffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			if (ActiveHandle.IsValid())
			{
				OutPassiveHandles.Add(ActiveHandle);
			}
		}
	}
}

#if WITH_EDITOR
EDataValidationResult UEC_GameplayAbilitySet::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = EDataValidationResult::Valid;

	for (int32 i = 0; i < Abilities.Num(); ++i)
	{
		const TSubclassOf<UGameplayAbility>& AbilityClass = Abilities[i].AbilityClass;
		if (!AbilityClass)
		{
			ValidationErrors.Add(FText::Format(
				NSLOCTEXT("EC_AbilitySet", "NullAbilityClass", "Abilities[{0}]: AbilityClass is null"),
				FText::AsNumber(i)));
			Result = EDataValidationResult::Invalid;
			continue;
		}

		if (!AbilityClass->HasAnyClassFlags(CLASS_Abstract) && AbilityClass->IsNative())
		{
			ValidationErrors.Add(FText::Format(
				NSLOCTEXT("EC_AbilitySet", "NativeAbilityClass", "Abilities[{0}]: \"{1}\" is a native C++ class. Use a Blueprint child instead (e.g. BP_{1}) so Blueprint events (ActivateAbility/EndAbility) can fire."),
				FText::AsNumber(i), FText::FromString(AbilityClass->GetName())));
			Result = EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif
