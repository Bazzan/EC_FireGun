#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EC_InputBindingSet.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FECTagInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction = nullptr;
};

UCLASS(BlueprintType)
class EC_FIREGUN_API UEC_InputBindingSet : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<FECTagInputBinding> InputBindings;
};
