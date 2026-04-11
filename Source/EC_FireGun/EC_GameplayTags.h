#pragma once

#include "NativeGameplayTags.h"

namespace EC_GameplayTags
{
	// Ability hierarchy
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot_Hitscan);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot_Projectile);

	// States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);

	// SetByCaller data keys
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
}
