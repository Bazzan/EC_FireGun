#include "AbilitySystem/EC_GameplayAbility.h"
#include "Characters/EC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UEC_GameplayAbility::UEC_GameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEC_GameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEC_GameplayAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEC_GameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UEC_GameplayAbility::CommitAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UEC_GameplayAbility::PreActivate(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

bool UEC_GameplayAbility::ShouldAbilityRespondToEvent(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayEventData* Payload) const
{
	return Super::ShouldAbilityRespondToEvent(ActorInfo, Payload);
}

AEC_PlayerCharacter* UEC_GameplayAbility::GetEC_PlayerCharacter() const
{
	return Cast<AEC_PlayerCharacter>(GetAvatarActorFromActorInfo());
}

FActiveGameplayEffectHandle UEC_GameplayAbility::EC_ApplyGameplayEffectToOwner(
	TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, int32 Stacks)
{
	return BP_ApplyGameplayEffectToOwner(GameplayEffectClass, GameplayEffectLevel, Stacks);
}

FActiveGameplayEffectHandle UEC_GameplayAbility::EC_ApplyGameplayEffectSpecToOwner(
	const FGameplayEffectSpecHandle EffectSpecHandle)
{
	return K2_ApplyGameplayEffectSpecToOwner(EffectSpecHandle);
}

TArray<FActiveGameplayEffectHandle> UEC_GameplayAbility::EC_ApplyGameplayEffectToTarget(
	const FGameplayAbilityTargetDataHandle TargetData,
	TSubclassOf<UGameplayEffect> GameplayEffectClass,
	int32 GameplayEffectLevel,
	int32 Stacks)
{
	return BP_ApplyGameplayEffectToTarget(TargetData, GameplayEffectClass, GameplayEffectLevel, Stacks);
}

TArray<FActiveGameplayEffectHandle> UEC_GameplayAbility::EC_ApplyGameplayEffectSpecToTarget(
	const FGameplayEffectSpecHandle EffectSpecHandle,
	const FGameplayAbilityTargetDataHandle TargetData)
{
	return K2_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, TargetData);
}

void UEC_GameplayAbility::EC_RemoveGameplayEffectFromOwnerWithHandle(
	FActiveGameplayEffectHandle Handle, int32 StacksToRemove)
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(Handle, StacksToRemove);
}

void UEC_GameplayAbility::EC_RemoveGameplayEffectFromOwnerWithGrantedTags(
	FGameplayTagContainer WithGrantedTags, int32 StacksToRemove)
{
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(WithGrantedTags, StacksToRemove);
}

void UEC_GameplayAbility::EC_RemoveGameplayEffectFromOwnerWithAssetTags(
	FGameplayTagContainer WithAssetTags, int32 StacksToRemove)
{
	BP_RemoveGameplayEffectFromOwnerWithAssetTags(WithAssetTags, StacksToRemove);
}
