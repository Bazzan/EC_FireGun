// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EC_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Core replicated attributes for EC_FireGun characters.
 * Health / MaxHealth replicate to every machine (COND_None) for team UI and future client-side work.
 */
UCLASS()
class EC_FIREGUN_API UEC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEC_AttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	ATTRIBUTE_ACCESSORS(UEC_AttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UEC_AttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UEC_AttributeSet, OutgoingDamageMultiplier);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	/** Multiplier applied to outgoing ability damage at the source (default 1.0). Buffs like Focus scale this. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingDamageMultiplier, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData OutgoingDamageMultiplier;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldValue);
};
