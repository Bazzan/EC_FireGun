#include "AbilitySystem/EC_InputBindingSet.h"
#include "EC_FireGun.h"

void UEC_InputBindingSet::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	for (int32 i = 0; i < InputBindings.Num(); ++i)
	{
		const FECTagInputBinding& Binding = InputBindings[i];
		if (!Binding.AbilityTag.IsValid())
		{
			UE_LOG(LogEC_FireGun, Warning, TEXT("%s: InputBindings[%d] has invalid AbilityTag"), *GetName(), i);
		}
		if (!Binding.InputAction)
		{
			UE_LOG(LogEC_FireGun, Warning, TEXT("%s: InputBindings[%d] has null InputAction"), *GetName(), i);
		}
	}
}
