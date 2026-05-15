# Project Status

*Last updated: 2026-05-15*

## Current Focus
<!-- What is actively being worked on right now -->

- [2026-05-15] Change all player abilities from `ServerInitiated` to `LocalPredicted` net execution policy for responsive feel in co-op (listen-server model). Update both C++ classes and Blueprint defaults.
- [2026-05-14] Blueprint setup (remaining): create `AS_DefaultAbilities` (shared) + per-class ability sets with Ultimate/Grenade/passives, assign on character BPs (see `Docs/Blueprint_TODOs.md`).

## Recently Completed
<!-- Features/tasks finished in the last few sessions -->

- [2026-05-15] Renamed all custom ability/actor classes to follow `EC_` prefix convention. Added `ActiveClassRedirects` in `DefaultEngine.ini` for Blueprint compatibility.
- [2026-05-14] Added editor validation to `UEC_GameplayAbilitySet::IsDataValid` — warns when a native C++ ability class (e.g. `UDashAbility`) is assigned instead of a Blueprint child (e.g. `BP_DashAbility`), preventing BP event graph issues.
- [2026-05-14] Added `UDashAbility` C++ class: `LocalPredicted` net policy, single velocity override from movement input direction (~3m dash), ground friction disabled for dash duration, blocked by `State.Dead`/`Cooldown.Dash` tags. Added `Ability.Dash` and `Cooldown.Dash` gameplay tags.
- [2026-05-14] Added `ActivateAbility`/`EndAbility` overrides to `UEC_GameplayAbility` (call Super) so all derived abilities fire BP Event Graph by default.
- [2026-05-14] Removed custom `BP_OnFocusStart`/`BP_OnFocusEnd` hooks from `UGunslingerFocusAbility` — now uses standard `Super::ActivateAbility` → `K2_ActivateAbility()` instead.
- [2026-05-14] Added `ClassAbilitySet` property to `AEC_PlayerCharacter` for per-class abilities (Ultimate, Grenade, class passive).
- [2026-05-14] Fixed tag-based ability input binding: changed `ETriggerEvent::Triggered` → `ETriggerEvent::Started` to prevent double-activation on press+release.
- [2026-05-14] Created `DA_Input_PlayerCharacterAbilityBinds` with Ultimate/Grenade input bindings.
- [2026-05-14] Reparented all GE Blueprints (`GE_Damage`, `GE_FocusMode`, `GE_Cooldown_*`, etc.) from `UGameplayEffect` → `UEC_GameplayEffect`.
- [2026-05-09] Added `UEC_InputBindingSet` data asset (`AbilitySystem/EC_InputBindingSet.h/.cpp`) with `FECTagInputBinding` struct mapping `FGameplayTag → UInputAction*`. Replaced hard-wired `UltimateAction`/`GrenadeAction` properties on `AEC_PlayerCharacter` with unified tag-based input binding via `InputBindingSet` + `TryActivateAbilitiesByTag`. Removed `DoActivateUltimate()`/`DoActivateGrenade()`.
- [2026-05-09] Removed `InputTag` from `FECAbilityGrant` — ability activation uses ability's own `AbilityTags` (set via `SetAssetTags`) instead.
- [2026-05-09] Added `UEC_GameplayAbilitySet` data asset (`AbilitySystem/EC_AbilitySet.h/.cpp`) for data-driven ability + passive granting. `GrantToASC()` grants abilities and applies passives in one call, tracking both handle types.
- [2026-05-09] Extended `AEC_PlayerCharacter`: new `DefaultAbilitySet` (shared abilities + passive GEs) granted on possession, cleaned up on EndPlay.
- [2026-05-09] Extended `AShooterNPC`: new `DefaultAbilitySet` granted in `InitializeAbilitySystem()`, cleared on EndPlay.
- [2026-05-09] GAS-fied `AEC_MinionCharacter`: added pawn-level ASC + `UEC_AttributeSet`, `IAbilitySystemInterface`, `DefaultAbilitySet` support, health delegate, damage bridge, death flow with `State.Dead` tag, and `BP_OnDeath` hook.
- [2026-05-09] Changed `AbilitySystemComponent` replication mode from `Minimal` to `Mixed` on `AShooterNPC` and `AEC_PlayerState` so non-owning clients see gameplay effect tags for UI/feedback purposes.
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
<!-- resolved: AEC_MinionCharacter now has ASC + AttributeSet -->

## Notes
<!-- Anything else relevant to the next session -->

- **All player abilities should use `LocalPredicted` net execution policy** (not `ServerInitiated`) for responsive feel in co-op listen-server model. Currently only `UDashAbility` uses `LocalPredicted`; shoot abilities, ultimate, and grenade abilities still use `ServerInitiated`.
- `UDashAbility` is the first ability using `LocalPredicted` net execution policy (all others are `ServerInitiated`). This is intentional for responsive dash feel — the client predicts the velocity override locally, server reconciles.
- New log category `LogClassAbility` covers ultimates and grenade abilities (declared in `UltimateAbilityBase.h`).
- Focus damage bonus flows through `UEC_AttributeSet::OutgoingDamageMultiplier` (default 1.0). Hitscan reads it at apply-time; projectile and grenade abilities pre-multiply at spawn (so the value is locked in at fire-time, not impact-time).
- To test GAS shooting: create `GE_Damage` BP asset, create a BP child of `UGA_ShootHitscan` or `UGA_ShootProjectile`, then set `ShootAbilityClass` on the weapon BP. The weapon handles granting automatically.
- `AShooterNPC::CurrentHP` is now a read-only runtime mirror of the GAS Health attribute (no longer `EditAnywhere`). `DefaultMaxHealth` is the new `EditAnywhere` property for setting initial HP. StateTree bindings to `CurrentHP` continue to work.
- This file is maintained by agents. See `.cursor/rules/status-tracking.mdc` for the enforcement rule.
