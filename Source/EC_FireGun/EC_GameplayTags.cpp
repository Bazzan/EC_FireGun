#include "EC_GameplayTags.h"

namespace EC_GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ability_Shoot,                       "Ability.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Shoot_Hitscan,               "Ability.Shoot.Hitscan");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Shoot_Projectile,            "Ability.Shoot.Projectile");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Ultimate,                    "Ability.Ultimate");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Ultimate_Gunslinger_Focus,   "Ability.Ultimate.Gunslinger.Focus");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Grenade,                     "Ability.Grenade");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Grenade_Gunslinger_Frag,     "Ability.Grenade.Gunslinger.Frag");

	UE_DEFINE_GAMEPLAY_TAG(State_Firing,                        "State.Firing");
	UE_DEFINE_GAMEPLAY_TAG(State_Dead,                          "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(State_Focus,                         "State.Focus");

	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Ultimate,                   "Cooldown.Ultimate");
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Grenade,                    "Cooldown.Grenade");

	UE_DEFINE_GAMEPLAY_TAG(Data_Damage,                         "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_KillingBlow,                    "Data.KillingBlow");
}
