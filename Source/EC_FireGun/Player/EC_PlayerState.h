// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "EC_PlayerState.generated.h"

class UAbilitySystemComponent;
class UEC_AttributeSet;

/**
 * PlayerState-owned Ability System (Lyra-style): attributes and abilities persist across pawn respawn.
 * Pawns (e.g. AEC_PlayerCharacter) call InitAbilityActorInfo(this, Pawn) to bind the avatar.
 * AI minions should keep a UAbilitySystemComponent on the pawn instead — not this class.
 */
UCLASS()
class EC_FIREGUN_API AEC_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEC_PlayerState();

	//~Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End IAbilitySystemInterface

	UEC_AttributeSet* GetECAttributeSet() const { return ECAttributeSet; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UEC_AttributeSet> ECAttributeSet;
};
