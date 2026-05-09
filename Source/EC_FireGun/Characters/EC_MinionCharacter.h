// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EC_Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "EC_MinionCharacter.generated.h"

class UAbilitySystemComponent;
class UEC_AttributeSet;
class UEC_GameplayAbilitySet;
struct FOnAttributeChangeData;

/**
 * AI/minion character with GAS support.
 * Defaults to orienting rotation to movement for navigation-driven motion.
 * Owns a pawn-level AbilitySystemComponent so GAS damage and abilities affect it.
 */
UCLASS()
class EC_FIREGUN_API AEC_MinionCharacter : public AEC_Character, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEC_MinionCharacter();

	/** Starting / max health used to initialize the GAS Health attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DefaultMaxHealth = 100.0f;

	/** Default abilities granted to this minion (e.g. charge, leap, death explosion) */
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UEC_GameplayAbilitySet> DefaultAbilitySet;

	//~Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End IAbilitySystemInterface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UEC_AttributeSet> ECAttributeSet;

	/** Actor tag to grant this character when it dies */
	UPROPERTY(EditAnywhere, Category = "Damage")
	FName DeathTag = FName("Dead");

	/** If true, this character has already died */
	bool bIsDead = false;

	/** Handles for cleanup on EndPlay */
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Initializes ASC actor info, health attributes, and grants default abilities. */
	void InitializeAbilitySystem();

	/** Callback when the Health attribute changes; triggers Die() at zero. */
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	/** Grants abilities from DefaultAbilitySet on authority. */
	void GrantDefaultAbilities();

	/** Clears all granted abilities on authority. */
	void ClearGrantedAbilities();

	/** Kills this minion. */
	void Die();

	/** BP hook called when the minion dies. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Minion", meta = (DisplayName = "On Death"))
	void BP_OnDeath();

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns true if this minion is dead. */
	bool IsDead() const;
};
