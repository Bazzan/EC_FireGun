# Blueprint TODOs

*Last updated: 2026-05-14*

> **Editor validation note:** `UEC_GameplayAbilitySet` now validates that `AbilityClass` entries use Blueprint children (not native C++ classes). If you see a validation warning on `AS_DefaultAbilities`, make sure `Abilities[].AbilityClass` points to a BP asset (e.g. `BP_DashAbility`), not the C++ class (`UDashAbility`).

## GAS GameplayEffects — Reparent to EC_GameplayEffect

- [x] Reparent `GE_Damage` from `UGameplayEffect` to `UEC_GameplayEffect`
- [x] Reparent `GE_FocusMode` from `UGameplayEffect` to `UEC_GameplayEffect`
- [x] Reparent `GE_Cooldown_Ultimate` from `UGameplayEffect` to `UEC_GameplayEffect`
- [x] Reparent `GE_Cooldown_Grenade` from `UGameplayEffect` to `UEC_GameplayEffect`
- [x] Reparent any other existing GE Blueprints from `UGameplayEffect` to `UEC_GameplayEffect`

## Ability System — Data Assets

### Shared (set on `DefaultAbilitySet`)
- [x] Created `UDashAbility` C++ class with `Ability.Dash` asset tag, `ClientPredicted` net policy, single velocity override, and optional `CooldownEffect` slot.
- [ ] Create `AS_DefaultAbilities` (class: `UEC_GameplayAbilitySet`) with shared ability grants (Dash, Interact)
- [ ] Create `BP_DashAbility` (child of `UDashAbility`, set `CooldownEffect` to `GE_Cooldown_Dash` if desired) / `BP_InteractAbility` ability Blueprints with matching `AbilityTags` via `SetAssetTags` (e.g., `Ability.Dash`, `Ability.Interact`) and assign in `AS_DefaultAbilities` → `Abilities`
- [ ] Set `DefaultAbilitySet = AS_DefaultAbilities` on each player character BP and NPC/minion BPs

### Class-specific (set on `ClassAbilitySet`)
- [ ] Create per-class ability sets (e.g. `AS_GunslingerAbilities`, `AS_VanguardAbilities`) for class Ultimate, Grenade, and class passive
- [ ] Add `BP_UltimateAbility_GunslingerFocus` / `BP_FragGrenade` to the corresponding class ability set → `Abilities`
- [ ] Create passive GE Blueprints per class (e.g. `GE_VanguardPassive`) inheriting `UEC_GameplayEffect` and assign in the per-class ability set → `PassiveEffects`
- [ ] Set `ClassAbilitySet` on each class's player character BP

## Input Binding Set

- [x] Create `DA_Input_PlayerCharacterAbilityBinds` (class: `UEC_InputBindingSet`)
  - [x] `Ability.Ultimate` → `IA_Ultimate` (X)
  - [x] `Ability.Grenade` → `IA_Grenade` (G)
  - [ ] `Ability.Dash` → `IA_Dash` (Left Shift) *(tag exists in C++: `EC_GameplayTags::Ability_Dash`)*
  - [ ] `Ability.Interact` → `IA_Interact` (E)
- [ ] Set `InputBindingSet = IS_PlayerAbilities` on each player character BP (can be set on a shared parent BP)
- [ ] Remove old `UltimateAction`/`GrenadeAction` input assignments from any existing player BPs

## Ability BP Tags

- [ ] Existing abilities (`BP_UltimateAbility_GunslingerFocus`, `BP_FragGrenade`): ensure their `AssetTags` include `Ability.Ultimate` / `Ability.Grenade` respectively (via `SetAssetTags` in BP or C++ constructor)

## Net Execution Policy — All Player Abilities to LocalPredicted

- [ ] Review all player-facing ability C++ classes (`UShootAbilityBase`, `UShootHitscanAbility`, `UShootProjectileAbility`, `UUltimateAbilityBase`, `UECGrenadeAbility`, etc.) and set `NetExecutionPolicy = LocalPredicted` in their constructors
- [ ] Verify Blueprint ability defaults aren't overriding net policy back to `ServerInitiated`
- [ ] Ensure abilities properly use `HasAuthority()` / `ShouldAbilityActivateOnServer()` checks where needed for predicted execution
