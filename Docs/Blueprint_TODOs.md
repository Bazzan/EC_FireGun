# Blueprint TODOs

*Last updated: 2026-05-09*

## GAS GameplayEffects — Reparent to EC_GameplayEffect

- [ ] Reparent `GE_Damage` from `UGameplayEffect` to `UEC_GameplayEffect`
- [ ] Reparent `GE_FocusMode` from `UGameplayEffect` to `UEC_GameplayEffect`
- [ ] Reparent `GE_Cooldown_Ultimate` from `UGameplayEffect` to `UEC_GameplayEffect`
- [ ] Reparent `GE_Cooldown_Grenade` from `UGameplayEffect` to `UEC_GameplayEffect`
- [ ] Reparent any other existing GE Blueprints from `UGameplayEffect` to `UEC_GameplayEffect`

## Ability System — Data Assets

- [ ] Create `AS_DefaultAbilities` (class: `UEC_GameplayAbilitySet`) with shared ability grants (Dash, Interact, etc.)
- [ ] Create `BP_DashAbility` / `BP_InteractAbility` ability Blueprints with matching `AbilityTags` via `SetAssetTags` (e.g., `Ability.Dash`, `Ability.Interact`) and assign in `AS_DefaultAbilities` → `Abilities` array
- [ ] Create passive GE Blueprints per class (e.g. `GE_VanguardPassive`) inheriting `UEC_GameplayEffect` and assign in `AS_DefaultAbilities` → `PassiveEffects` array
- [ ] Assign `DefaultAbilitySet` on each player character BP and NPC/minion BPs

## Input Binding Set

- [ ] Create `IS_PlayerAbilities` (class: `UEC_InputBindingSet`) with tag→input mappings:
  - `Ability.Ultimate` → `IA_Ultimate` (X)
  - `Ability.Grenade` → `IA_Grenade` (G)
  - `Ability.Dash` → `IA_Dash` (Left Shift)
  - `Ability.Interact` → `IA_Interact` (E)
- [ ] Set `InputBindingSet = IS_PlayerAbilities` on each player character BP (can be set on a shared parent BP)
- [ ] Remove old `UltimateAction`/`GrenadeAction` input assignments from any existing player BPs

## Ability BP Tags

- [ ] Existing abilities (`BP_UltimateAbility_GunslingerFocus`, `BP_FragGrenade`): ensure their `AssetTags` include `Ability.Ultimate` / `Ability.Grenade` respectively (via `SetAssetTags` in BP or C++ constructor)
