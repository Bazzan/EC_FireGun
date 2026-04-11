# Project Status

*Last updated: 2026-04-11*

## Current Focus
<!-- What is actively being worked on right now -->

- [2026-04-11] GAS shooting abilities need a Blueprint GE_Damage GameplayEffect asset (SetByCaller Data.Damage → Health) before testing.
- [2026-04-11] Weapon BPs need `ShootAbilityClass` assigned to opt into GAS shooting (see `Docs/Blueprint_TODOs.md`).

## Recently Completed
<!-- Features/tasks finished in the last few sessions -->

- [2026-04-11] Fixed `AECProjectile` collision: projectiles no longer block each other (`ECC_WorldDynamic` set to Ignore).
- [2026-04-11] Fixed GAS damage sign: `SetByCaller` magnitude is now negated in `GA_ShootBase` and `AECProjectile` so damage subtracts Health.
- [2026-04-11] Added pawn-owned ASC + `UEC_AttributeSet` to `AShooterNPC` so GAS shoot abilities can damage NPCs. `TakeDamage` bridges legacy damage into the ASC. `State.Dead` tag applied on death.
- [2026-04-11] Integrated GAS abilities into `AShooterWeapon`: weapon grants/revokes shoot ability on equip/unequip, `FireProjectile` routes through ASC when configured.
- [2026-04-11] Added native gameplay tags (`EC_GameplayTags.h/.cpp`): Ability.Shoot hierarchy, State.Firing, State.Dead, Data.Damage.
- [2026-04-11] Created project base ability class `UEC_GameplayAbility` (ServerInitiated, InstancedPerActor).
- [2026-04-11] Created `UGA_ShootBase`, `UGA_ShootHitscan`, `UGA_ShootProjectile`, `AECProjectile`.
- [2026-04-11] Added `LogWeapon` log category, `GameplayTasks` module dependency, new include paths.
- [2026-04-11] Fixed `ATTRIBUTE_ACCESSORS` compile errors in `EC_AttributeSet.h`.
- [2026-04-11] Fixed deprecated `AbilityTags` → `SetAssetTags()` in `GA_ShootBase`.
- [2026-04-11] Added "GAS damage & effects rule" to Design.md and agent rules.
- [2026-04-11] Initial STATUS.md created for project tracking.

## Known Issues and Bugs
<!-- Active bugs, broken behavior, or regressions -->

- None recorded yet.

## Potential Risks
<!-- Things that might break, tech debt, or areas needing attention -->

- [2026-04-11] Legacy `AShooterProjectile` still uses `UGameplayStatics::ApplyDamage`. Weapons opted into GAS bypass this, but NPC-owned weapons and old BPs still use the legacy path. `AShooterNPC::TakeDamage` now bridges this into the ASC.
- [2026-04-11] `AEC_MinionCharacter` has no ASC; GAS damage will not affect it until one is added.

## Notes
<!-- Anything else relevant to the next session -->

- To test GAS shooting: create `GE_Damage` BP asset, create a BP child of `UGA_ShootHitscan` or `UGA_ShootProjectile`, then set `ShootAbilityClass` on the weapon BP. The weapon handles granting automatically.
- `AShooterNPC::CurrentHP` is now a read-only runtime mirror of the GAS Health attribute (no longer `EditAnywhere`). `DefaultMaxHealth` is the new `EditAnywhere` property for setting initial HP. StateTree bindings to `CurrentHP` continue to work.
- This file is maintained by agents. See `.cursor/rules/status-tracking.mdc` for the enforcement rule.
