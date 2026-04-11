// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/EC_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UEC_AttributeSet::UEC_AttributeSet()
{
	InitHealth(500.0f);
	InitMaxHealth(500.0f);
}

void UEC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEC_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEC_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UEC_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UEC_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEC_AttributeSet, Health, OldValue);
}

void UEC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEC_AttributeSet, MaxHealth, OldValue);
}
