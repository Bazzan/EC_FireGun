// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/EC_PlayerState.h"
#include "AbilitySystem/EC_AttributeSet.h"
#include "AbilitySystemComponent.h"

AEC_PlayerState::AEC_PlayerState()
{
	bReplicateUsingRegisteredSubObjectList = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	ECAttributeSet = CreateDefaultSubobject<UEC_AttributeSet>(TEXT("ECAttributeSet"));
	AbilitySystemComponent->AddSpawnedAttribute(ECAttributeSet);
}

UAbilitySystemComponent* AEC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
