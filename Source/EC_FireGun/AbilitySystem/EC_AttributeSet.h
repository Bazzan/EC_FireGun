// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EC_AttributeSet.generated.h"

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

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
};
