# Project Status

*Last updated: 2026-04-18*

## Current Focus
<!-- What is actively being worked on right now -->

- [2026-04-18] Gunslinger Ultimate + Grenade Blueprint setup: build `GE_FocusMode`, `GE_Cooldown_Ultimate`, `GE_Cooldown_Grenade`, `BP_UltimateAbility_GunslingerFocus`, `BP_FragGrenade`, `BP_GrenadeAbility_GunslingerFrag`, `IA_Ultimate` (X) and `IA_Grenade` (G), and assign on player BP (see `Docs/Blueprint_TODOs.md`).
- [2026-04-12] Hub map & editor wiring: assign `GM_Hub` override on LVL_Hub World Settings, assign `GM_FireGun` on run/gameplay maps.
- [2026-04-12] Verify any non-EC BP game modes have `PlayerStateClass` set to `AEC_PlayerState` for GAS support.
- [2026-04-12] Listen-server PIE test (host + 2 clients): confirm health/UI, player state, and game mode per map.
- [2026-04-12] GAS shooting Blueprint setup: create `GE_Damage`, `BP_Projectile`, `BP_ShootHitscan`, `BP_ShootProjectile`, assign `ShootAbilityClass` on weapon BPs (see `Docs/Blueprint_TODOs.md`).

## Recently Completed
<!-- Features/tasks finished in the last few sessions -->

- [2026-04-18] Added `AEC_GameSession` (new `LogECSession` category, `MaxPlayers=3` per Design.md §4, `ApproveLogin` override logs rejections) and wired `GameSessionClass` on `AEC_FireGunGameModeBase` so Hub and Run modes inherit the cap.
- [2026-04-18] Added Gunslinger Ultimate (Focus) and Frag Grenade scaffolding: new tags, `OutgoingDamageMultiplier` attribute (auto-applied in shoot/projectile/grenade pipelines), `UUltimateAbilityBase` + `UGunslingerFocusAbility`, `UECGrenadeAbility` + `AECGrenade` (impact-detonating, sphere-overlap GE damage with falloff), and `UltimateAbilityClass`/`GrenadeAbilityClass` + `UltimateAction`/`GrenadeAction` slots on `AEC_PlayerCharacter` granted on possession.
- [2026-04-18] Added Design.md §5.2 line for per-class **Grenade** slot (short cooldown, server-authoritative).
- [2026-04-18] Renamed C++ shoot abilities to drop `GA_` prefix (reserved for BPs): `UGA_ShootBase`/`Hitscan`/`Projectile` -> `UShootAbilityBase`/`UShootHitscanAbility`/`UShootProjectileAbility`; added `ActiveClassRedirects` so existing BP children keep working.
- [2026-04-12] Created hub map (`LVL_Hub`), start zone (`LVL_StartZone01`), and BP game modes (`GM_Hub`, `GM_FireGun`).
- [2026-04-12] Set project default game mode to `GM_Hub` and default/editor startup map to `LVL_Hub` in `DefaultEngine.ini`.
- [2026-04-11] Fixed `AECProjectile` collision: projectiles no longer block each other (`ECC_WorldDynamic` set to Ignore).
- [2026-04-11] Fixed GAS damage sign: `SetByCaller` magnitude is now negated in `GA_ShootBase` and `AECProjectile` so damage subtracts Health.
- [2026-04-11] Added pawn-owned ASC + `UEC_AttributeSet` to `AShooterNPC` so GAS shoot abilities can damage NPCs.
- [2026-04-11] Integrated GAS abilities into `AShooterWeapon`: weapon grants/revokes shoot ability on equip/unequip.

## Known Issues and Bugs
<!-- Active bugs, broken behavior, or regressions -->

- None recorded yet.

## Potential Risks
<!-- Things that might break, tech debt, or areas needing attention -->

- [2026-04-11] Legacy `AShooterProjectile` still uses `UGameplayStatics::ApplyDamage`. Weapons opted into GAS bypass this, but NPC-owned weapons and old BPs still use the legacy path. `AShooterNPC::TakeDamage` now bridges this into the ASC.
- [2026-04-11] `AEC_MinionCharacter` has no ASC; GAS damage will not affect it until one is added.

## Notes
<!-- Anything else relevant to the next session -->

- New log category `LogClassAbility` covers ultimates and grenade abilities (declared in `UltimateAbilityBase.h`).
- Focus damage bonus flows through `UEC_AttributeSet::OutgoingDamageMultiplier` (default 1.0). Hitscan reads it at apply-time; projectile and grenade abilities pre-multiply at spawn (so the value is locked in at fire-time, not impact-time).
- To test GAS shooting: create `GE_Damage` BP asset, create a BP child of `UGA_ShootHitscan` or `UGA_ShootProjectile`, then set `ShootAbilityClass` on the weapon BP. The weapon handles granting automatically.
- `AShooterNPC::CurrentHP` is now a read-only runtime mirror of the GAS Health attribute (no longer `EditAnywhere`). `DefaultMaxHealth` is the new `EditAnywhere` property for setting initial HP. StateTree bindings to `CurrentHP` continue to work.
- This file is maintained by agents. See `.cursor/rules/status-tracking.mdc` for the enforcement rule.
