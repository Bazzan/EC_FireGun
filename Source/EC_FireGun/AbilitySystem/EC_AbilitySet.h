#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "EC_AbilitySet.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FECAbilityGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Level = 1;
};

USTRUCT(BlueprintType)
struct FECPassiveEffectGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passive")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passive")
	int32 Level = 1;
};

UCLASS(BlueprintType)
class EC_FIREGUN_API UEC_GameplayAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<FECAbilityGrant> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passives")
	TArray<FECPassiveEffectGrant> PassiveEffects;

	void GrantToASC(UAbilitySystemComponent* ASC,
					UObject* SourceObject,
					TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles,
					TArray<FActiveGameplayEffectHandle>& OutPassiveHandles) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
