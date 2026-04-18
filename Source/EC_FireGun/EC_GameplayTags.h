#pragma once

#include "NativeGameplayTags.h"

namespace EC_GameplayTags
{
	// Ability hierarchy
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot_Hitscan);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Shoot_Projectile);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Ultimate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Ultimate_Gunslinger_Focus);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grenade);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grenade_Gunslinger_Frag);

	// States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Focus);

	// Cooldowns (granted by cooldown GameplayEffects)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Ultimate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Grenade);

	// SetByCaller data keys
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_KillingBlow);
}
