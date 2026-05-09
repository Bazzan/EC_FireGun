// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/EC_MinionCharacter.h"
#include "AbilitySystem/EC_AbilitySet.h"
#include "AbilitySystem/EC_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EC_GameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

AEC_MinionCharacter::AEC_MinionCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ECAttributeSet = CreateDefaultSubobject<UEC_AttributeSet>(TEXT("ECAttributeSet"));
	AbilitySystemComponent->AddSpawnedAttribute(ECAttributeSet);
}

UAbilitySystemComponent* AEC_MinionCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEC_MinionCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
}

void AEC_MinionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UEC_AttributeSet::GetHealthAttribute()).RemoveAll(this);
	}

	ClearGrantedAbilities();

	Super::EndPlay(EndPlayReason);
}

void AEC_MinionCharacter::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		AbilitySystemComponent->SetNumericAttributeBase(
			UEC_AttributeSet::GetMaxHealthAttribute(), DefaultMaxHealth);
		AbilitySystemComponent->SetNumericAttributeBase(
			UEC_AttributeSet::GetHealthAttribute(), DefaultMaxHealth);

		GrantDefaultAbilities();
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UEC_AttributeSet::GetHealthAttribute())
		.AddUObject(this, &AEC_MinionCharacter::OnHealthAttributeChanged);
}

void AEC_MinionCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (HasAuthority() && Data.NewValue <= 0.0f && !bIsDead)
	{
		Die();
	}
}

void AEC_MinionCharacter::GrantDefaultAbilities()
{
	if (!DefaultAbilitySet || !AbilitySystemComponent)
	{
		return;
	}

	TArray<FActiveGameplayEffectHandle> DummyPassiveHandles;
	DefaultAbilitySet->GrantToASC(AbilitySystemComponent, this, GrantedAbilityHandles, DummyPassiveHandles);
}

void AEC_MinionCharacter::ClearGrantedAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		GrantedAbilityHandles.Reset();
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : GrantedAbilityHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}
	}
	GrantedAbilityHandles.Reset();
}

float AEC_MinionCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority() || bIsDead || !AbilitySystemComponent)
	{
		return 0.0f;
	}

	AbilitySystemComponent->ApplyModToAttribute(
		UEC_AttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage);

	return Damage;
}

void AEC_MinionCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	Tags.Add(DeathTag);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(EC_GameplayTags::State_Dead);
	}

	// Stop movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->StopActiveMovement();

	// Disable collision
	SetActorEnableCollision(false);

	// BP hook for death effects (e.g. play anim, spawn particles)
	BP_OnDeath();
}

bool AEC_MinionCharacter::IsDead() const
{
	return bIsDead;
}
