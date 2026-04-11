#include "AbilitySystem/EC_GameplayAbility.h"
#include "Characters/EC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"

UEC_GameplayAbility::UEC_GameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

AEC_PlayerCharacter* UEC_GameplayAbility::GetEC_PlayerCharacter() const
{
	return Cast<AEC_PlayerCharacter>(GetAvatarActorFromActorInfo());
}
